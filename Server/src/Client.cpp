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
#include "Client.hpp"

using namespace OpenTimeTracker::Server;

Client::Client(QObject *parent, QTcpSocket *socket)
    : QObject(parent),
      m_socket(socket),
      m_packetParser()
{
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(handleDisconnect()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(processReceivedData()));
}

void Client::handleDisconnect()
{
    // Notify that the client's socket was disconnected
    emit disconnected(this);
}

void Client::processReceivedData()
{
    // Read all available data and add it to the parser
    m_packetParser.addData(m_socket->readAll());

    // Extract packets with the packet parser
    PacketParser::Result result = PacketParser::Result_Error;

    do
    {
        // Parse received data
        result = m_packetParser.parse();

        switch (result)
        {
            case PacketParser::Result_Success:
            {
                // Process received packet
                const bool success = processReceivedPacket(m_packetParser.takePacketPayload());

                if (!success)
                {
                    // Error, processing of the payload failed
                    // Terminate the connection
                    m_socket->close();
                }
                break;
            }

            case PacketParser::Result_NeedMoreData:
            {
                // More data is needed
                break;
            }

            case PacketParser::Result_Error:
            default:
            {
                // Error occurred, terminate the connection
                m_socket->close();
                break;
            }
        }
    }
    // Continue parsing until either an error occurs or more data is needed
    while (result == PacketParser::Result_Success);
}

bool Client::sendPacket(const QString &packetPayload)
{
    bool success = false;

    if (m_socket->isOpen())
    {
        // Write packet
        const QByteArray rawPacketData = PacketParser::toRawPacketData(packetPayload);
        const qint64 bytesWritten = m_socket->write(rawPacketData);

        if (bytesWritten == rawPacketData.size())
        {
            success = true;
        }
    }

    return success;
}

bool Client::processReceivedPacket(const QString &packetPayload)
{
    bool success = false;

    // TODO: implement!

    return success;
}
