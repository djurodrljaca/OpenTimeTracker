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
#include "Schedule.hpp"

using namespace OpenTimeTracker::Server;

Schedule::Schedule()
    : m_id(0LL),
      m_userId(0LL),
      m_startTimestamp(),
      m_endTimestamp()
{
}

Schedule::Schedule(const Schedule &other)
    : m_id(other.m_id),
      m_userId(other.m_userId),
      m_startTimestamp(other.m_startTimestamp),
      m_endTimestamp(other.m_endTimestamp)
{
}

Schedule &Schedule::operator =(const Schedule &other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_userId = other.m_userId;
        m_startTimestamp = other.m_startTimestamp;
        m_endTimestamp = other.m_endTimestamp;
    }

    return *this;
}

bool Schedule::isValid() const
{
    bool valid = true;

    if ((m_id < 1LL) ||
        (m_userId < 1LL) ||
        (!m_startTimestamp.isValid()) ||
        (!m_endTimestamp.isValid()))
    {
        valid = false;
    }
    else if (m_startTimestamp >= m_endTimestamp)
    {
        valid = false;
    }
    else
    {
        // Valid
    }

    return valid;
}

qint64 Schedule::id() const
{
    return m_id;
}

void Schedule::setId(const qint64 &newId)
{
    m_id = newId;
}

qint64 Schedule::userId() const
{
    return m_userId;
}

void Schedule::setUserId(const qint64 &newUserId)
{
    m_userId = newUserId;
}

QDateTime Schedule::startTimestamp() const
{
    return m_startTimestamp;
}

void Schedule::setStartTimestamp(const QDateTime &newStartTimestamp)
{
    m_startTimestamp = newStartTimestamp;
}

QDateTime Schedule::endTimestamp() const
{
    return m_endTimestamp;
}

void Schedule::setEndTimestamp(const QDateTime &newEndTimestamp)
{
    m_endTimestamp = newEndTimestamp;
}

Schedule Schedule::fromMap(const QMap<QString, QVariant> &map)
{
    Schedule schedule;

    if (map.size() == 4)
    {
        bool success = false;

        // Get schedule ID
        QVariant value = map["id"];

        if (value.canConvert<qint64>())
        {
            schedule.setId(value.toLongLong(&success));
        }

        // Get schedule change log item user ID
        if (success)
        {
            value = map["userId"];

            if (value.canConvert<qint64>())
            {
                schedule.setUserId(value.toLongLong(&success));
            }
            else
            {
                success = false;
            }
        }

        // Get schedule start timestamp
        if (success)
        {
            value = map["startTimestamp"];

            if (value.canConvert<QString>())
            {
                // Since start timestamp in the database is stored in UTC it is converted to local
                // time here before it is stored to the schedule
                QDateTime startTimestamp = QDateTime::fromString(value.toString(), Qt::ISODate);
                startTimestamp.setTimeSpec(Qt::UTC);
                schedule.setStartTimestamp(startTimestamp.toLocalTime());
            }
            else
            {
                success = false;
            }
        }

        // Get schedule end timestamp
        if (success)
        {
            value = map["endTimestamp"];

            if (value.canConvert<QString>())
            {
                // Since end timestamp in the database is stored in UTC it is converted to local
                // time here before it is stored to the schedule
                QDateTime endTimestamp = QDateTime::fromString(value.toString(), Qt::ISODate);
                endTimestamp.setTimeSpec(Qt::UTC);
                schedule.setEndTimestamp(endTimestamp.toLocalTime());
            }
            else
            {
                success = false;
            }
        }

        // On error clear the object
        if (!success)
        {
            schedule = Schedule();
        }
    }

    return schedule;
}
