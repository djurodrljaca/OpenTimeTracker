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
#ifndef OPENTIMETRACKER_SERVER_PACKETS_PACKET_HPP
#define OPENTIMETRACKER_SERVER_PACKETS_PACKET_HPP

#include <QtCore/QString>

namespace OpenTimeTracker
{
namespace Server
{
namespace Packets
{

/*!
 * \brief   Abstraction of a packet
 */
class Packet
{
public:
    /*!
     * \brief   Constructor
     */
    Packet();

    /*!
     * \brief   Destructor
     */
    virtual ~Packet() = 0;

    /*!
     * \brief   Gets packet type
     *
     * \return  Packet type
     */
    virtual QString type() const = 0;

    /*!
     * \brief   Gets packet ID
     *
     * \return  Packet ID
     */
    quint32 id() const;

    /*!
     * \brief   Sets packet ID
     *
     * \param   idValue     Packet ID
     */
    void setId(const quint32 idValue);

private:
    /*!
     * \brief   Holds the packet ID
     */
    quint32 m_id;
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_PACKETS_PACKET_HPP
