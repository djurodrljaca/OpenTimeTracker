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
#include "EventChangeLogItem.hpp"

using namespace OpenTimeTracker::Server;

EventChangeLogItem::EventChangeLogItem()
    : m_id(0LL),
      m_eventId(0LL),
      m_timestamp(),
      m_fieldName(),
      m_fromValue(),
      m_toValue(),
      m_userId(0LL),
      m_comment()
{
}

EventChangeLogItem::EventChangeLogItem(const EventChangeLogItem &other)
    : m_id(other.m_id),
      m_eventId(other.m_eventId),
      m_timestamp(other.m_timestamp),
      m_fieldName(other.m_fieldName),
      m_fromValue(other.m_fromValue),
      m_toValue(other.m_toValue),
      m_userId(other.m_userId),
      m_comment(other.m_comment)
{
}

EventChangeLogItem &EventChangeLogItem::operator =(const EventChangeLogItem &other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_eventId = other.m_eventId;
        m_timestamp = other.m_timestamp;
        m_fieldName = other.m_fieldName;
        m_fromValue = other.m_fromValue;
        m_toValue = other.m_toValue;
        m_userId = other.m_userId;
        m_comment = other.m_comment;
    }

    return *this;
}

bool EventChangeLogItem::isValid() const
{
    bool valid = true;

    if ((m_id < 1LL) ||
        (m_eventId < 1LL) ||
        (!m_timestamp.isValid()) ||
        m_fieldName.isEmpty() ||
        (!m_fromValue.isValid()) ||
        m_fromValue.isNull() ||
        (!m_toValue.isValid()) ||
        m_toValue.isNull() ||
        (m_userId < 1LL) ||
        m_comment.isEmpty())
    {
        valid = false;
    }

    return valid;
}

qint64 EventChangeLogItem::id() const
{
    return m_id;
}

void EventChangeLogItem::setId(const qint64 &newId)
{
    m_id = newId;
}

qint64 EventChangeLogItem::eventId() const
{
    return m_eventId;
}

void EventChangeLogItem::setEventId(const qint64 &newEventId)
{
    m_eventId = newEventId;
}

QDateTime EventChangeLogItem::timestamp() const
{
    return m_timestamp;
}

void EventChangeLogItem::setTimestamp(const QDateTime &newTimestamp)
{
    m_timestamp = newTimestamp;
}

QString EventChangeLogItem::fieldName() const
{
    return m_fieldName;
}

void EventChangeLogItem::setFieldName(const QString &newFieldName)
{
    m_fieldName = newFieldName;
}

QVariant EventChangeLogItem::fromValue() const
{
    return m_fromValue;
}

void EventChangeLogItem::setFromValue(const QVariant &newFromValue)
{
    m_fromValue = newFromValue;
}

QVariant EventChangeLogItem::toValue() const
{
    return m_toValue;
}

void EventChangeLogItem::setToValue(const QVariant &newToValue)
{
    m_toValue = newToValue;
}

qint64 EventChangeLogItem::userId() const
{
    return m_userId;
}

void EventChangeLogItem::setUserId(const qint64 &newUserId)
{
    m_userId = newUserId;
}

QString EventChangeLogItem::comment() const
{
    return m_comment;
}

void EventChangeLogItem::setComment(const QString &newComment)
{
    m_comment = newComment;
}

EventChangeLogItem EventChangeLogItem::fromMap(const QMap<QString, QVariant> &map)
{
    EventChangeLogItem eventChangeLogItem;

    if (map.size() == 8)
    {
        bool success = false;

        // Get event change log item ID
        QVariant value = map["id"];

        if (value.canConvert<qint64>())
        {
            eventChangeLogItem.setId(value.toLongLong(&success));
        }

        // Get event change log item event ID
        if (success)
        {
            value = map["eventId"];

            if (value.canConvert<qint64>())
            {
                eventChangeLogItem.setEventId(value.toLongLong(&success));
            }
            else
            {
                success = false;
            }
        }

        // Get event change log item timestamp
        if (success)
        {
            value = map["timestamp"];

            if (value.canConvert<QString>())
            {
                QDateTime timestamp = QDateTime::fromString(value.toString(), Qt::ISODate);
                timestamp.setTimeSpec(Qt::UTC);
                eventChangeLogItem.setTimestamp(timestamp);
            }
            else
            {
                success = false;
            }
        }

        // Get event change log item field name
        if (success)
        {
            value = map["fieldName"];

            if (value.canConvert<QString>())
            {
                eventChangeLogItem.setFieldName(value.toString());
            }
            else
            {
                success = false;
            }
        }

        // Get event change log item original value
        if (success)
        {
            value = map["fromValue"];

            if (value.isValid() && (!value.isNull()))
            {
                if (eventChangeLogItem.fieldName() == QStringLiteral("timestamp"))
                {
                    QDateTime timestamp = value.toDateTime();
                    timestamp.setTimeSpec(Qt::UTC);
                    eventChangeLogItem.setFromValue(timestamp);
                }
                else
                {
                    eventChangeLogItem.setFromValue(value);
                }
            }
            else
            {
                success = false;
            }
        }

        // Get event change log item new value
        if (success)
        {
            value = map["toValue"];

            if (value.isValid() && (!value.isNull()))
            {
                eventChangeLogItem.setToValue(value);
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
                eventChangeLogItem.setUserId(value.toLongLong(&success));
            }
            else
            {
                success = false;
            }
        }

        // Get event change log item comment
        if (success)
        {
            value = map["comment"];

            if (value.canConvert<QString>())
            {
                eventChangeLogItem.setComment(value.toString());
            }
            else
            {
                success = false;
            }
        }

        // On error clear the object
        if (!success)
        {
            eventChangeLogItem = EventChangeLogItem();
        }
    }

    return eventChangeLogItem;
}
