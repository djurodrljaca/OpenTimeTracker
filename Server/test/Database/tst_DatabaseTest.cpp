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
#include <QString>
#include <QtTest>
#include <QFile>
#include "../../src/Database.hpp"

class DatabaseTest : public QObject
{
    Q_OBJECT

public:
    DatabaseTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testCaseConnect();
    void testCaseDisconnect();
    void testCaseReconnect();

    void testCaseReadAllUsersEmptyDatabase();

    void testCaseAddUser_data();
    void testCaseAddUser();
    void testCaseAddUserFail_data();
    void testCaseAddUserFail();

    void testCaseReadAllUsersNonEmptyDatabase();

private:
    QString m_databaseFilePath;
};

DatabaseTest::DatabaseTest()
    : m_databaseFilePath("test.db")
{
}

void DatabaseTest::initTestCase()
{
    if (QFile::exists(m_databaseFilePath))
    {
        QVERIFY2(QFile::remove(m_databaseFilePath), "Database not removed");
    }
}

void DatabaseTest::cleanupTestCase()
{
    if (QFile::exists(m_databaseFilePath))
    {
        QVERIFY2(QFile::remove(m_databaseFilePath), "Database not removed");
    }
}

void DatabaseTest::testCaseConnect()
{
    OpenTimeTracker::Server::Database database;

    // Initially it has to be disconnected
    QCOMPARE(database.isConnected(), false);

    // Connecting should be successful
    QVERIFY(database.connect(m_databaseFilePath));

    // Check if really connected
    QCOMPARE(database.isConnected(), true);
}

void DatabaseTest::testCaseDisconnect()
{
    OpenTimeTracker::Server::Database database;

    // Initially it has to be disconnected
    QCOMPARE(database.isConnected(), false);

    // Connecting should be successful
    QVERIFY(database.connect(m_databaseFilePath));

    // Check if really connected
    QCOMPARE(database.isConnected(), true);

    // Disconnect
    database.disconnect();

    // Check if really disconnected
    QCOMPARE(database.isConnected(), false);
}

void DatabaseTest::testCaseReconnect()
{
    OpenTimeTracker::Server::Database database;

    // Initially it has to be disconnected
    QCOMPARE(database.isConnected(), false);

    // Connecting should be successful
    QVERIFY(database.connect(m_databaseFilePath));

    // Check if really connected
    QCOMPARE(database.isConnected(), true);

    // Disconnect
    database.disconnect();

    // Check if really disconnected
    QCOMPARE(database.isConnected(), false);

    // Reconnecting should be successful
    QVERIFY(database.connect(m_databaseFilePath));
}

void DatabaseTest::testCaseReadAllUsersEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::UserInfo> userList = database.readAllUsers();

    QVERIFY(userList.isEmpty());
}

void DatabaseTest::testCaseAddUser_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("password");

    QTest::newRow("1") << "user1" << "111";
    QTest::newRow("2") << "user2" << QString();
}

void DatabaseTest::testCaseAddUser()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Add user
    QFETCH(QString, name);
    QFETCH(QString, password);
    QVERIFY(database.addUser(name, password));
}

void DatabaseTest::testCaseAddUserFail_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("password");

    // Invalid name
    QTest::newRow("1") << QString() << "123";
    QTest::newRow("2") << "" << "234";

    // Invalid password
    QTest::newRow("3") << "user4" << "";

    // Existing password
    QTest::newRow("4") << "user6" << "111";
}

void DatabaseTest::testCaseAddUserFail()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Add user
    QFETCH(QString, name);
    QFETCH(QString, password);
    QVERIFY(!database.addUser(name, password));
}

void DatabaseTest::testCaseReadAllUsersNonEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::UserInfo> userList = database.readAllUsers();

    QCOMPARE(userList.size(), 2);

    // User 1
    QCOMPARE(userList[0].name(), QString("user1"));
    QCOMPARE(userList[0].password(), QString("111"));

    // User 2
    QCOMPARE(userList[1].name(), QString("user2"));
    QVERIFY(userList[1].password().isNull());
}

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_DatabaseTest.moc"
