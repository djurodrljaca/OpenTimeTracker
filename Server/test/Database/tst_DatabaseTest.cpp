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
#include "../../src/Database/DatabaseManagement.hpp"
#include "../../src/Database/EventManagement.hpp"
#include "../../src/Database/SettingsManagement.hpp"
#include "../../src/Database/UserManagement.hpp"

Q_DECLARE_METATYPE(OpenTimeTracker::Server::Event::Type)

class DatabaseTest : public QObject
{
    Q_OBJECT

public:
    DatabaseTest();

private Q_SLOTS:
    // Initialization and cleanup
    void initTestCase();
    void cleanupTestCase();


    // TODO: check below if any tests need to be added, removed or modified!


    // Connection unit tests
    void testCaseConnect();
    void testCaseDisconnect();
    void testCaseReconnect();

    // User unit tests
    void testCaseReadUsersEmptyDatabase();
    void testCaseAddUser_data();
    void testCaseAddUser();
    void testCaseAddUserFail_data();
    void testCaseAddUserFail();
    // TODO: change user: name, password, enable state
    void testCaseReadUsersNonEmptyDatabase();

    // User group unit tests
    void testCaseReadUserGroupsEmptyDatabase();
    void testCaseAddUserGroup_data();
    void testCaseAddUserGroup();
    void testCaseAddUserGroupFail_data();
    void testCaseAddUserGroupFail();
    // TODO: change user group name
    void testCaseReadUserGroupsNonEmptyDatabase();

    // User mapping unit tests
    void testCaseReadUserMappingsEmptyDatabase();
    void testCaseAddUserMapping_data();
    void testCaseAddUserMapping();
    void testCaseAddUserMappingFail_data();
    void testCaseAddUserMappingFail();
    // TODO: remove user mapping
    void testCaseReadUserMappingsNonEmptyDatabase();

    // Event unit tests
    void testCaseReadEventsEmptyDatabase();
    void testCaseAddEvent_data();
    void testCaseAddEvent();
    void testCaseAddEventFail_data();
    void testCaseAddEventFail();
    void testCaseReadEventsNonEmptyDatabase();

    // Change event unit tests
    void testCaseReadEventChangeLogUnchangedEvent();
    void testCaseChangeEventTimestamp();
    void testCaseChangeEventTimestampFail();
    void testCaseChangeEventType();
    void testCaseChangeEventTypeFail();
    void testCaseChangeEventEnableState();
    void testCaseChangeEventEnableStateFail();
    void testCaseReadEventChangeLogChangedEvent();

    // Settings unit tests
    void testCaseReadSettingsEmptyDatabase();
    void testCaseAddSettings();
    void testCaseAddSettingFail_data();
    void testCaseAddSettingFail();
    // TODO: change setting
    void testCaseReadSettingsNonEmptyDatabase();

private:
    void removeDatabaseFile();

    const QString m_databaseFilePath;
};

DatabaseTest::DatabaseTest()
    : m_databaseFilePath("test.db")
{
}

// Initialization and cleanup **********************************************************************

void DatabaseTest::removeDatabaseFile()
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

void DatabaseTest::initTestCase()
{
    removeDatabaseFile();
}

void DatabaseTest::cleanupTestCase()
{
    removeDatabaseFile();
}

// Connection unit tests ***************************************************************************

void DatabaseTest::testCaseConnect()
{
    using namespace OpenTimeTracker::Server::Database;

    // Initially it has to be disconnected
    QCOMPARE(DatabaseManagement::isConnected(), false);

    // Connecting should be successful
    QVERIFY(DatabaseManagement::connect(m_databaseFilePath));

    // Check if really connected
    QCOMPARE(DatabaseManagement::isConnected(), true);
}

void DatabaseTest::testCaseDisconnect()
{
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Disconnect
    DatabaseManagement::disconnect();

    // Check if really disconnected
    QCOMPARE(DatabaseManagement::isConnected(), false);
}

void DatabaseTest::testCaseReconnect()
{
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Disconnect
    DatabaseManagement::disconnect();

    // Check if really disconnected
    QCOMPARE(DatabaseManagement::isConnected(), false);

    // Reconnecting should be successful
    QVERIFY(DatabaseManagement::connect(m_databaseFilePath));

    // Check if really connected
    QCOMPARE(DatabaseManagement::isConnected(), true);
}

// User unit tests *********************************************************************************

void DatabaseTest::testCaseReadUsersEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read users
    QList<User> users = UserManagement::readUsers();

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
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add user
    QFETCH(QString, name);
    QFETCH(QString, password);
    QVERIFY(UserManagement::addUser(name, password));
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

    // Existing password
    QTest::newRow("4") << "user6" << "111";
}

void DatabaseTest::testCaseAddUserFail()
{
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add user
    QFETCH(QString, name);
    QFETCH(QString, password);
    QVERIFY(!UserManagement::addUser(name, password));
}

void DatabaseTest::testCaseReadUsersNonEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read users
    QList<User> users = UserManagement::readUsers();

    QCOMPARE(users.size(), 2);

    // User 1
    QCOMPARE(users[0].name(), QString("user1"));
    QCOMPARE(users[0].password(), QString("111"));

    // User 2
    QCOMPARE(users[1].name(), QString("user2"));
    QVERIFY(users[1].password().isNull());
}

// User group unit tests ***************************************************************************

void DatabaseTest::testCaseReadUserGroupsEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read user groups
    QList<UserGroup> userGroups = UserManagement::readUserGroups();

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
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add user group
    QFETCH(QString, name);
    QVERIFY(UserManagement::addUserGroup(name));
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
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add user group
    QFETCH(QString, name);
    QVERIFY(!UserManagement::addUserGroup(name));
}

void DatabaseTest::testCaseReadUserGroupsNonEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read user groups
    QList<UserGroup> userGroups = UserManagement::readUserGroups();

    QCOMPARE(userGroups.size(), 2);

    // User group 1
    QCOMPARE(userGroups[0].name(), QString("userGroup1"));

    // User group 2
    QCOMPARE(userGroups[1].name(), QString("userGroup2"));
}

// User mapping unit tests *************************************************************************

void DatabaseTest::testCaseReadUserMappingsEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read user mappings
    QList<UserMapping> userMappings = UserManagement::readUserMappings();

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
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add user mapping
    QFETCH(qint64, userGroupId);
    QFETCH(qint64, userId);
    QVERIFY(UserManagement::addUserMapping(userGroupId, userId));
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
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add user mapping
    QFETCH(qint64, userGroupId);
    QFETCH(qint64, userId);
    QVERIFY(!UserManagement::addUserMapping(userGroupId, userId));
}

void DatabaseTest::testCaseReadUserMappingsNonEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read user mappings
    QList<UserMapping> userMappings = UserManagement::readUserMappings();

    QCOMPARE(userMappings.size(), 3);

    // User mapping 1 (user group 1 to user 1)
    QCOMPARE(userMappings[0].userGroupId(), 1LL);
    QCOMPARE(userMappings[0].userId(), 1LL);

    // User mapping 2 (user group 2 to user 1)
    QCOMPARE(userMappings[1].userGroupId(), 2LL);
    QCOMPARE(userMappings[1].userId(), 1LL);

    // User mapping 3 (user group 2 to user 2)
    QCOMPARE(userMappings[2].userGroupId(), 2LL);
    QCOMPARE(userMappings[2].userId(), 2LL);
}

// Event unit tests ********************************************************************************

void DatabaseTest::testCaseReadEventsEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read events
    QList<Event> events = EventManagement::readEvents(
                              QDateTime(QDate(2015, 12, 23), QTime(0, 00, 00)),
                              QDateTime(QDate(2015, 12, 23), QTime(23, 59, 59)),
                              1LL);

    QVERIFY(events.isEmpty());
}

void DatabaseTest::testCaseAddEvent_data()
{
    using namespace OpenTimeTracker::Server;

    QTest::addColumn<QDateTime>("timestamp");
    QTest::addColumn<qint64>("userId");
    QTest::addColumn<Event::Type>("type");

    // User 1
    QTest::newRow("1") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 1LL
                       << Event::Type_Started;
    QTest::newRow("2") << QDateTime(QDate(2015, 12, 23), QTime(21, 21, 00))
                       << 1LL
                       << Event::Type_OnBreak;
    QTest::newRow("3") << QDateTime(QDate(2015, 12, 23), QTime(21, 22, 00))
                       << 1LL
                       << Event::Type_FromBreak;
    QTest::newRow("4") << QDateTime(QDate(2015, 12, 23), QTime(21, 23, 00))
                       << 1LL
                       << Event::Type_Finished;

    // User 2
    QTest::newRow("5") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 2LL
                       << Event::Type_Started;
    QTest::newRow("6") << QDateTime(QDate(2015, 12, 23), QTime(21, 21, 00))
                       << 2LL
                       << Event::Type_OnBreak;
    QTest::newRow("7") << QDateTime(QDate(2015, 12, 23), QTime(21, 22, 00))
                       << 2LL
                       << Event::Type_FromBreak;
    QTest::newRow("8") << QDateTime(QDate(2015, 12, 23), QTime(21, 23, 00))
                       << 2LL
                       << Event::Type_Finished;
}

void DatabaseTest::testCaseAddEvent()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add event
    QFETCH(QDateTime, timestamp);
    QFETCH(qint64, userId);
    QFETCH(Event::Type, type);
    QVERIFY(EventManagement::addEvent(timestamp, userId, type));
}

void DatabaseTest::testCaseAddEventFail_data()
{
    using namespace OpenTimeTracker::Server;

    QTest::addColumn<QDateTime>("timestamp");
    QTest::addColumn<qint64>("userId");
    QTest::addColumn<Event::Type>("type");

    // Invalid timestamp
    QTest::newRow("1") << QDateTime() << 1LL << Event::Type_Started;

    // Invalid user ID
    QTest::newRow("2") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 0LL
                       << Event::Type_Started;
    QTest::newRow("3") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 3LL
                       << Event::Type_Started;

    // Invalid type
    QTest::newRow("4") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 1LL
                       << Event::Type_Invalid;
}

void DatabaseTest::testCaseAddEventFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add event
    QFETCH(QDateTime, timestamp);
    QFETCH(qint64, userId);
    QFETCH(Event::Type, type);
    QVERIFY(!EventManagement::addEvent(timestamp, userId, type));
}

void DatabaseTest::testCaseReadEventsNonEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // User 1 *****

    // Time range before events
    QList<Event> events;
    events = EventManagement::readEvents(QDateTime(QDate(2015, 12, 23), QTime(0, 00, 00)),
                                         QDateTime(QDate(2015, 12, 23), QTime(21, 19, 59)),
                                         1LL);

    QVERIFY(events.isEmpty());

    // Time range for only one event
    events = EventManagement::readEvents(QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)),
                                         QDateTime(QDate(2015, 12, 23), QTime(21, 20, 01)),
                                         1LL);

    QCOMPARE(events.size(), 1);

    QCOMPARE(events[0].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)));
    QCOMPARE(events[0].userId(), 1LL);
    QCOMPARE(events[0].type(), Event::Type_Started);

    // Time range for all events
    events = EventManagement::readEvents(QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)),
                                         QDateTime(QDate(2015, 12, 23), QTime(21, 23, 01)),
                                         1LL);

    QCOMPARE(events.size(), 4);

    QCOMPARE(events[0].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)));
    QCOMPARE(events[0].userId(), 1LL);
    QCOMPARE(events[0].type(), Event::Type_Started);

    QCOMPARE(events[1].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 21, 00)));
    QCOMPARE(events[1].userId(), 1LL);
    QCOMPARE(events[1].type(), Event::Type_OnBreak);

    QCOMPARE(events[2].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 22, 00)));
    QCOMPARE(events[2].userId(), 1LL);
    QCOMPARE(events[2].type(), Event::Type_FromBreak);

    QCOMPARE(events[3].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 23, 00)));
    QCOMPARE(events[3].userId(), 1LL);
    QCOMPARE(events[3].type(), Event::Type_Finished);

    // Time range after events
    events = EventManagement::readEvents(QDateTime(QDate(2015, 12, 23), QTime(21, 23, 01)),
                                         QDateTime(QDate(2015, 12, 23), QTime(23, 59, 59)),
                                         1LL);

    QVERIFY(events.isEmpty());

    // User 2 *****

    // Just check if an event for user 2 can be read
    events = EventManagement::readEvents(QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)),
                                         QDateTime(QDate(2015, 12, 23), QTime(21, 20, 01)),
                                         2LL);

    QCOMPARE(events.size(), 1);

    QCOMPARE(events[0].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)));
    QCOMPARE(events[0].userId(), 2LL);
    QCOMPARE(events[0].type(), Event::Type_Started);
}

// Change event unit tests *************************************************************************

void DatabaseTest::testCaseReadEventChangeLogUnchangedEvent()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read event change log
    QList<EventChangeLogItem> eventChangeLog = EventManagement::readEventChangeLog(1LL);

    QVERIFY(eventChangeLog.isEmpty());
}

void DatabaseTest::testCaseChangeEventTimestamp()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Change event's timestamp
    const qint64 eventId = 1LL;
    const QDateTime newTimestamp(QDate(2015, 12, 23), QTime(20, 20, 00));

    QVERIFY(EventManagement::changeEventTimestamp(eventId, newTimestamp, 1LL, "Changed timestamp"));

    // Read event and check if the timestamp was really changed
    const Event event = EventManagement::readEvent(eventId);

    QCOMPARE(event.timestamp(), newTimestamp);
}

void DatabaseTest::testCaseChangeEventTimestampFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Change to the same event's timestamp
    const qint64 eventId = 1LL;
    const QDateTime newTimestamp(QDate(2015, 12, 23), QTime(20, 20, 00));

    QVERIFY(!EventManagement::changeEventTimestamp(eventId, newTimestamp, 1LL, "Same timestamp"));

    // Invalid event's timestamp
    QVERIFY(!EventManagement::changeEventTimestamp(eventId, QDateTime(), 1LL, "Invalid timestamp"));
}

void DatabaseTest::testCaseChangeEventType()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Change event's type
    const qint64 eventId = 1LL;
    const Event::Type newType(Event::Type_OnBreak);

    QVERIFY(EventManagement::changeEventType(eventId, newType, 1LL, "Changed type"));

    // Read event and check if the type was really changed
    const Event event = EventManagement::readEvent(eventId);

    QCOMPARE(event.type(), newType);
}

void DatabaseTest::testCaseChangeEventTypeFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Change to the same event's type
    const qint64 eventId = 1LL;
    const Event::Type newType(Event::Type_OnBreak);

    QVERIFY(!EventManagement::changeEventType(eventId, newType, 1LL, "Same type"));

    // Invalid event's type
    QVERIFY(!EventManagement::changeEventType(eventId,
                                              Event::Type_Invalid,
                                              1LL,
                                              "Invalid type"));
}

void DatabaseTest::testCaseChangeEventEnableState()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Change event's enable state
    const qint64 eventId = 1LL;
    bool newEnableState = false;

    QVERIFY(EventManagement::changeEventEnableState(eventId,
                                                    newEnableState,
                                                    1LL,
                                                    "Changed enable state"));

    // Read event and check if the enable state was really changed
    const Event event = EventManagement::readEvent(eventId);

    QCOMPARE(event.isEnabled(), newEnableState);
}

void DatabaseTest::testCaseChangeEventEnableStateFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Change to the same event's enable state
    const qint64 eventId = 1LL;
    bool newEnableState = false;

    QVERIFY(!EventManagement::changeEventEnableState(eventId,
                                                     newEnableState,
                                                     1LL,
                                                     "Same enable state"));
}

void DatabaseTest::testCaseReadEventChangeLogChangedEvent()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read event change log
    const qint64 eventId = 1LL;
    const QList<EventChangeLogItem> eventChangeLog = EventManagement::readEventChangeLog(eventId);

    QCOMPARE(eventChangeLog.size(), 3);

    // Check changed timestamp
    const QDateTime currentTime = QDateTime::currentDateTime();

    QDateTime fromValueTimestamp = eventChangeLog[0].fromValue().toDateTime();
    fromValueTimestamp.setTimeSpec(Qt::UTC);

    QDateTime toValueTimestamp = eventChangeLog[0].toValue().toDateTime();
    toValueTimestamp.setTimeSpec(Qt::UTC);

    QCOMPARE(eventChangeLog[0].eventId(), eventId);
    QVERIFY(eventChangeLog[0].timestamp().secsTo(currentTime) >= 0);
    QVERIFY(eventChangeLog[0].timestamp().secsTo(currentTime) < 60);
    QCOMPARE(eventChangeLog[0].fieldName(), QString("timestamp"));
    QCOMPARE(fromValueTimestamp, QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)));
    QCOMPARE(toValueTimestamp, QDateTime(QDate(2015, 12, 23), QTime(20, 20, 00)));

    // Check changed type
    bool success = false;
    Event::Type fromValueType = static_cast<Event::Type>(
                                    eventChangeLog[1].fromValue().toInt(&success));
    QVERIFY(success);

    success = false;
    Event::Type toValueType = static_cast<Event::Type>(
                                  eventChangeLog[1].toValue().toInt(&success));
    QVERIFY(success);

    QCOMPARE(eventChangeLog[1].eventId(), eventId);
    QVERIFY(eventChangeLog[1].timestamp().secsTo(currentTime) >= 0);
    QVERIFY(eventChangeLog[1].timestamp().secsTo(currentTime) < 60);
    QCOMPARE(eventChangeLog[1].fieldName(), QString("type"));
    QCOMPARE(fromValueType, Event::Type_Started);
    QCOMPARE(toValueType, Event::Type_OnBreak);

    // Check changed enable state
    const bool fromValueEnableState = eventChangeLog[2].fromValue().toBool();
    const bool toValueEnableState = eventChangeLog[2].toValue().toBool();

    QCOMPARE(eventChangeLog[2].eventId(), eventId);
    QVERIFY(eventChangeLog[2].timestamp().secsTo(currentTime) >= 0);
    QVERIFY(eventChangeLog[2].timestamp().secsTo(currentTime) < 60);
    QCOMPARE(eventChangeLog[2].fieldName(), QString("enabled"));
    QCOMPARE(fromValueEnableState, true);
    QCOMPARE(toValueEnableState, false);
}

// Setting unit tests ******************************************************************************

void DatabaseTest::testCaseReadSettingsEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read settings
    QMap<QString, QVariant> settings = SettingsManagement::readSettings();

    QVERIFY(settings.isEmpty());
}

void DatabaseTest::testCaseAddSettings()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Prepare new settings
    QMap<QString, QVariant> settings;
    settings["setting1"] = "stringValue";
    settings["setting2"] = 123;

    // Add settings
    QVERIFY(SettingsManagement::addSettings(settings));
}

void DatabaseTest::testCaseAddSettingFail_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QVariant>("value");

    // Invalid name
    QTest::newRow("1") << QString() << QVariant(QString());
    QTest::newRow("2") << "" << QVariant(QString());

    // Existing name
    QTest::newRow("3") << "setting1" << QVariant("newStringValue");
}

void DatabaseTest::testCaseAddSettingFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add setting
    QFETCH(QString, name);
    QFETCH(QVariant, value);

    // Prepare new (invalid) setting
    QMap<QString, QVariant> settings;
    settings[name] = value;

    // Add (invalid) setting
    QVERIFY(!SettingsManagement::addSettings(settings));
}

void DatabaseTest::testCaseReadSettingsNonEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read settings
    QMap<QString, QVariant> settings = SettingsManagement::readSettings();

    QCOMPARE(settings.size(), 2);

    // Setting 1
    QVERIFY(settings.contains("setting1"));
    QCOMPARE(settings["setting1"], QVariant("stringValue"));

    // Setting 2
    QVERIFY(settings.contains("setting2"));
    QCOMPARE(settings["setting2"], QVariant(123));

}

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_DatabaseTest.moc"
