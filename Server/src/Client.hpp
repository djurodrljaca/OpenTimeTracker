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
#ifndef OPENTIMETRACKER_SERVER_CLIENT_HPP
#define OPENTIMETRACKER_SERVER_CLIENT_HPP

#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include "PacketHandler.hpp"

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Client object for communication with the Open Time Tracker Client
 */
class Client : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief   Constructor
     *
     * \param   parent  Pointer to the parent object
     * \param   socket  Pointer to the socket object
     */
    explicit Client(QObject *parent, QTcpSocket *socket);

signals:
    /*!
     * \brief   Notification that the client's socket was disconnected
     *
     * \param   client  Pointer to the instance of the client object that was disconnected
     */
    void disconnected(Client *client);

private slots:
    /*!
     * \brief   Handles the situation when the socket is disconnected
     *
     * It just emits the Client::disconnected() signal
     */
    void handleDisconnect();

    /*!
     * \brief   Processes received data
     */
    void processReceivedData();

private:
    /*!
     * \brief   Sends packet to the server
     *
     * \param   packetPayload   Packet payload
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool sendPacket(const QString &packetPayload);

    /*!
     * \brief   Processes received packet
     *
     * \param   packetPayload   Packet payload
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool processReceivedPacket(const QString &packetPayload);

    /*!
     * \brief   Holds the TCP socket
     */
    QTcpSocket *m_socket;

    /*!
     * \brief   Holds the packet handler
     */
    PacketHandler m_packetHandler;
};

}
}

#endif // OPENTIMETRACKER_SERVER_CLIENT_HPP
