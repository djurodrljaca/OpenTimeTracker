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
#ifndef OPENTIMETRACKER_SERVER_EVENTCHANGELOGITEM_HPP
#define OPENTIMETRACKER_SERVER_EVENTCHANGELOGITEM_HPP

#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Tracks changes applied to event entries
 */
class EventChangeLogItem
{
public:
    /*!
     * \brief   Constructor
     */
    EventChangeLogItem();

    /*!
     * \brief   Copy constructor
     * \param   other   Object to be copied
     */
    EventChangeLogItem(const EventChangeLogItem &other);

    /*!
     * \brief   operator =
     * \param   other   Object to be copied
     *
     * \return  Reference to the this object
     */
    EventChangeLogItem &operator =(const EventChangeLogItem &other);

    /*!
     * \brief   Checks if object is valid
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    bool isValid() const;

    /*!
     * \brief   Gets event change log item's ID
     *
     * \return  Event change log item's ID
     */
    qint64 id() const;

    /*!
     * \brief   Sets event change log item's new ID
     *
     * \param   newId   Event change log item's new ID
     */
    void setId(const qint64 &newId);

    /*!
     * \brief   Gets event change log item's event ID
     *
     * \return  Event change log item's event ID
     */
    qint64 eventId() const;

    /*!
     * \brief   Sets event change log item's new event ID
     *
     * \param   newEventId  Event change log item's new event ID
     */
    void setEventId(const qint64 &newEventId);

    /*!
     * \brief   Gets event change log item's timestamp
     *
     * \return  Event change log item's timestamp
     */
    QDateTime timestamp() const;

    /*!
     * \brief   Sets event change log item's new timestamp
     *
     * \param   newTimestamp    Event change log item's new timestamp
     */
    void setTimestamp(const QDateTime &newTimestamp);

    /*!
     * \brief   Gets event change log item's field name
     *
     * \return  Event change log item's field name
     */
    QString fieldName() const;

    /*!
     * \brief   Sets event change log item's new field name
     *
     * \param   newFieldName    Event change log item's new field name
     */
    void setFieldName(const QString &newFieldName);

    /*!
     * \brief   Gets event change log item's original value
     *
     * \return  Event change log item's original value
     */
    QVariant fromValue() const;

    /*!
     * \brief   Sets event change log item's new original value
     *
     * \param   newId   Event change log item's new original value
     */
    void setFromValue(const QVariant &newFromValue);

    /*!
     * \brief   Gets event change log item's new value
     *
     * \return  Event change log item's new value
     */
    QVariant toValue() const;

    /*!
     * \brief   Sets event change log item's new value
     *
     * \param   newId   Event change log item's new value
     */
    void setToValue(const QVariant &newToValue);

    /*!
     * \brief   Gets event change log item's user ID
     *
     * \return  Event change log item's user ID
     */
    qint64 userId() const;

    /*!
     * \brief   Sets event change log item's new user ID
     *
     * \param   newId   Event change log item's new user ID
     */
    void setUserId(const qint64 &newUserId);

    /*!
     * \brief   Gets event change log item's comment
     *
     * \return  Event change log item's comment
     */
    QString comment() const;

    /*!
     * \brief   Sets event change log item's new comment
     *
     * \param   newId   Event change log item's new comment
     */
    void setComment(const QString &newComment);

    /*!
     * \brief   Creates an object from a map
     *
     * \param   map     Map that contains the object's values
     *
     * \return  A new object
     *
     * \note    Created object is invalid if the values in the map cannot be used to create a valid
     *          object.
     */
    static EventChangeLogItem fromMap(const QMap<QString, QVariant> &map);

private:
    /*!
     * \brief   Holds the event change log item's ID
     */
    qint64 m_id;

    /*!
     * \brief   Holds the event change log item's event ID
     */
    qint64 m_eventId;

    /*!
     * \brief   Holds the event change log item's timestamp
     *
     * \note    Timestamp is in UTC!
     */
    QDateTime m_timestamp;

    /*!
     * \brief   Holds the event change log item's field name
     */
    QString m_fieldName;

    /*!
     * \brief   Holds the event change log item's original value
     */
    QVariant m_fromValue;

    /*!
     * \brief   Holds the event change log item's new value
     */
    QVariant m_toValue;

    /*!
     * \brief   Holds the event change log item's user ID
     */
    qint64 m_userId;

    /*!
     * \brief   Holds the event change log item's comment
     */
    QString m_comment;
};

}
}

#endif // OPENTIMETRACKER_SERVER_EVENTCHANGELOGITEM_HPP
