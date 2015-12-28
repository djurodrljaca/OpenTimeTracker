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
#ifndef OPENTIMETRACKER_SERVER_BREAKTIMECALCULATOR_HPP
#define OPENTIMETRACKER_SERVER_BREAKTIMECALCULATOR_HPP

#include <QtCore/QDateTime>

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Calculates break time
 */
class BreakTimeCalculator
{
public:
    /*!
     * \brief   Constructor
     */
    BreakTimeCalculator();

    /*!
     * \brief   Copy constructor
     * \param   other   Object to be copied
     */
    BreakTimeCalculator(const BreakTimeCalculator &other);

    /*!
     * \brief   operator =
     * \param   other   Object to be copied
     *
     * \return  Reference to the this object
     */
    BreakTimeCalculator &operator =(const BreakTimeCalculator &other);

    /*!
     * \brief   Checks if object is valid
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    bool isValid() const;

    /*!
     * \brief   Initializes the break time calculator
     *
     * \param   workingTime         Working time (in hours)
     * \param   allowedBreakTime    Allowed break time for the specified working time (in hours)
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool initialize(const double &workingTime, const double &allowedBreakTime);

    /*!
     * \brief   Calculates allowed break time
     *
     * \param   workingTime Working time (in seconds)
     * \param   breakTime   Break time (in seconds)
     *
     * \return  Allowed break time
     */
    qint32 calculate(const qint32 workingTime, const qint32 breakTime) const;

private:
    /*!
     * \brief   Holds the coefficient for calculation of allowed break time
     */
    double m_coefficient;
};

}
}

#endif // OPENTIMETRACKER_SERVER_BREAKTIMECALCULATOR_HPP
