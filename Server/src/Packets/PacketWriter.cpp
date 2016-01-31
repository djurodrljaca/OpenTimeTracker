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
#include <QtCore/QJsonDocument>
#include "PacketWriter.hpp"

using namespace OpenTimeTracker::Server::Packets;

PacketWriter::PacketWriter()
{
}

PacketWriter::~PacketWriter()
{
}

QByteArray PacketWriter::toByteArray(const Packet &packet) const
{
    bool success = false;
    QJsonObject packetObject;

    // Write packet header
    success = writeHeader(packet, packetObject);

    // Write packet body
    if (success)
    {
        success = writeBody(packet, packetObject);
    }

    // Create packet data
    QByteArray packetData;

    if (success)
    {
        packetData.append('\x02');
        packetData.append(QJsonDocument(packetObject).toJson());
        packetData.append('\x03');
    }

    return packetData;
}

bool PacketWriter::writeHeader(const Packet &packet, QJsonObject &packetObject) const
{
    bool success = false;

    // Check input parameters
    if (packet.type() == packetType())
    {
        success = true;
    }

    // Write packet type and ID
    if (success)
    {
        packetObject["type"] = packet.type();
        packetObject["id"] = static_cast<double>(packet.id());
    }

    return success;
}
