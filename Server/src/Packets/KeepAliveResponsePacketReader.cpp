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
#include "KeepAliveResponsePacketReader.hpp"

using namespace OpenTimeTracker::Server::Packets;

KeepAliveResponsePacketReader::KeepAliveResponsePacketReader()
    : PacketReader()
{
}

KeepAliveResponsePacketReader::~KeepAliveResponsePacketReader()
{
}

QString KeepAliveResponsePacketReader::packetType() const
{
    return KeepAliveResponsePacket::staticType();
}

Packet *KeepAliveResponsePacketReader::createPacket() const
{
    return new KeepAliveResponsePacket();
}

bool KeepAliveResponsePacketReader::readBody(const QJsonObject &packetObject, Packet *packet) const
{
    bool success = false;

    // Check input parameters
    if ((!packetObject.isEmpty()) && (packet != nullptr))
    {
        success = true;
    }

    // Downcast packet pointer to the derived class
    KeepAliveResponsePacket *responsePacket = nullptr;

    if (success)
    {
        responsePacket = dynamic_cast<KeepAliveResponsePacket *>(packet);

        if (responsePacket == nullptr)
        {
            success = false;
        }
    }

    // Read reference ID
    if (success)
    {
        success = false;

        if (packetObject.contains("refId"))
        {
            const QJsonValue value = packetObject["refId"];

            if (value.isDouble())
            {
                const qint64 refIdValue = qRound64(value.toDouble(-1.0));

                if ((0 <= refIdValue) && (refIdValue <= UINT32_MAX))
                {
                    const quint32 refId = static_cast<quint32>(refIdValue);
                    responsePacket->setReferenceId(refId);
                    success = true;
                }
            }
        }
    }

    return success;
}
