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
#include "PacketReader.hpp"

using namespace OpenTimeTracker::Server::Packets;

PacketReader::PacketReader()
{
}

PacketReader::~PacketReader()
{
}

Packet *PacketReader::fromPacketObject(const QJsonObject &packetObject) const
{
    Packet *packet = nullptr;
    bool success = false;

    // Check input parameters
    if (!packetObject.isEmpty())
    {
        success = true;
    }

    // Create packet object
    if (success)
    {
        success = false;
        packet = createPacket();

        if (packet != nullptr)
        {
            success = true;
        }
    }

    // Read header
    if (success)
    {
        success = readHeader(packetObject, packet);
    }

    // Read body
    if (success)
    {
        success = readBody(packetObject, packet);
    }

    // On error destroy the packet
    if (!success)
    {
        delete packet;
        packet = nullptr;
    }

    return packet;
}

bool PacketReader::readHeader(const QJsonObject &packetObject, Packet *packet) const
{
    bool success = false;

    // Check input parameters
    if ((!packetObject.isEmpty()) && (packet != nullptr))
    {
        success = true;
    }

    // Make sure this packet type is supported
    if (success)
    {
        success = false;

        if (packetObject.contains("type"))
        {
            const QJsonValue value = packetObject["type"];

            if (value.isString())
            {
                QString type = value.toString();

                if (type == packet->type())
                {
                    success = true;
                }
            }
        }
    }

    // Read packet ID
    if (success)
    {
        success = false;

        if (packetObject.contains("id"))
        {
            const QJsonValue value = packetObject["id"];

            if (value.isDouble())
            {
                const qint64 idValue = qRound64(value.toDouble(-1.0));

                if ((0 <= idValue) && (idValue <= UINT32_MAX))
                {
                    const quint32 id = static_cast<quint32>(idValue);
                    packet->setId(id);
                    success = true;
                }
            }
        }
    }

    return success;
}
