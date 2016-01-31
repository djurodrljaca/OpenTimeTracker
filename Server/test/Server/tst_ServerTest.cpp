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
#include <QtCore/QString>
#include <QtTest>
#include "../../src/Server.hpp"
#include "../../src/Database/DatabaseManagement.hpp"
#include "../../src/Database/UserManagement.hpp"

class ServerTest : public QObject
{
    Q_OBJECT

public:
    ServerTest();

private Q_SLOTS:
    // Initialization and cleanup
    void initTestCase();
    void cleanupTestCase();

    // Start/stop unit tests
    void testCaseStartStop();
    void testCaseStartFail();

    // Client unit tests
    void testCaseClientConnect();

private:
    void removeDatabaseFile();

    const QString m_databaseFilePath;
    const qint16 m_port;
};

ServerTest::ServerTest()
    : m_databaseFilePath("test.db"),
      m_port(61234U)
{
}

void ServerTest::removeDatabaseFile()
{
    using namespace OpenTimeTracker::Server::Database;

    if (DatabaseManagement::isConnected())
    {
        DatabaseManagement::disconnect();
    }

    if (QFile::exists(m_databaseFilePath))
    {
        QVERIFY2(QFile::remove(m_databaseFilePath), "Database not removed");
    }
}

// Initialization and cleanup **********************************************************************

void ServerTest::initTestCase()
{
    using namespace OpenTimeTracker::Server;

    // First make sure old database file is removed
    removeDatabaseFile();

    // Create the test database
    Database::DatabaseManagement::connect("test.db");

    // Create users
    Database::UserManagement::addUser("user1", "111");
    Database::UserManagement::addUser("user2", "222");
    Database::UserManagement::addUser("user3", "222");
}

void ServerTest::cleanupTestCase()
{
    // Remove the database file
    removeDatabaseFile();
}

// Start/stop unit tests ***************************************************************************

void ServerTest::testCaseStartStop()
{
    using namespace OpenTimeTracker::Server;

    // Start server
    Server server;

    QVERIFY(server.start(m_port));
    QVERIFY(server.isStarted());

    // Stop server
    server.stop();
    QVERIFY(!server.isStarted());
}

void ServerTest::testCaseStartFail()
{
    using namespace OpenTimeTracker::Server;

    // Occupy the TCP port
    QTcpServer tcpServer;
    QVERIFY(tcpServer.listen(QHostAddress::Any, m_port));

    // Start server
    Server server;

    QVERIFY(!server.start(m_port));
    QVERIFY(!server.isStarted());
}

// Client unit tests *******************************************************************************

void ServerTest::testCaseClientConnect()
{
    using namespace OpenTimeTracker::Server;

    // Start server
    Server server;

    QVERIFY(server.start(m_port));
    QVERIFY(server.isStarted());

    // TODO: make a socket and connect? make a test client?
}

// *************************************************************************************************

QTEST_APPLESS_MAIN(ServerTest)

#include "tst_ServerTest.moc"
