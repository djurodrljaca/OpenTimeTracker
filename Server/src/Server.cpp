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
#include "Server.hpp"
#include "Database/DatabaseManagement.hpp"
#include "Database/UserManagement.hpp"

using namespace OpenTimeTracker::Server;

Server::Server(QObject *parent)
    : QObject(parent),
      m_tcpServer(nullptr),
      m_clients(),
      m_users(),
      m_timeTrackers()
{
    m_tcpServer = new QTcpServer(this);

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(addNewClient()));
}

Server::~Server()
{
    // Remove all clients
    removeAllClients();

    // Delete TCP server
    m_tcpServer->deleteLater();
}

bool Server::isStarted() const
{
    return m_tcpServer->isListening();
}

bool Server::start(const quint16 port)
{
    // Check if connected to database
    bool success = Database::DatabaseManagement::isConnected();

    // Make sure all clients are removed
    if (success)
    {
        removeAllClients();
    }

    // Read user information from the database
    if (success)
    {
        readUsers();
    }

    // Initialize time trackers
    if (success)
    {
        initializeTimeTrackers();
    }

    // Start the TCP server
    if (success)
    {
        success = m_tcpServer->listen(QHostAddress::Any, port);
    }

    return success;
}

void Server::stop()
{
    if (isStarted())
    {
        // Close the TCP server and remove all clients
        m_tcpServer->close();
        removeAllClients();
    }
}

void Server::addNewClient()
{
    // For each pending connection (TCP socket) create a client and add it to the client list
    while (m_tcpServer->hasPendingConnections() && m_tcpServer->isListening())
    {
        Client *client = new Client(this, m_tcpServer->nextPendingConnection());
        connect(client, SIGNAL(disconnected(Client*)), this, SLOT(removeClient(Client*)));
        m_clients.append(client);
        client = nullptr;
    }
}

void Server::removeClient(Client *client)
{
    // Take the client from the list, disconnect all signals that are connecting this class and the
    // client, and delete the client object
    m_clients.removeAll(client);
    client->disconnect(this);
    client->deleteLater();
}

void Server::removeAllClients()
{
    // Delete all clients and remove them from the list
    while (!m_clients.isEmpty())
    {
        // Take the client from the list, disconnect all signals that are connecting this class and
        // the client, and delete the client object
        Client *client = m_clients.takeFirst();
        client->disconnect(this);
        client->deleteLater();
    };
}

void Server::readUsers()
{
    m_users = Database::UserManagement::readUsers();
    //Database::UserManagement::readUserGroups();
    //Database::UserManagement::readUserMappings();
}

void Server::initializeTimeTrackers()
{
    m_timeTrackers.clear();

    foreach (const User &user, m_users)
    {
        TimeTracker timeTracker;
        timeTracker.setUserId(user.id());

        m_timeTrackers.append(timeTracker);
    }
}
