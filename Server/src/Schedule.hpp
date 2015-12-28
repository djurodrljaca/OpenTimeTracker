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
#ifndef OPENTIMETRACKER_SERVER_SCHEDULE_HPP
#define OPENTIMETRACKER_SERVER_SCHEDULE_HPP

#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Holds schedule entries
 */
class Schedule
{
public:
    /*!
     * \brief   Constructor
     */
    Schedule();

    /*!
     * \brief   Copy constructor
     * \param   other   Object to be copied
     */
    Schedule(const Schedule &other);

    /*!
     * \brief   operator =
     * \param   other   Object to be copied
     *
     * \return  Reference to the this object
     */
    Schedule &operator =(const Schedule &other);

    /*!
     * \brief   Checks if object is valid
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    bool isValid() const;

    /*!
     * \brief   Gets schedule's ID
     *
     * \return  Schedule's ID
     */
    qint64 id() const;

    /*!
     * \brief   Sets schedule's new ID
     *
     * \param   newId   Schedule's new ID
     */
    void setId(const qint64 &newId);

    /*!
     * \brief   Gets schedule's user ID
     *
     * \return  Schedule's user ID
     */
    qint64 userId() const;

    /*!
     * \brief   Sets schedule's user ID
     *
     * \param   newUserId   New schedule's user ID
     */
    void setUserId(const qint64 &newUserId);

    /*!
     * \brief   Gets schedule's start timestamp
     *
     * \return  Schedule's start timestamp
     */
    QDateTime startTimestamp() const;

    /*!
     * \brief   Sets schedule's start timestamp
     *
     * \param   newStartTimestamp   New schedule's start timestamp
     */
    void setStartTimestamp(const QDateTime &newStartTimestamp);

    /*!
     * \brief   Gets schedule's end timestamp
     *
     * \return  Schedule's end timestamp
     */
    QDateTime endTimestamp() const;

    /*!
     * \brief   Sets schedule's end timestamp
     *
     * \param   newEndTimestamp     New schedule's end timestamp
     */
    void setEndTimestamp(const QDateTime &newEndTimestamp);

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
    static Schedule fromMap(const QMap<QString, QVariant> &map);

private:
    /*!
     * \brief   Holds the schedule's ID
     */
    qint64 m_id;

    /*!
     * \brief   Holds the schedule's user ID
     */
    qint64 m_userId;

    /*!
     * \brief   Holds the schedule's start timestamp
     */
    QDateTime m_startTimestamp;

    /*!
     * \brief   Holds the schedule's end timestamp
     */
    QDateTime m_endTimestamp;
};

}
}

#endif // OPENTIMETRACKER_SERVER_SCHEDULE_HPP
