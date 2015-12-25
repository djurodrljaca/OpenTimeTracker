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
     * \brief   Gets working time
     *
     * \return  Working time (in seconds)
     */
    qint32 workingTime() const;

    /*!
     * \brief   Gets break time
     *
     * \return  Break time (in seconds)
     */
    qint32 breakTime() const;

    /*!
     * \brief   Gets current state of the user being tracked
     *
     * \return  State
     */
    State state() const;

    /*!
     * \brief   Clears the internal state
     *
     * This should be called at the start of work day.
     *
     * For example if each work day starts at midnight, then this should be called at midnight.
     */
    void clear();

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
     * \brief   Holds the ID of the user to track
     */
    qint64 m_userId;

    /*!
     * \brief   Holds the current working time (in seconds)
     */
    qint32 m_workingTime;

    /*!
     * \brief   Holds the current break time (in seconds)
     */
    qint32 m_breakTime;

    /*!
     * \brief   Holds the current state of the tracked user
     */
    State m_state;

    /*!
     * \brief   Holds the timestamp of the last state change
     */
    QDateTime m_stateChangedTimestamp;

    // TODO: add work schedule?
};

}
}

#endif // OPENTIMETRACKER_SERVER_TIMETRACKER_HPP
