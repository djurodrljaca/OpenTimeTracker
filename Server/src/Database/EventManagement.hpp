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
#ifndef OPENTIMETRACKER_SERVER_DATABASE_EVENTMANAGEMENT_HPP
#define OPENTIMETRACKER_SERVER_DATABASE_EVENTMANAGEMENT_HPP

#include "../Event.hpp"
#include "../EventChangeLogItem.hpp"

namespace OpenTimeTracker
{
namespace Server
{
namespace Database
{

/*!
 * \brief   Manages access to events in the database
 */
class EventManagement
{
public:
    /*!
     * \brief   Reads an specific event from the database
     *
     * \param   eventId     Event's ID
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static Event readEvent(const qint64 &eventId);

    /*!
     * \brief   Reads events from the database for a specific time range and user
     *
     * \param   startTimestamp  Read events from and including this timestamp
     * \param   endTimestamp    Read events up to and including this timestamp
     * \param   userId          Read events for the selected user
     *
     * \return  List of events
     */
    static QList<Event> readEvents(const QDateTime &startTimestamp,
                                   const QDateTime &endTimestamp,
                                   const qint64 &userId);

    /*!
     * \brief   Reads event change log from the database for a specific event
     *
     * \param   eventId     Read event change log for the selected event
     *
     * \return  List of event change log items
     */
    static QList<EventChangeLogItem> readEventChangeLog(const qint64 &eventId);

    /*!
     * \brief   Adds a new event to the database
     *
     * \param   timestamp   Event's timestamp
     * \param   userId      Event's user ID
     * \param   type        Event's type
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    All new events are by default enabled
     */
    static bool addEvent(const QDateTime &timestamp, const qint64 &userId, const Event::Type type);

    /*!
     * \brief   Changes the timestamp in an event in the database
     *
     * \param   eventId         ID of the event to change
     * \param   newTimestamp    New timestamp
     * \param   userId          ID of the user that requested the change
     * \param   comment         Explanation why the change was requested
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Comment must not be empty
     */
    static bool changeEventTimestamp(const qint64 &eventId,
                                     const QDateTime &newTimestamp,
                                     const qint64 &userId,
                                     const QString &comment);

    /*!
     * \brief   Changes the type in an event in the database
     *
     * \param   eventId     ID of the event to change
     * \param   newType     New type
     * \param   userId      ID of the user that requested the change
     * \param   comment     Explanation why the change was requested
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Comment must not be empty
     */
    static bool changeEventType(const qint64 &eventId,
                                const Event::Type &newType,
                                const qint64 &userId,
                                const QString &comment);

    /*!
     * \brief   Changes the enable state in an event in the database
     *
     * \param   eventId         ID of the event to change
     * \param   newEnableState  New enable state
     * \param   userId          ID of the user that requested the change
     * \param   comment         Explanation why the change was requested
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Comment must not be empty
     */
    static bool changeEventEnableState(const qint64 &eventId,
                                       const bool newEnableState,
                                       const qint64 &userId,
                                       const QString &comment);

private:
    /*!
     * \brief   Constructor is disabled
     */
    EventManagement();
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_DATABASE_EVENTMANAGEMENT_HPP
