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
#include "../../src/Database/DatabaseManagement.hpp"
#include "../../src/Database/UserManagement.hpp"
#include "../../src/Packets/KeepAliveRequestPacket.hpp"
#include "../../src/Packets/KeepAliveRequestPacketReader.hpp"
#include "../../src/Packets/KeepAliveRequestPacketWriter.hpp"
#include "../../src/Packets/KeepAliveResponsePacket.hpp"
#include "../../src/Packets/KeepAliveResponsePacketReader.hpp"
#include "../../src/Packets/KeepAliveResponsePacketWriter.hpp"
#include "../../src/Server.hpp"

namespace Test
{
using namespace OpenTimeTracker::Server;

class Client : QObject
{
    Q_OBJECT

public:
    Client()
        : QObject(),
          m_socket(),
          m_packetHandler()
    {
        m_packetHandler.registerPacketReader(new Packets::KeepAliveRequestPacketReader());
        m_packetHandler.registerPacketWriter(new Packets::KeepAliveRequestPacketWriter());

        m_packetHandler.registerPacketReader(new Packets::KeepAliveResponsePacketReader());
        m_packetHandler.registerPacketWriter(new Packets::KeepAliveResponsePacketWriter());

        QObject::connect(&m_socket, SIGNAL(readyRead()), this, SLOT(readReceivedData()));
    }

    ~Client()
    {
    }

    bool connect(const qint16 port)
    {
        m_socket.connectToHost(QHostAddress::LocalHost, port);
        return m_socket.waitForConnected();
    }

    bool disconnect()
    {
        m_socket.disconnectFromHost();
        return m_socket.waitForDisconnected();
    }

    bool sendPacket(const Packets::Packet &packet)
    {
        bool success = false;

        const QByteArray data = m_packetHandler.toByteArray(packet);
        const qint64 bytesWritten = m_socket.write(data);

        if (bytesWritten == data.size())
        {
            success = m_socket.flush();
        }

        return success;
    }

    Packets::Packet *readPacket()
    {
        Packets::Packet *packet = nullptr;
        QElapsedTimer timer;
        timer.start();
        bool finished = false;

        do
        {
            switch (m_packetHandler.read())
            {
                case PacketHandler::Result_Success:
                {
                    packet = m_packetHandler.takePacket();
                    finished = true;
                    break;
                }

                case PacketHandler::Result_NeedMoreData:
                {
                    if (timer.elapsed() < 10000)
                    {
                        QCoreApplication::processEvents();
                        QThread::msleep(100);
                    }
                    else
                    {
                        finished = true;
                    }
                    break;
                }

                default:
                {
                    // Error
                    finished = true;
                    break;
                }
            }
        }
        while (!finished);

        return packet;
    }

private slots:
    void readReceivedData()
    {
        m_packetHandler.addData(m_socket.readAll());
    }

private:
    QTcpSocket m_socket;
    PacketHandler m_packetHandler;
};
}

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

// Client unit tests *******************************************************************************

void ServerTest::testCaseClientConnect()
{
    using namespace OpenTimeTracker::Server;

    // Start server
    Server server;

    QVERIFY(server.start(m_port));
    QVERIFY(server.isStarted());
    QCoreApplication::processEvents();

    // Create test client and connect to the server
    Test::Client client;

    QVERIFY(client.connect(m_port));
    QCoreApplication::processEvents();

    // Send a "keep alive" request and wait for its response
    Packets::KeepAliveRequestPacket requestPacket;
    requestPacket.setId(PacketHandler::createPacketId());

    QVERIFY(client.sendPacket(requestPacket));

    // Wait for response packet
    Packets::Packet *responsePacket = client.readPacket();

    QVERIFY(responsePacket != nullptr);

    // Check reference ID
    Packets::KeepAliveResponsePacket *derivedPacket =
            dynamic_cast<Packets::KeepAliveResponsePacket *>(responsePacket);

    QVERIFY(derivedPacket != nullptr);
    QCOMPARE(derivedPacket->referenceId(), requestPacket.id());
}

// *************************************************************************************************

QTEST_MAIN(ServerTest)

#include "tst_ServerTest.moc"
