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
#include "Event.hpp"

using namespace OpenTimeTracker::Server;

Event::Event()
    : m_id(0LL),
      m_timestamp(),
      m_userId(0LL),
      m_type(Type_Invalid),
      m_enabled(false)
{
}

Event::Event(const Event &other)
    : m_id(other.m_id),
      m_timestamp(other.m_timestamp),
      m_userId(other.m_userId),
      m_type(other.m_type),
      m_enabled(other.m_enabled)
{
}

Event &Event::operator =(const Event &other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_timestamp = other.m_timestamp;
        m_userId = other.m_userId;
        m_type = other.m_type;
        m_enabled = other.m_enabled;
    }

    return *this;
}

bool Event::isValid() const
{
    bool valid = true;

    if ((m_id < 1LL) || (!m_timestamp.isValid()) || (m_userId < 1LL) || (m_type == Type_Invalid))
    {
        valid = false;
    }

    return valid;
}

qint64 Event::id() const
{
    return m_id;
}

void Event::setId(const qint64 &newId)
{
    m_id = newId;
}

QDateTime Event::timestamp() const
{
    return m_timestamp;
}

void Event::setTimestamp(const QDateTime &newTimestamp)
{
    m_timestamp = newTimestamp;
}

qint64 Event::userId() const
{
    return m_userId;
}

void Event::setUserId(const qint64 &newUserId)
{
    m_userId = newUserId;
}

Event::Type Event::type() const
{
    return m_type;
}

void Event::setType(const Type &newType)
{
    if ((newType >= static_cast<int>(Type_Invalid)) ||
        (newType <= static_cast<int>(Type_Finished)))
    {
        m_type = newType;
    }
    else
    {
        m_type = Type_Invalid;
    }
}

bool Event::isEnabled() const
{
    return m_enabled;
}

void Event::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

Event Event::fromMap(const QMap<QString, QVariant> &map)
{
    Event event;

    if (map.size() == 5)
    {
        bool success = false;

        // Get event ID
        QVariant value = map["id"];

        if (value.canConvert<qint64>())
        {
            event.setId(value.toLongLong(&success));
        }

        // Get event timestamp
        if (success)
        {
            value = map["timestamp"];

            if (value.canConvert<QString>())
            {
                // Since timestamp in the database is stored in UTC it is converted to local time
                // here before it is stored to the event
                QDateTime timestamp = QDateTime::fromString(value.toString(), Qt::ISODate);
                timestamp.setTimeSpec(Qt::UTC);
                event.setTimestamp(timestamp.toLocalTime());
            }
            else
            {
                success = false;
            }
        }

        // Get event change log item user ID
        if (success)
        {
            value = map["userId"];

            if (value.canConvert<qint64>())
            {
                event.setUserId(value.toLongLong(&success));
            }
            else
            {
                success = false;
            }
        }

        // Get event type
        if (success)
        {
            value = map["type"];

            if (value.canConvert<int>())
            {
                int typeIntValue = value.toInt(&success);

                if (success)
                {
                    if ((typeIntValue >= static_cast<int>(Type_Invalid)) ||
                        (typeIntValue <= static_cast<int>(Type_Finished)))
                    {
                        event.setType(static_cast<Type>(typeIntValue));
                    }
                    else
                    {
                        success = false;
                    }
                }
            }
            else
            {
                success = false;
            }
        }

        // Get event enable state
        if (success)
        {
            value = map["enabled"];

            if (value.canConvert<int>())
            {
                int enabledIntValue = value.toInt(&success);

                if (success)
                {
                    switch (enabledIntValue)
                    {
                        case 0:
                        {
                            event.setEnabled(false);
                            break;
                        }

                        case 1:
                        {
                            event.setEnabled(true);
                            break;
                        }

                        default:
                        {
                            success = false;
                            break;
                        }
                    }
                }
            }
            else
            {
                success = false;
            }
        }

        // On error clear the object
        if (!success)
        {
            event = Event();
        }
    }

    return event;
}
