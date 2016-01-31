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

quint32 PacketHandler::m_nextPacketId = 0U;

PacketHandler::PacketHandler()
    : m_dataBuffer(),
      m_readPacket(nullptr),
      m_registeredPacketReaders(),
      m_registeredPacketWriters()
{
}

PacketHandler::~PacketHandler()
{
    // Remove all registered readers
    qDeleteAll(m_registeredPacketReaders);
    m_registeredPacketReaders.clear();

    // Remove all registered writers
    qDeleteAll(m_registeredPacketWriters);
    m_registeredPacketWriters.clear();
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
    // Prepare packet
    QByteArray packetData;

    // Find a matching packet writer and write the packet
    const QString type = packet.type();

    if (!type.isEmpty())
    {
        foreach (Packets::PacketWriter *registeredWriter, m_registeredPacketWriters)
        {
            if (registeredWriter->packetType() == type)
            {
                // Matching writer found, write the packet
                packetData = registeredWriter->toByteArray(packet);
                break;
            }
        }
    }
    return packetData;
}

bool PacketHandler::registerPacketReader(Packets::PacketReader *reader)
{
    bool success = false;

    // Check input parameters
    if (reader != nullptr)
    {
        if (!reader->packetType().isEmpty())
        {
            success = true;
        }
    }

    // Check if a reader for the same packet type is already registered
    if (success)
    {
        foreach (Packets::PacketReader *registeredReader, m_registeredPacketReaders)
        {
            if (reader->packetType() == registeredReader->packetType())
            {
                // Error, reader for the same packet type is already registered
                success = false;
                break;
            }
        }
    }

    // Register reader
    if (success)
    {
        m_registeredPacketReaders.append(reader);
    }

    return success;
}

bool PacketHandler::registerPacketWriter(Packets::PacketWriter *writer)
{
    bool success = false;

    // Check input parameters
    if (writer != nullptr)
    {
        if (!writer->packetType().isEmpty())
        {
            success = true;
        }
    }

    // Check if a writer for the same packet type is already registered
    if (success)
    {
        foreach (Packets::PacketWriter *registeredWriter, m_registeredPacketWriters)
        {
            if (writer->packetType() == registeredWriter->packetType())
            {
                // Error, writer for the same packet type is already registered
                success = false;
                break;
            }
        }
    }

    // Register writer
    if (success)
    {
        m_registeredPacketWriters.append(writer);
    }

    return success;
}

quint32 PacketHandler::createPacketId()
{
    return m_nextPacketId++;
}

Packets::Packet *PacketHandler::fromPacketPayload(const QByteArray &packetPayload) const
{
    Packets::Packet *packet = nullptr;

    // Read JSON document
    QJsonDocument document = QJsonDocument::fromJson(packetPayload);

    if (document.isObject())
    {
        QJsonObject packetObject = document.object();

        // Read packet type
        QString type;

        if (packetObject.contains("type"))
        {
            const QJsonValue value = packetObject["type"];

            if (value.isString())
            {
                type = value.toString();
            }
        }

        // Find a matching packet reader and read the packet
        if (!type.isEmpty())
        {
            foreach (Packets::PacketReader *registeredReader, m_registeredPacketReaders)
            {
                if (registeredReader->packetType() == type)
                {
                    // Matching reader found, read the packet
                    packet = registeredReader->fromPacketObject(packetObject);
                    break;
                }
            }
        }
    }

    return packet;
}
