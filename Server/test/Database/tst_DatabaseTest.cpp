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

    void testCaseReadAllUserGroupsEmptyDatabase();
    void testCaseAddUserGroup_data();
    void testCaseAddUserGroup();
    void testCaseAddUserGroupFail_data();
    void testCaseAddUserGroupFail();
    void testCaseReadAllUserGroupsNonEmptyDatabase();

    void testCaseReadAllUserMappingsEmptyDatabase();
    void testCaseAddUserMapping_data();
    void testCaseAddUserMapping();
    void testCaseAddUserMappingFail_data();
    void testCaseAddUserMappingFail();
    void testCaseReadAllUserMappingsNonEmptyDatabase();

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

    QList<OpenTimeTracker::Server::UserInfo> users = database.readAllUsers();

    QVERIFY(users.isEmpty());
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

    // Existing name
    QTest::newRow("3") << "user1" << "333";

    // Invalid password
    QTest::newRow("4") << "user4" << "";

    // Existing password
    QTest::newRow("5") << "user6" << "111";
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

    QList<OpenTimeTracker::Server::UserInfo> users = database.readAllUsers();

    QCOMPARE(users.size(), 2);

    // User 1
    QCOMPARE(users[0].name(), QString("user1"));
    QCOMPARE(users[0].password(), QString("111"));

    // User 2
    QCOMPARE(users[1].name(), QString("user2"));
    QVERIFY(users[1].password().isNull());
}

void DatabaseTest::testCaseReadAllUserGroupsEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::UserGroupInfo> userGroups = database.readAllUserGroups();

    QVERIFY(userGroups.isEmpty());
}

void DatabaseTest::testCaseAddUserGroup_data()
{
    QTest::addColumn<QString>("name");

    QTest::newRow("1") << "userGroup1";
    QTest::newRow("2") << "userGroup2";
}

void DatabaseTest::testCaseAddUserGroup()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Add user group
    QFETCH(QString, name);
    QVERIFY(database.addUserGroup(name));
}

void DatabaseTest::testCaseAddUserGroupFail_data()
{
    QTest::addColumn<QString>("name");

    // Invalid name
    QTest::newRow("1") << QString();
    QTest::newRow("2") << "";

    // Existing name
    QTest::newRow("3") << "userGroup1";
}

void DatabaseTest::testCaseAddUserGroupFail()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Add user group
    QFETCH(QString, name);
    QVERIFY(!database.addUserGroup(name));
}

void DatabaseTest::testCaseReadAllUserGroupsNonEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::UserGroupInfo> userGroups = database.readAllUserGroups();

    QCOMPARE(userGroups.size(), 2);

    // User group 1
    QCOMPARE(userGroups[0].name(), QString("userGroup1"));

    // User group 2
    QCOMPARE(userGroups[1].name(), QString("userGroup2"));
}

void DatabaseTest::testCaseReadAllUserMappingsEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::UserMappingInfo> userMappings = database.readAllUserMappings();

    QVERIFY(userMappings.isEmpty());
}

void DatabaseTest::testCaseAddUserMapping_data()
{
    QTest::addColumn<qint64>("userGroupId");
    QTest::addColumn<qint64>("userId");

    // Map user 1 to user groups 1 and 2
    QTest::newRow("1") << 1LL << 1LL;
    QTest::newRow("2") << 2LL << 1LL;

    // Map user 2 to user group 2
    QTest::newRow("3") << 2LL << 2LL;
}

void DatabaseTest::testCaseAddUserMapping()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Add user mapping
    QFETCH(qint64, userGroupId);
    QFETCH(qint64, userId);
    QVERIFY(database.addUserMapping(userGroupId, userId));
}

void DatabaseTest::testCaseAddUserMappingFail_data()
{
    QTest::addColumn<qint64>("userGroupId");
    QTest::addColumn<qint64>("userId");

    // Invalid user group ID
    QTest::newRow("1") << 0LL << 1LL;
    QTest::newRow("2") << 3LL << 1LL;

    // Invalid user ID
    QTest::newRow("3") << 1LL << 0LL;
    QTest::newRow("4") << 1LL << 3LL;
}

void DatabaseTest::testCaseAddUserMappingFail()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Add user mapping
    QFETCH(qint64, userGroupId);
    QFETCH(qint64, userId);
    QVERIFY(!database.addUserMapping(userGroupId, userId));
}

void DatabaseTest::testCaseReadAllUserMappingsNonEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::UserMappingInfo> userMappings = database.readAllUserMappings();

    QCOMPARE(userMappings.size(), 3);

    // User mapping 1 (user group 1 to user 1)
    QCOMPARE(userMappings[0].userGroupId(), 1);
    QCOMPARE(userMappings[0].userId(), 1);

    // User mapping 2 (user group 2 to user 1)
    QCOMPARE(userMappings[1].userGroupId(), 2);
    QCOMPARE(userMappings[1].userId(), 1);

    // User mapping 3 (user group 2 to user 2)
    QCOMPARE(userMappings[2].userGroupId(), 2);
    QCOMPARE(userMappings[2].userId(), 2);
}

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_DatabaseTest.moc"
