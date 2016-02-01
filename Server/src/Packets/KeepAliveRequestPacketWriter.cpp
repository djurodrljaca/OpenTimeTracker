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
#include "KeepAliveRequestPacket.hpp"
#include "KeepAliveRequestPacketWriter.hpp"

using namespace OpenTimeTracker::Server::Packets;

KeepAliveRequestPacketWriter::KeepAliveRequestPacketWriter()
    : PacketWriter()
{
}

KeepAliveRequestPacketWriter::~KeepAliveRequestPacketWriter()
{
}

QString KeepAliveRequestPacketWriter::packetType() const
{
    return KeepAliveRequestPacket::staticType();
}

bool KeepAliveRequestPacketWriter::writeBody(const Packet &packet, QJsonObject &packetObject) const
{
    Q_UNUSED(packet);
    Q_UNUSED(packetObject);

    // This packet has no body (additional data members)
    return true;
}
