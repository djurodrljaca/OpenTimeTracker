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
#ifndef OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVERESPONSEPACKETWRITER_HPP
#define OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVERESPONSEPACKETWRITER_HPP

#include "PacketWriter.hpp"

namespace OpenTimeTracker
{
namespace Server
{
namespace Packets
{

/*!
 * \brief   Packet writer for Keep Alive Response packet
 */
class KeepAliveResponsePacketWriter : public PacketWriter
{
public:
    /*!
     * \brief   Constructor
     */
    KeepAliveResponsePacketWriter();

    /*!
     * \brief   Destructor
     */
    virtual ~KeepAliveResponsePacketWriter();

    /*!
     * \brief   Gets packet type that can be read by this class
     *
     * \return  Packet type
     */
    virtual QString packetType() const;

private:
    /*!
     * \copydoc PacketWriter::writeBody
     */
    virtual bool writeBody(const Packet &packet, QJsonObject &packetObject) const;
};

}
}
}


#endif // OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVERESPONSEPACKETWRITER_HPP
