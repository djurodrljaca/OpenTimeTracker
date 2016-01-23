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
      m_workingPeriods(),
      m_breakPeriods(),
      m_state(State_NotWorking),
      m_lastEventTimestamp()
{
}

TimeTracker::TimeTracker(const TimeTracker &other)
    : m_userId(other.m_userId),
      m_breakTimeCalculator(other.m_breakTimeCalculator),
      m_schedules(other.m_schedules),
      m_workingPeriods(other.m_workingPeriods),
      m_breakPeriods(other.m_breakPeriods),
      m_state(other.m_state),
      m_lastEventTimestamp(other.m_lastEventTimestamp)
{
}

TimeTracker &TimeTracker::operator =(const TimeTracker &other)
{
    if (this != &other)
    {
        m_userId = other.m_userId;
        m_breakTimeCalculator = other.m_breakTimeCalculator;
        m_schedules = other.m_schedules;
        m_workingPeriods = other.m_workingPeriods;
        m_breakPeriods = other.m_breakPeriods;
        m_state = other.m_state;
        m_lastEventTimestamp = other.m_lastEventTimestamp;
    }

    return *this;
}

bool TimeTracker::isValid() const
{
    bool valid = true;

    if ((m_userId < 1LL) ||
        (!m_breakTimeCalculator.isValid()))
    {
        valid = false;
    }
    else if (((m_state == State_Working) || (m_state == State_OnBreak)) &&
             (!m_lastEventTimestamp.isValid()))
    {
        valid = false;
    }
    else if (m_workingPeriods.isEmpty() &&
             m_breakPeriods.isEmpty() &&
             (m_state == State_NotWorking) &&
             m_lastEventTimestamp.isValid())
    {
        // Last event timestamp should be invalid in this case
        valid = false;
    }
    else if (((!m_workingPeriods.isEmpty()) || (!m_breakPeriods.isEmpty())) &&
             (!m_lastEventTimestamp.isValid()))
    {
        // Last event timestamp should be valid in this case
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

qint32 TimeTracker::calculateWorkingTime(const QDateTime &timestamp) const
{
    qint32 workingTime = 0;

    if (isValid())
    {
        if (timestamp.isValid())
        {
            // Calculate working time for the already logged working periods
            workingTime = calculateScheduledTime(m_workingPeriods, timestamp);

            if ((m_state == State_Working) && (m_lastEventTimestamp < timestamp))
            {
                // Calculate working time for the current working period
                workingTime += calculateScheduledTime(m_lastEventTimestamp, timestamp);
            }
        }
    }

    return workingTime;
}

qint32 TimeTracker::calculateBreakTime(const QDateTime &timestamp) const
{
    qint32 breakTime = 0;

    if (isValid())
    {
        if (timestamp.isValid())
        {
            // Calculate break time for the already logged break periods
            breakTime = calculateScheduledTime(m_breakPeriods, timestamp);

            if ((m_state == State_OnBreak) && (m_lastEventTimestamp < timestamp))
            {
                // Calculate break time for the current break period
                breakTime += calculateScheduledTime(m_lastEventTimestamp, timestamp);
            }
        }
    }

    return breakTime;
}

qint32 TimeTracker::calculateTotalWorkingTime(const QDateTime &timestamp) const
{
    qint32 totalWorkingTime = 0;

    if (isValid())
    {
        // First get the working and break times
        const qint32 workingTime = calculateWorkingTime(timestamp);
        const qint32 breakTime = calculateBreakTime(timestamp);

        // Calculate total time
        const qint32 allowedBreakTime = m_breakTimeCalculator.calculate(workingTime, breakTime);
        totalWorkingTime = workingTime + allowedBreakTime;
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
        QDateTime endOfLastSchedule;

        // Verify schedules
        foreach (const Schedule &schedule, schedules)
        {
            if ((!schedule.isValid()) || (schedule.userId() != m_userId))
            {
                // Invalid schedule
                success = false;
            }
            else if (endOfLastSchedule.isNull())
            {
                // This is the first schedule checked, save its end timestamp
                endOfLastSchedule = schedule.endTimestamp();
            }
            else if (endOfLastSchedule <= schedule.startTimestamp())
            {
                // This schedule comes after the last schedule, save its end timestamp
                endOfLastSchedule = schedule.endTimestamp();
            }
            else
            {
                // Error, this schedule overlaps with the previous schedule
                success = false;
            }

            // On failure exit the loop
            if (!success)
            {
                break;
            }
        }

        // Start workday
        if (success)
        {
            m_breakTimeCalculator = breakTimeCalculator;
            m_schedules = schedules;
            m_workingPeriods.clear();
            m_breakPeriods.clear();
            m_state = State_NotWorking;
            m_lastEventTimestamp = QDateTime();

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
            if ((m_lastEventTimestamp.isValid() && (m_lastEventTimestamp <= timestamp)) ||
                (!m_lastEventTimestamp.isValid()))
            {
                // Start tracking users work time
                m_state = State_Working;
                m_lastEventTimestamp = timestamp;
                success = true;
            }
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
        if (timestamp.isValid() && (m_lastEventTimestamp <= timestamp))
        {
            // Add latest working period interval to the cumulative working periods
            m_workingPeriods.append(QPair<QDateTime, QDateTime>(m_lastEventTimestamp, timestamp));

            // Start tracking users break time
            m_state = State_OnBreak;
            m_lastEventTimestamp = timestamp;
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
        if (m_lastEventTimestamp.isValid() && (m_lastEventTimestamp <= timestamp))
        {
            // Add latest break period interval to the cumulative break periods
            m_breakPeriods.append(QPair<QDateTime, QDateTime>(m_lastEventTimestamp, timestamp));

            // Start tracking users work time
            m_state = State_Working;
            m_lastEventTimestamp = timestamp;
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
        if (m_lastEventTimestamp.isValid() && (m_lastEventTimestamp <= timestamp))
        {
            // Add latest working period interval to the cumulative working periods
            m_workingPeriods.append(QPair<QDateTime, QDateTime>(m_lastEventTimestamp, timestamp));

            // Stop tracking users working time
            m_state = State_NotWorking;
            m_lastEventTimestamp = timestamp;
            success = true;
        }
    }

    return success;
}

qint32 TimeTracker::calculateScheduledTime(const QDateTime &startTimestamp,
                                           const QDateTime &endTimestamp) const
{
    qint32 scheduledTime = 0;

    if (isValid() && (startTimestamp < endTimestamp))
    {
        QDateTime timestamp = startTimestamp;

        foreach (const Schedule &schedule, m_schedules)
        {
            // Check if all time has been accounted for
            if (endTimestamp <= schedule.startTimestamp())
            {
                // Stop calculation
                break;
            }

            // Calculate start timestamp based on the schedule
            QDateTime scheduledStartTimestamp = timestamp;

            if (scheduledStartTimestamp < schedule.startTimestamp())
            {
                scheduledStartTimestamp = schedule.startTimestamp();
            }

            // Calculate end timestamp based on the schedule
            QDateTime scheduledEndTimestamp = endTimestamp;

            if (scheduledEndTimestamp > schedule.endTimestamp())
            {
                scheduledEndTimestamp = schedule.endTimestamp();
            }

            // Calculate elapsed time
            scheduledTime += scheduledStartTimestamp.secsTo(scheduledEndTimestamp);
            timestamp = scheduledEndTimestamp;
        }
    }

    return scheduledTime;
}

qint32 TimeTracker::calculateScheduledTime(const QList<QPair<QDateTime, QDateTime> > &timePeriods,
                                           const QDateTime &timestamp) const
{
    qint32 time = 0;

    // Make a sum of elapsed times from all time periods considering the schedule
    for (int i = 0; i < timePeriods.size(); i++)
    {
        // Calculate elapsed time
        const QPair<QDateTime, QDateTime> &timePeriod = timePeriods.at(i);
        bool limitReached = false;

        if (timestamp <= timePeriod.first)
        {
            limitReached = true;
        }
        else if (timestamp <= timePeriod.second)
        {
            time += calculateScheduledTime(timePeriod.first, timestamp);
            limitReached = true;
        }
        else
        {
            time += calculateScheduledTime(timePeriod.first, timePeriod.second);
        }

        // Stop the calculation when the limit is reached
        if (limitReached)
        {
            break;
        }
    }

    return time;
}
