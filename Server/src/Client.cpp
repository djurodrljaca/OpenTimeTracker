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
      m_packetHandler()
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
    m_packetHandler.addData(m_socket->readAll());

    // Extract packets with the packet parser
    PacketHandler::Result result = PacketHandler::Result_Error;

    do
    {
        // Parse received data
        result = m_packetHandler.read();

        switch (result)
        {
            case PacketHandler::Result_Success:
            {
                // Process received packet
                bool success = false;
                const QScopedPointer<Packets::Packet> packet(m_packetHandler.takePacket());

                if (!packet.isNull())
                {
                    success = processReceivedPacket(*packet);
                }

                if (!success)
                {
                    // Error, processing of the payload failed
                    // Terminate the connection
                    m_socket->close();
                }
                break;
            }

            case PacketHandler::Result_NeedMoreData:
            {
                // More data is needed
                break;
            }

            case PacketHandler::Result_Error:
            default:
            {
                // Error occurred, terminate the connection
                m_socket->close();
                break;
            }
        }
    }
    // Continue parsing until either an error occurs or more data is needed
    while (result == PacketHandler::Result_Success);
}

bool Client::sendPacket(const Packets::Packet &packet)
{
    bool success = false;

    if (m_socket->isOpen())
    {
        // Write packet
        const QByteArray packetData = m_packetHandler.toByteArray(packet);
        const qint64 bytesWritten = m_socket->write(packetData);

        if (bytesWritten == packetData.size())
        {
            success = true;
        }
    }

    return success;
}

bool Client::processReceivedPacket(const Packets::Packet &packet)
{
    bool success = false;

    // TODO: implement!

    return success;
}
