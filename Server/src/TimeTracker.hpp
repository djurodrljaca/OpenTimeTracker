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
#ifndef OPENTIMETRACKER_SERVER_TIMETRACKER_HPP
#define OPENTIMETRACKER_SERVER_TIMETRACKER_HPP

#include <QDateTime>
#include <QList>
#include "Schedule.hpp"
#include "BreakTimeCalculator.hpp"

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Tracks working time of the selected user
 */
class TimeTracker
{
public:
    /*!
     * \brief   Defines the state of the user being tracked
     */
    enum State
    {
        State_NotWorking = 0,   /*!< Not working */
        State_Working,          /*!< Working */
        State_OnBreak           /*!< On break */
    };

    /*!
     * \brief   Constructor
     */
    TimeTracker();

    /*!
     * \brief   Copy constructor
     * \param   other   Object to be copied
     */
    TimeTracker(const TimeTracker &other);

    /*!
     * \brief   operator =
     * \param   other   Object to be copied
     *
     * \return  Reference to the this object
     */
    TimeTracker &operator =(const TimeTracker &other);

    /*!
     * \brief   Checks if object is valid
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    bool isValid() const;

    /*!
     * \brief   Gets ID of the user being tracked
     *
     * \return  User ID
     */
    qint64 userId() const;

    /*!
     * \brief   Sets new user ID
     *
     * \param   newUserId   New user ID
     */
    void setUserId(const qint64 &newUserId);

    /*!
     * \brief   Calculates working time
     *
     * \param   timestamp   A point in time for which the working time should be calculated
     *
     * \return  Working time (in seconds)
     */
    qint32 calculateWorkingTime(const QDateTime &timestamp) const;

    /*!
     * \brief   Calculates break time
     *
     * \param   timestamp   A point in time for which the break time should be calculated
     *
     * \return  Break time (in seconds)
     */
    qint32 calculateBreakTime(const QDateTime &timestamp) const;

    /*!
     * \brief   Calculates total working time
     *
     * \param   timestamp   A point in time for which the total working time should be calculated
     *
     * \return  Total working time (in seconds)
     *
     * Total working time is a sum of the working time and (limited) break time.
     */
    qint32 calculateTotalWorkingTime(const QDateTime &timestamp) const;

    /*!
     * \brief   Gets current state of the user being tracked
     *
     * \return  State
     */
    State state() const;

    /*!
     * \brief   Starts the workday
     *
     * \param   breakTimeCalculator Break time calculator
     * \param   schedules           List of schedules that define when the working time shall be
     *                              tracked
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * This should be called at the start of work day or when the server is started.
     *
     * Examples:
     * - If each workday starts at midnight, then this should be called at midnight.
     *
     * - If server was started in the middle of the workday then this should be called and the
     *   events for this workday should be put through this object to get the current state of the
     *   user.
     *
     * \note    Working and break times, state and timestamp of the last state change are all
     *          (re)initialized when this method is called.
     *
     * \note    Only the working time (and allowed break time) within the schedules is tracked.
     */
    bool startWorkday(const BreakTimeCalculator &breakTimeCalculator,
                      const QList<Schedule> &schedules);

    /*!
     * \brief   Starts tracking users working time
     *
     * \param   timestamp   The exact date and time when the user started working
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    This is only allowed when the user is in "not working" state
     */
    bool startWorking(const QDateTime &timestamp);

    /*!
     * \brief   Changes the user's state from "working" to "on break"
     *
     * \param   timestamp   The exact date and time when the user started their break
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    This is only allowed when the user is in "working" state
     */
    bool startBreak(const QDateTime &timestamp);

    /*!
     * \brief   Changes the user's state from "on break" to "working"
     *
     * \param   timestamp   The exact date and time when the user ended their break
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    This is only allowed when the user is in "on break" state
     */
    bool endBreak(const QDateTime &timestamp);

    /*!
     * \brief   Stops tracking users working time
     *
     * \param   timestamp   The exact date and time when the user stopped working
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    This is only allowed when the user is in "working" state
     */
    bool stopWorking(const QDateTime &timestamp);

private:
    /*!
     * \brief   Calculates scheduled time for the specified time period
     *
     * \param   startTimestamp  Start timestamp
     * \param   endTimestamp    End timestamp
     *
     * \return  Scheduled time (in seconds)
     */
    qint32 calculateScheduledTime(const QDateTime &startTimestamp,
                                  const QDateTime &endTimestamp) const;

    /*!
     * \brief   Calculates scheduled time for the specified time periods
     *
     * \param   timePeriods Time periods
     * \param   timestamp   Timestamp up to and including to which the scheduled time needs to be
     *                      calculated
     *
     * \return  Scheduled time (in seconds)
     */
    qint32 calculateScheduledTime(const QList<QPair<QDateTime, QDateTime> > &timePeriods,
                                  const QDateTime &timestamp) const;

    /*!
     * \brief   Holds the ID of the user to track
     */
    qint64 m_userId;

    /*!
     * \brief   Holds the break time calculator
     */
    BreakTimeCalculator m_breakTimeCalculator;

    /*!
     * \brief   Holds the schedules that define when the working time shall be tracked
     */
    QList<Schedule> m_schedules;

    /*!
     * \brief   Holds the working periods
     */
    QList<QPair<QDateTime, QDateTime> > m_workingPeriods;

    /*!
     * \brief   Holds the break time periods
     */
    QList<QPair<QDateTime, QDateTime> > m_breakPeriods;

    /*!
     * \brief   Holds the current state of the tracked user
     */
    State m_state;

    /*!
     * \brief   Holds the timestamp of the last event
     */
    QDateTime m_lastEventTimestamp;

    // TODO: update and extend the unit tests!
};

}
}

#endif // OPENTIMETRACKER_SERVER_TIMETRACKER_HPP
