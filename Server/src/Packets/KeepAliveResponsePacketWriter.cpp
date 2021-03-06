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
#include "KeepAliveResponsePacket.hpp"
#include "KeepAliveResponsePacketWriter.hpp"

using namespace OpenTimeTracker::Server::Packets;

KeepAliveResponsePacketWriter::KeepAliveResponsePacketWriter()
    : PacketWriter()
{
}

KeepAliveResponsePacketWriter::~KeepAliveResponsePacketWriter()
{
}

QString KeepAliveResponsePacketWriter::packetType() const
{
    return KeepAliveResponsePacket::staticType();
}

bool KeepAliveResponsePacketWriter::writeBody(const Packet &packet, QJsonObject &packetObject) const
{
    bool success = false;

    // Downcast to the derived class
    const KeepAliveResponsePacket *responsePacket =
            dynamic_cast<const KeepAliveResponsePacket *>(&packet);

    if (responsePacket != nullptr)
    {
        // Write reference ID
        packetObject["refId"] = static_cast<double>(responsePacket->referenceId());
        success = true;
    }

    return success;
}
