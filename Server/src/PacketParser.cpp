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
#include "PacketParser.hpp"

using namespace OpenTimeTracker::Server;

PacketParser::PacketParser()
    : m_packetDataBuffer(),
      m_parsedPacketPayload()
{
}

void PacketParser::clear()
{
    m_packetDataBuffer.clear();
    m_parsedPacketPayload.clear();
}

void PacketParser::addData(const QByteArray &data)
{
    m_packetDataBuffer.append(data);
}

PacketParser::Result PacketParser::parse()
{
    Result result = Result_Error;

    // Make sure a valid packet was received (format: <STX>[packet payload in UTF-8]<ETX>)

    // Check for STX
    bool stxFound = false;

    if (m_packetDataBuffer.isEmpty())
    {
        // More data is needed
        result = Result_NeedMoreData;
    }
    else
    {
        if (m_packetDataBuffer.at(0) == '\x02')
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
        if (m_packetDataBuffer.size() < 2LL)
        {
            // More data is needed
            result = Result_NeedMoreData;
        }
        else
        {
            const int etxIndex = m_packetDataBuffer.indexOf('\x03', 1);

            if (etxIndex < 0)
            {
                // More data is needed
                result = Result_NeedMoreData;
            }
            else
            {
                // ETX found, extract packet data
                const QByteArray data = m_packetDataBuffer.mid(1, etxIndex - 1);

                // Save packet payload
                m_parsedPacketPayload = QString::fromUtf8(data);

                // Remove processed packet
                m_packetDataBuffer.remove(0, etxIndex + 1);

                // Parsing was successful
                result = Result_Success;
            }
        }
    }

    return result;
}

QString PacketParser::takePacketPayload()
{
    QString payload = m_parsedPacketPayload;
    m_parsedPacketPayload.clear();

    return payload;
}

QByteArray PacketParser::toRawPacketData(const QString &packetPayload)
{
    // Prepare packet (format: <STX>[packet payload in UTF-8]<ETX>)
    QByteArray packet;
    packet.append('\x02');
    packet.append(packetPayload.toUtf8());
    packet.append('\x03');

    return packet;
}
