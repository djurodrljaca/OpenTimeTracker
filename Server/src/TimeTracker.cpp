/* Copyright 2015  Djuro Drljaca <djurodrljaca@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "TimeTracker.hpp"

using namespace OpenTimeTracker::Server;

TimeTracker::TimeTracker()
    : m_userId(0LL),
      m_breakTimeCalculator(),
      m_schedules(),
      m_workingTime(0),
      m_breakTime(0),
      m_state(State_NotWorking),
      m_stateChangedTimestamp()
{
}

TimeTracker::TimeTracker(const TimeTracker &other)
    : m_userId(other.m_userId),
      m_breakTimeCalculator(other.m_breakTimeCalculator),
      m_schedules(other.m_schedules),
      m_workingTime(other.m_workingTime),
      m_breakTime(other.m_breakTime),
      m_state(other.m_state),
      m_stateChangedTimestamp(other.m_stateChangedTimestamp)
{
}

TimeTracker &TimeTracker::operator =(const TimeTracker &other)
{
    if (this != &other)
    {
        m_userId = other.m_userId;
        m_breakTimeCalculator = other.m_breakTimeCalculator;
        m_schedules = other.m_schedules;
        m_workingTime = other.m_workingTime;
        m_breakTime = other.m_breakTime;
        m_state = other.m_state;
        m_stateChangedTimestamp = other.m_stateChangedTimestamp;
    }

    return *this;
}

bool TimeTracker::isValid() const
{
    bool valid = true;

    if ((m_userId < 1LL) ||
        (!m_breakTimeCalculator.isValid()) ||
        m_schedules.isEmpty() ||
        (m_workingTime < 0) ||
        (m_breakTime < 0))
    {
        valid = false;
    }
    else if (((m_state == State_Working) || (m_state == State_OnBreak)) &&
             (!m_stateChangedTimestamp.isValid()))
    {
        valid = false;
    }
    else if (m_stateChangedTimestamp.isValid())
    {
        valid = false;
    }
    else
    {
        foreach (const Schedule &schedule, m_schedules)
        {
            if ((!schedule.isValid()) || (schedule.userId() != m_userId))
            {
                valid = false;
                break;
            }
        }
    }

    return valid;
}

qint64 TimeTracker::userId() const
{
    return m_userId;
}

void TimeTracker::setUserId(const qint64 &newUserId)
{
    m_userId = newUserId;
}

qint32 TimeTracker::calculateWorkingTime() const
{
    qint32 workingTime = 0;

    if (isValid())
    {
        workingTime = m_workingTime;

        if (m_state == State_Working)
        {
            if (m_stateChangedTimestamp.isValid())
            {
                workingTime += calculateScheduledTime(m_stateChangedTimestamp,
                                                      QDateTime::currentDateTime());
            }
            else
            {
                workingTime = -1;
            }
        }
    }

    return workingTime;
}

qint32 TimeTracker::calculateBreakTime() const
{
    qint32 breakTime = 0;

    if (isValid())
    {
        breakTime = m_breakTime;

        if (m_state == State_OnBreak)
        {
            if (m_stateChangedTimestamp.isValid())
            {
                breakTime += calculateScheduledTime(m_stateChangedTimestamp,
                                                    QDateTime::currentDateTime());
            }
            else
            {
                breakTime = -1;
            }
        }
    }

    return breakTime;
}

qint32 TimeTracker::calculateTotalWorkingTime() const
{
    qint32 totalWorkingTime = 0;

    if (isValid())
    {
        // First get the working and break times
        const qint32 workingTime = calculateWorkingTime();
        const qint32 breakTime = calculateBreakTime();

        // Calculate total time
        totalWorkingTime = workingTime + m_breakTimeCalculator.calculate(workingTime, breakTime);

        if (breakTime < breakTimeLimit)
        {
            totalWorkingTime += breakTime;
        }
        else
        {
            totalWorkingTime += static_cast<qint32>(round(breakTimeLimit));
        }
    }

    return totalWorkingTime;
}

TimeTracker::State TimeTracker::state() const
{
    return m_state;
}

bool TimeTracker::startWorkday(const BreakTimeCalculator &breakTimeCalculator,
                               const QList<Schedule> &schedules)
{
    bool success = false;

    if (breakTimeCalculator.isValid())
    {
        success = true;

        foreach (const Schedule &schedule, m_schedules)
        {
            if ((!schedule.isValid()) || (schedule.userId() != m_userId))
            {
                valid = false;
                break;
            }
        }

        if (success)
        {
            m_breakTimeCalculator = breakTimeCalculator;
            m_schedules = schedules;
            m_workingTime = 0;
            m_breakTime = 0;
            m_state = State_NotWorking;
            m_stateChangedTimestamp = QDateTime();

            success = isValid();
        }
    }

    return success;
}

bool TimeTracker::startWorking(const QDateTime &timestamp)
{
    bool success = false;

    // Check current state
    if ((isValid()) && (m_state == State_NotWorking))
    {
        // Check if timestamp is valid
        if (timestamp.isValid())
        {
            // Start tracking users work time
            m_state = State_Working;
            m_stateChangedTimestamp = timestamp;
            success = true;
        }
    }

    return success;
}

bool TimeTracker::startBreak(const QDateTime &timestamp)
{
    bool success = false;

    // Check current state
    if ((isValid()) && (m_state == State_Working))
    {
        // Check if timestamp comes after the time user started working
        if (timestamp.isValid() && (timestamp >= m_stateChangedTimestamp))
        {
            // Add latest working time interval to the cumulative working time
            m_workingTime += calculateScheduledTime(m_stateChangedTimestamp, timestamp);

            // Start tracking users break time
            m_state = State_OnBreak;
            m_stateChangedTimestamp = timestamp;
            success = true;
        }
    }

    return success;
}

bool TimeTracker::endBreak(const QDateTime &timestamp)
{
    bool success = false;

    // Check current state
    if ((isValid()) && (m_state == State_OnBreak))
    {
        // Check if timestamp comes after the time user started their break
        if (timestamp.isValid() && (timestamp >= m_stateChangedTimestamp))
        {
            // Add latest break time interval to the cumulative break time
            m_breakTime += calculateScheduledTime(m_stateChangedTimestamp, timestamp);

            // Start tracking users work time
            m_state = State_Working;
            m_stateChangedTimestamp = timestamp;
            success = true;
        }
    }

    return success;
}

bool TimeTracker::stopWorking(const QDateTime &timestamp)
{
    bool success = false;

    // Check current state
    if ((isValid()) && (m_state == State_Working))
    {
        // Check if timestamp comes after the time user started working
        if (timestamp.isValid() && (timestamp >= m_stateChangedTimestamp))
        {
            // Add latest working time interval to the cumulative working time
            m_workingTime += calculateScheduledTime(m_stateChangedTimestamp, timestamp);

            // Stop tracking users working time
            m_state = State_NotWorking;
            m_stateChangedTimestamp = timestamp;
            success = true;
        }
    }

    return success;
}

qint32 TimeTracker::calculateScheduledTime(const QDateTime &startTimestamp,
                                           const QDateTime &endTimestamp) const
{
    qint32 scheduledTime = 0;

    if (isValid())
    {
        QDateTime timestamp = startTimestamp;

        foreach (const Schedule &schedule, m_schedules)
        {
            // Check for scheduled time
            if ((schedule.startTimestamp() <= timestamp) && (timestamp <= schedule.endTimestamp()))
            {
                if (endTimestamp < schedule.endTimestamp())
                {
                    // All time has been accounted for, stop calculation
                    scheduledTime += timestamp.secsTo(endTimestamp);
                    break;
                }
                else
                {
                    // All time has not been accounted for yet, continue calculation
                    scheduledTime += timestamp.secsTo(schedule.endTimestamp());
                    timestamp = schedule.endTimestamp();
                }
            }
        }
    }
    return scheduledTime;
}
