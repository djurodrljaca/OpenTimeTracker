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
#ifndef OPENTIMETRACKER_SERVER_SERVER_HPP
#define OPENTIMETRACKER_SERVER_SERVER_HPP

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QScopedPointer>
#include <QtNetwork/QTcpServer>
#include "Client.hpp"
#include "TimeTracker.hpp"
#include "User.hpp"

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Central object for managing time tracking of all users
 */
class Server : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief   Constructor
     *
     * \param   parent  Pointer to the parent object
     */
    explicit Server(QObject *parent = 0);

    /*!
     * \brief   Destructor
     */
    ~Server();

    /*!
     * \brief   Checks if the server is started
     *
     * \retval  true    Server is started
     * \retval  false   Server is not started
     */
    bool isStarted() const;

    /*!
     * \brief   Starts the server
     *
     * \param   port    TCP port on which the TCP server will be listening
     *
     * \retval  true    Server was started
     * \retval  false   Server was not started
     */
    bool start(const quint16 port);

    /*!
     * \brief   Stops the server
     */
    void stop();

private slots:
    /*!
     * \brief   Adds a new client to the client list
     */
    void addNewClient();

    /*!
     * \brief   Removes the specified client from the client list
     *
     * \param   client  Client that needs to be removed
     */
    void removeClient(Client *client);

private:
    /*!
     * \brief   Removes all clients from the server
     */
    void removeAllClients();

    /*!
     * \brief   Reads the user list from the database
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * Clears the user list and reads all users from the database
     */
    void readUsers();

    /*!
     * \brief   Initializes time trackers
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * Clears the time tracker list and creates a time tracker for each user
     */
    void initializeTimeTrackers();

    /*!
     * \brief   Holds the TCP server object
     */
    QTcpServer *m_tcpServer;

    /*!
     * \brief   Holds the client list
     */
    QList<Client *> m_clients;

    /*!
     * \brief   Holds user list
     */
    QList<User> m_users;

    /*!
     * \brief   Holds time trackers for all users in the database
     */
    QList<TimeTracker> m_timeTrackers;
};

}
}

#endif // OPENTIMETRACKER_SERVER_SERVER_HPP
