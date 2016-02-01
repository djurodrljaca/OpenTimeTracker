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
#ifndef OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVERESPONSEPACKETREADER_HPP
#define OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVERESPONSEPACKETREADER_HPP

#include "PacketReader.hpp"

namespace OpenTimeTracker
{
namespace Server
{
namespace Packets
{

/*!
 * \brief   Abstraction of a packet reader
 */
class KeepAliveResponsePacketReader : public PacketReader
{
public:
    /*!
     * \brief   Constructor
     */
    KeepAliveResponsePacketReader();

    /*!
     * \brief   Destructor
     */
    virtual ~KeepAliveResponsePacketReader();

    /*!
     * \brief   Gets packet type that can be read by this class
     *
     * \return  Packet type
     */
    virtual QString packetType() const;

private:
    /*!
     * \brief   Creates an empty packet object
     *
     * \return  Empty packet object
     */
    virtual Packet *createPacket() const;

    /*!
     * \brief   Reads packet body from the packet JSON object
     *
     * \param       packetObject    Packet JSON object
     * \param[out]  packet          Packet that whose body is being read
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    virtual bool readBody(const QJsonObject &packetObject, Packet *packet) const;
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVERESPONSEPACKETREADER_HPP
