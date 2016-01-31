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
#include <QtCore/QJsonObject>
#include "PacketHandler.hpp"

using namespace OpenTimeTracker::Server;

PacketHandler::PacketHandler()
    : m_dataBuffer(),
      m_readPacket(nullptr)
{
}

void PacketHandler::addData(const QByteArray &data)
{
    m_dataBuffer.append(data);
}

PacketHandler::Result PacketHandler::read()
{
    Result result = Result_Error;

    // Make sure a valid packet was received (format: <STX>[packet payload in UTF-8]<ETX>)

    // Check for STX
    bool stxFound = false;

    if (m_dataBuffer.isEmpty())
    {
        // More data is needed
        result = Result_NeedMoreData;
    }
    else
    {
        if (m_dataBuffer.at(0) == '\x02')
        {
            // STX found
            stxFound = true;
        }
        else
        {
            // Error, invalid format
            result = Result_Error;
        }
    }

    // Check for ETX
    if (stxFound)
    {
        if (m_dataBuffer.size() < 2LL)
        {
            // More data is needed
            result = Result_NeedMoreData;
        }
        else
        {
            const int etxIndex = m_dataBuffer.indexOf('\x03', 1);

            if (etxIndex < 0)
            {
                // More data is needed
                result = Result_NeedMoreData;
            }
            else
            {
                // ETX found, extract packet payload
                const QByteArray packetPayload = m_dataBuffer.mid(1, etxIndex - 1);
                m_dataBuffer.remove(0, etxIndex + 1);

                // Convert the packet payload to a packet object
                Packets::Packet *packet = fromPacketPayload(packetPayload);

                if (packet == nullptr)
                {
                    // Error, failed to convert the packet
                    result = Result_Error;
                }
                else
                {
                    // Store the read packet
                    m_readPacket.reset(packet);
                    packet = nullptr;
                    result = Result_Success;
                }
            }
        }
    }

    return result;
}

Packets::Packet *PacketHandler::takePacket()
{
    return m_readPacket.take();
}

QByteArray PacketHandler::toByteArray(const Packets::Packet &packet)
{
    // Prepare packet (format: <STX>[packet payload in UTF-8]<ETX>)
    QByteArray packet;
    packet.append('\x02');
    packet.append(packet.toUtf8());
    packet.append('\x03');

    return packet;
}

Packets::Packet *PacketHandler::fromPacketPayload(const QByteArray &packetPayload) const
{
    Packets::Packet *packet = nullptr;

    // Read JSON document
    QJsonDocument document = QJsonDocument::fromJson(packetPayload);

    if (document.isObject())
    {
        QJsonObject object = document.object();

        // Read packet type
        QString type;

        if (object.contains("type"))
        {
            const QJsonValue value = object["type"];

            if (value.isString())
            {
                type = value.toString();
            }
        }

        // TODO: implement
    }



    // TODO: implement
    return packet;
}
