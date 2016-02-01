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
#ifndef OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVEREQUESTPACKET_HPP
#define OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVEREQUESTPACKET_HPP

#include "Packet.hpp"

namespace OpenTimeTracker
{
namespace Server
{
namespace Packets
{

/*!
 * \brief   Packet: Keep Alive Request
 */
class KeepAliveRequestPacket : public Packet
{
public:
    /*!
     * \brief   Constructor
     */
    KeepAliveRequestPacket();

    /*!
     * \brief   Destructor
     */
    virtual ~KeepAliveRequestPacket();

    /*!
     * \copydoc Packet::type
     */
    virtual QString type() const;

    /*!
     * \copydoc Packet::type
     */
    static QString staticType();
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVEREQUESTPACKET_HPP
