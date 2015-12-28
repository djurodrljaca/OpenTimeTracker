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
#include "BreakTimeCalculator.hpp"

using namespace OpenTimeTracker::Server;

BreakTimeCalculator::BreakTimeCalculator()
    : m_coefficient(0.0)
{
}

BreakTimeCalculator::BreakTimeCalculator(const BreakTimeCalculator &other)
    : m_coefficient(other.m_coefficient)
{
}

BreakTimeCalculator &BreakTimeCalculator::operator =(const BreakTimeCalculator &other)
{
    if (this != &other)
    {
        m_coefficient = other.m_coefficient;
    }

    return *this;
}

bool BreakTimeCalculator::isValid() const
{
    bool valid = true;

    if (m_coefficient < 0.0)
    {
        valid = false;
    }

    return valid;
}

bool BreakTimeCalculator::initialize(const double &workingTime, const double &allowedBreakTime)
{
    bool success = false;

    if ((workingTime > 0.0) && (allowedBreakTime >= 0.0))
    {
        m_coefficient = allowedBreakTime / workingTime;
    }

    return success;
}

qint32 BreakTimeCalculator::calculate(const qint32 workingTime, const qint32 breakTime) const
{
    qint32 allowedBreakTime = 0;

    if (isValid())
    {
        if ((workingTime >= 0.0) && (breakTime >= 0.0))
        {
            const double limit = workingTime * m_coefficient;

            if (breakTime > limit)
            {
                allowedBreakTime = static_cast<qint32>(round(limit));
            }
            else
            {
                allowedBreakTime = breakTime;
            }
        }
    }

    return allowedBreakTime;
}
