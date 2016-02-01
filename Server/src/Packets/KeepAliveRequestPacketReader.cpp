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
#include "KeepAliveRequestPacketReader.hpp"

using namespace OpenTimeTracker::Server::Packets;

KeepAliveRequestPacketReader::KeepAliveRequestPacketReader()
    : PacketReader()
{
}

KeepAliveRequestPacketReader::~KeepAliveRequestPacketReader()
{
}

QString KeepAliveRequestPacketReader::packetType() const
{
    return KeepAliveRequestPacket::staticType();
}

Packet *KeepAliveRequestPacketReader::createPacket() const
{
    return new KeepAliveRequestPacket();
}

bool KeepAliveRequestPacketReader::readBody(const QJsonObject &packetObject, Packet *packet) const
{
    Q_UNUSED(packetObject);
    Q_UNUSED(packet);

    // This packet has no body (additional data members)
    return true;
}
