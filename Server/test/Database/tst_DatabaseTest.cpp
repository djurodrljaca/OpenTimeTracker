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

    // Connection unit tests
    void testCaseConnect();
    void testCaseDisconnect();
    void testCaseReconnect();

    // User unit tests
    void testCaseReadAllUsersEmptyDatabase();
    void testCaseAddUser_data();
    void testCaseAddUser();
    void testCaseAddUserFail_data();
    void testCaseAddUserFail();
    void testCaseReadAllUsersNonEmptyDatabase();

    // User group unit tests
    void testCaseReadAllUserGroupsEmptyDatabase();
    void testCaseAddUserGroup_data();
    void testCaseAddUserGroup();
    void testCaseAddUserGroupFail_data();
    void testCaseAddUserGroupFail();
    void testCaseReadAllUserGroupsNonEmptyDatabase();

    // User mapping unit tests
    void testCaseReadAllUserMappingsEmptyDatabase();
    void testCaseAddUserMapping_data();
    void testCaseAddUserMapping();
    void testCaseAddUserMappingFail_data();
    void testCaseAddUserMappingFail();
    void testCaseReadAllUserMappingsNonEmptyDatabase();

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
    // TODO: implement!

private:
    QString m_databaseFilePath;
};

DatabaseTest::DatabaseTest()
    : m_databaseFilePath("test.db")
{
}

// Initialization and cleanup **********************************************************************

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

// Connection unit tests ***************************************************************************

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

// User unit tests *********************************************************************************

void DatabaseTest::testCaseReadAllUsersEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::User> users = database.readAllUsers();

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

    QList<OpenTimeTracker::Server::User> users = database.readAllUsers();

    QCOMPARE(users.size(), 2);

    // User 1
    QCOMPARE(users[0].name(), QString("user1"));
    QCOMPARE(users[0].password(), QString("111"));

    // User 2
    QCOMPARE(users[1].name(), QString("user2"));
    QVERIFY(users[1].password().isNull());
}

// User group unit tests ***************************************************************************

void DatabaseTest::testCaseReadAllUserGroupsEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::UserGroup> userGroups = database.readAllUserGroups();

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

    QList<OpenTimeTracker::Server::UserGroup> userGroups = database.readAllUserGroups();

    QCOMPARE(userGroups.size(), 2);

    // User group 1
    QCOMPARE(userGroups[0].name(), QString("userGroup1"));

    // User group 2
    QCOMPARE(userGroups[1].name(), QString("userGroup2"));
}

// User mapping unit tests *************************************************************************

void DatabaseTest::testCaseReadAllUserMappingsEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::UserMapping> userMappings = database.readAllUserMappings();

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

    QList<OpenTimeTracker::Server::UserMapping> userMappings = database.readAllUserMappings();

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
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::Event> events =
            database.readEvents(QDateTime(QDate(2015, 12, 23), QTime(0, 00, 00)),
                                QDateTime(QDate(2015, 12, 23), QTime(23, 59, 59)),
                                1LL);

    QVERIFY(events.isEmpty());
}

void DatabaseTest::testCaseAddEvent_data()
{
    QTest::addColumn<QDateTime>("timestamp");
    QTest::addColumn<qint64>("userId");
    QTest::addColumn<OpenTimeTracker::Server::Event::Type>("type");

    // User 1
    QTest::newRow("1") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 1LL
                       << OpenTimeTracker::Server::Event::Type_Started;
    QTest::newRow("2") << QDateTime(QDate(2015, 12, 23), QTime(21, 21, 00))
                       << 1LL
                       << OpenTimeTracker::Server::Event::Type_OnBreak;
    QTest::newRow("3") << QDateTime(QDate(2015, 12, 23), QTime(21, 22, 00))
                       << 1LL
                       << OpenTimeTracker::Server::Event::Type_FromBreak;
    QTest::newRow("4") << QDateTime(QDate(2015, 12, 23), QTime(21, 23, 00))
                       << 1LL
                       << OpenTimeTracker::Server::Event::Type_Finished;

    // User 2
    QTest::newRow("5") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 2LL
                       << OpenTimeTracker::Server::Event::Type_Started;
    QTest::newRow("6") << QDateTime(QDate(2015, 12, 23), QTime(21, 21, 00))
                       << 2LL
                       << OpenTimeTracker::Server::Event::Type_OnBreak;
    QTest::newRow("7") << QDateTime(QDate(2015, 12, 23), QTime(21, 22, 00))
                       << 2LL
                       << OpenTimeTracker::Server::Event::Type_FromBreak;
    QTest::newRow("8") << QDateTime(QDate(2015, 12, 23), QTime(21, 23, 00))
                       << 2LL
                       << OpenTimeTracker::Server::Event::Type_Finished;
}

void DatabaseTest::testCaseAddEvent()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Add event
    QFETCH(QDateTime, timestamp);
    QFETCH(qint64, userId);
    QFETCH(OpenTimeTracker::Server::Event::Type, type);
    QVERIFY(database.addEvent(timestamp, userId, type));
}

void DatabaseTest::testCaseAddEventFail_data()
{
    QTest::addColumn<QDateTime>("timestamp");
    QTest::addColumn<qint64>("userId");
    QTest::addColumn<OpenTimeTracker::Server::Event::Type>("type");

    // Invalid timestamp
    QTest::newRow("1") << QDateTime() << 1LL << OpenTimeTracker::Server::Event::Type_Started;

    // Invalid user ID
    QTest::newRow("2") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 0LL
                       << OpenTimeTracker::Server::Event::Type_Started;
    QTest::newRow("3") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 3LL
                       << OpenTimeTracker::Server::Event::Type_Started;

    // Invalid type
    QTest::newRow("4") << QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00))
                       << 1LL
                       << OpenTimeTracker::Server::Event::Type_Invalid;
}

void DatabaseTest::testCaseAddEventFail()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Add event
    QFETCH(QDateTime, timestamp);
    QFETCH(qint64, userId);
    QFETCH(OpenTimeTracker::Server::Event::Type, type);
    QVERIFY(!database.addEvent(timestamp, userId, type));
}

void DatabaseTest::testCaseReadEventsNonEmptyDatabase()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));
    QList<OpenTimeTracker::Server::Event> events;

    // User 1 *****

    // Time range before events
    events = database.readEvents(QDateTime(QDate(2015, 12, 23), QTime(0, 00, 00)),
                                 QDateTime(QDate(2015, 12, 23), QTime(21, 19, 59)),
                                 1LL);

    QVERIFY(events.isEmpty());

    // Time range for only one event
    events = database.readEvents(QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)),
                                 QDateTime(QDate(2015, 12, 23), QTime(21, 20, 01)),
                                 1LL);

    QCOMPARE(events.size(), 1);

    QCOMPARE(events[0].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)));
    QCOMPARE(events[0].userId(), 1LL);
    QCOMPARE(events[0].type(), OpenTimeTracker::Server::Event::Type_Started);

    // Time range for all events
    events = database.readEvents(QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)),
                                 QDateTime(QDate(2015, 12, 23), QTime(21, 23, 01)),
                                 1LL);

    QCOMPARE(events.size(), 4);

    QCOMPARE(events[0].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)));
    QCOMPARE(events[0].userId(), 1LL);
    QCOMPARE(events[0].type(), OpenTimeTracker::Server::Event::Type_Started);

    QCOMPARE(events[1].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 21, 00)));
    QCOMPARE(events[1].userId(), 1LL);
    QCOMPARE(events[1].type(), OpenTimeTracker::Server::Event::Type_OnBreak);

    QCOMPARE(events[2].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 22, 00)));
    QCOMPARE(events[2].userId(), 1LL);
    QCOMPARE(events[2].type(), OpenTimeTracker::Server::Event::Type_FromBreak);

    QCOMPARE(events[3].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 23, 00)));
    QCOMPARE(events[3].userId(), 1LL);
    QCOMPARE(events[3].type(), OpenTimeTracker::Server::Event::Type_Finished);

    // Time range after events
    events = database.readEvents(QDateTime(QDate(2015, 12, 23), QTime(21, 23, 01)),
                                 QDateTime(QDate(2015, 12, 23), QTime(23, 59, 59)),
                                 1LL);

    QVERIFY(events.isEmpty());

    // User 2 *****

    // Just check if an event for user 2 can be read
    events = database.readEvents(QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)),
                                 QDateTime(QDate(2015, 12, 23), QTime(21, 20, 01)),
                                 2LL);

    QCOMPARE(events.size(), 1);

    QCOMPARE(events[0].timestamp(), QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)));
    QCOMPARE(events[0].userId(), 2LL);
    QCOMPARE(events[0].type(), OpenTimeTracker::Server::Event::Type_Started);
}

// Change event unit tests *************************************************************************

void DatabaseTest::testCaseReadEventChangeLogUnchangedEvent()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    QList<OpenTimeTracker::Server::EventChangeLogItem> eventChangeLog =
            database.readEventChangeLog(1LL);

    QVERIFY(eventChangeLog.isEmpty());
}

void DatabaseTest::testCaseChangeEventTimestamp()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Change event's timestamp
    const qint64 eventId = 1LL;
    const QDateTime newTimestamp(QDate(2015, 12, 23), QTime(20, 20, 00));

    QVERIFY(database.changeEventTimestamp(eventId, newTimestamp, 1LL, "Changed timestamp"));

    // Read event and check if the timestamp was really changed
    const OpenTimeTracker::Server::Event event = database.readEvent(eventId);

    QCOMPARE(event.timestamp(), newTimestamp);
}

void DatabaseTest::testCaseChangeEventTimestampFail()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Change to the same event's timestamp
    const qint64 eventId = 1LL;
    const QDateTime newTimestamp(QDate(2015, 12, 23), QTime(20, 20, 00));

    QVERIFY(!database.changeEventTimestamp(eventId, newTimestamp, 1LL, "Same timestamp"));

    // Invalid event's timestamp
    QVERIFY(!database.changeEventTimestamp(eventId, QDateTime(), 1LL, "Invalid timestamp"));
}

void DatabaseTest::testCaseChangeEventType()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Change event's type
    const qint64 eventId = 1LL;
    const OpenTimeTracker::Server::Event::Type
            newType(OpenTimeTracker::Server::Event::Type_OnBreak);

    QVERIFY(database.changeEventType(eventId, newType, 1LL, "Changed type"));

    // Read event and check if the type was really changed
    const OpenTimeTracker::Server::Event event = database.readEvent(eventId);

    QCOMPARE(event.type(), newType);
}

void DatabaseTest::testCaseChangeEventTypeFail()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Change to the same event's type
    const qint64 eventId = 1LL;
    const OpenTimeTracker::Server::Event::Type
            newType(OpenTimeTracker::Server::Event::Type_OnBreak);

    QVERIFY(!database.changeEventType(eventId, newType, 1LL, "Same type"));

    // Invalid event's type
    QVERIFY(!database.changeEventType(eventId,
                                      OpenTimeTracker::Server::Event::Type_Invalid,
                                      1LL,
                                      "Invalid type"));
}

void DatabaseTest::testCaseChangeEventEnableState()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Change event's enable state
    const qint64 eventId = 1LL;
    bool newEnableState = false;

    QVERIFY(database.changeEventEnableState(eventId, newEnableState, 1LL, "Changed enable state"));

    // Read event and check if the enable state was really changed
    const OpenTimeTracker::Server::Event event = database.readEvent(eventId);

    QCOMPARE(event.isEnabled(), newEnableState);
}

void DatabaseTest::testCaseChangeEventEnableStateFail()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));

    // Change to the same event's enable state
    const qint64 eventId = 1LL;
    bool newEnableState = false;

    QVERIFY(!database.changeEventEnableState(eventId, newEnableState, 1LL, "Same enable state"));
}

void DatabaseTest::testCaseReadEventChangeLogChangedEvent()
{
    OpenTimeTracker::Server::Database database;

    QVERIFY(database.connect(m_databaseFilePath));
    const qint64 eventId = 1LL;
    const QList<OpenTimeTracker::Server::EventChangeLogItem> eventChangeLog =
            database.readEventChangeLog(eventId);

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
    QCOMPARE(eventChangeLog[0].fieldName(), QStringLiteral("timestamp"));
    QCOMPARE(fromValueTimestamp, QDateTime(QDate(2015, 12, 23), QTime(21, 20, 00)));
    QCOMPARE(toValueTimestamp, QDateTime(QDate(2015, 12, 23), QTime(20, 20, 00)));

    // Check changed type
    bool success = false;
    OpenTimeTracker::Server::Event::Type fromValueType =
            static_cast<OpenTimeTracker::Server::Event::Type>(
                eventChangeLog[1].fromValue().toInt(&success));
    QVERIFY(success);

    success = false;
    OpenTimeTracker::Server::Event::Type toValueType =
            static_cast<OpenTimeTracker::Server::Event::Type>(
                eventChangeLog[1].toValue().toInt(&success));
    QVERIFY(success);

    QCOMPARE(eventChangeLog[1].eventId(), eventId);
    QVERIFY(eventChangeLog[1].timestamp().secsTo(currentTime) >= 0);
    QVERIFY(eventChangeLog[1].timestamp().secsTo(currentTime) < 60);
    QCOMPARE(eventChangeLog[1].fieldName(), QStringLiteral("type"));
    QCOMPARE(fromValueType, OpenTimeTracker::Server::Event::Type_Started);
    QCOMPARE(toValueType, OpenTimeTracker::Server::Event::Type_OnBreak);

    // Check changed enable state
    const bool fromValueEnableState = eventChangeLog[2].fromValue().toBool();
    const bool toValueEnableState = eventChangeLog[2].toValue().toBool();

    QCOMPARE(eventChangeLog[2].eventId(), eventId);
    QVERIFY(eventChangeLog[2].timestamp().secsTo(currentTime) >= 0);
    QVERIFY(eventChangeLog[2].timestamp().secsTo(currentTime) < 60);
    QCOMPARE(eventChangeLog[2].fieldName(), QStringLiteral("enabled"));
    QCOMPARE(fromValueEnableState, true);
    QCOMPARE(toValueEnableState, false);
}

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_DatabaseTest.moc"
