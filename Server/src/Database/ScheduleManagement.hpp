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
#ifndef OPENTIMETRACKER_SERVER_DATABASE_SCHEDULEMANAGEMENT_HPP
#define OPENTIMETRACKER_SERVER_DATABASE_SCHEDULEMANAGEMENT_HPP

#include <QDateTime>
#include <QPair>

namespace OpenTimeTracker
{
namespace Server
{
namespace Database
{

/*!
 * \brief   Manages access to the working days and schedules in the database
 */
class ScheduleManagement
{
public:
    /*!
     * \brief   Reads all working day time range
     *
     * \param   timestamp   Timestamp that is within the working day time range
     *
     * \return  A pair of timestamps:
     *          - first: start of working day
     *          - second: end of working day
     *
     * \note    The last added working day entry found in the database that satisfies the search
     *          parameter is returned.
     */
    static QPair<QDateTime, QDateTime> readWorkingDay(const QDateTime &timestamp);

    /*!
     * \brief   Adds a new working day to the database
     *
     * \param   startTimestamp  The exact date and time when working day shall start
     * \param   endTimestamp    The exact date and time when working day shall end
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * Example:
     * - Start of working day: 2015-12-27 00:00:00
     * - End of working day:   2015-12-27 23:59:59
     */
    static bool addWorkingDay(const QDateTime &startTimestamp, const QDateTime &endTimestamp);

private:
    /*!
     * \brief   Constructor is disabled
     */
    ScheduleManagement();
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_DATABASE_SCHEDULEMANAGEMENT_HPP
