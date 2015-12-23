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
#ifndef OPENTIMETRACKER_SERVER_EVENT_HPP
#define OPENTIMETRACKER_SERVER_EVENT_HPP

#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Holds event entries
 */
class Event
{
public:
    enum Type
    {
        Type_Invalid = 0,
        Type_Started,
        Type_OnBreak,
        Type_FromBreak,
        Type_Finished
    };

    /*!
     * \brief   Constructor
     */
    Event();

    /*!
     * \brief   Copy constructor
     * \param   other   Object to be copied
     */
    Event(const Event &other);

    /*!
     * \brief   operator =
     * \param   other   Object to be copied
     *
     * \return  Reference to the this object
     */
    Event &operator =(const Event &other);

    /*!
     * \brief   Checks if object is valid
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    bool isValid() const;

    /*!
     * \brief   Gets event's ID
     *
     * \return  Event's ID
     */
    qint64 id() const;

    /*!
     * \brief   Sets event's new ID
     *
     * \param   newId   Event's new ID
     */
    void setId(const qint64 &newId);

    /*!
     * \brief   Gets event's timestamp
     *
     * \return  Event's timestamp
     */
    QDateTime timestamp() const;

    /*!
     * \brief   Sets event's timestamp
     *
     * \param   newTimestamp    New event's timestamp
     */
    void setTimestamp(const QDateTime &newTimestamp);

    /*!
     * \brief   Gets event's type
     *
     * \return  Event's type
     */
    Type type() const;

    /*!
     * \brief   Sets event's type
     *
     * \param   newType New event's type
     */
    void setType(const Type &newType);

    /*!
     * \brief   Gets event's enable state
     *
     * \return  Event's enable state
     */
    bool isEnabled() const;

    /*!
     * \brief   Sets event's enable state
     *
     * \param   enabled     New event's enable state
     */
    void setEnabled(bool enabled);

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
    static Event fromMap(const QMap<QString, QVariant> &map);

private:
    /*!
     * \brief   Holds the event's ID
     */
    qint64 m_id;

    /*!
     * \brief   Holds the event's timestamp
     *
     * \note    Timestamp is in UTC!
     */
    QDateTime m_timestamp;

    /*!
     * \brief   Holds the event's type
     */
    Type m_type;

    /*!
     * \brief   Holds the event's enable state
     */
    bool m_enabled;

};

}
}

#endif // OPENTIMETRACKER_SERVER_EVENT_HPP
