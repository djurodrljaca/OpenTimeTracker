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
#include "../../src/Database/ScheduleManagement.hpp"
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

    // Connection unit tests
    void testCaseConnect();
    void testCaseDisconnect();
    void testCaseReconnect();

    // Settings unit tests
    void testCaseReadSettingsEmptyDatabase();
    void testCaseAddSettings();
    void testCaseAddSettingFail_data();
    void testCaseAddSettingFail();
    void testCaseChangeSetting();
    void testCaseChangeSettingFail();
    void testCaseReadSettingsNonEmptyDatabase();

    // Working days unit tests
    void testCaseReadWorkingDaysEmptyDatabase();
    void testCaseAddWorkingDay_data();
    void testCaseAddWorkingDay();
    void testCaseAddWorkingDayFail_data();
    void testCaseAddWorkingDayFail();
    void testCaseReadWorkingDaysNonEmptyDatabase();

    // User unit tests
    void testCaseReadUsersEmptyDatabase();
    void testCaseAddUser_data();
    void testCaseAddUser();
    void testCaseAddUserFail_data();
    void testCaseAddUserFail();
    void testCaseChangeUserName();
    void testCaseChangeUserNameFail();
    void testCaseChangeUserPassword();
    void testCaseChangeUserPasswordFail();
    void testCaseChangeUserEnableState();
    void testCaseReadUsersNonEmptyDatabase();

    // User group unit tests
    void testCaseReadUserGroupsEmptyDatabase();
    void testCaseAddUserGroup_data();
    void testCaseAddUserGroup();
    void testCaseAddUserGroupFail_data();
    void testCaseAddUserGroupFail();
    void testCaseChangeUserGroupName();
    void testCaseChangeUserGroupNameFail();
    void testCaseReadUserGroupsNonEmptyDatabase();

    // User mapping unit tests
    void testCaseReadUserMappingsEmptyDatabase();
    void testCaseAddUserMapping_data();
    void testCaseAddUserMapping();
    void testCaseAddUserMappingFail_data();
    void testCaseAddUserMappingFail();
    void testCaseRemoveUserMapping();
    void testCaseReadUserMappingsNonEmptyDatabase();

    // Schedules unit tests
    void testCaseReadSchedulesEmptyDatabase();
    void testCaseAddSchedule_data();
    void testCaseAddSchedule();
    void testCaseAddScheduleFail_data();
    void testCaseAddScheduleFail();
    void testCaseRemoveSchedule();
    void testCaseReadSchedulesNonEmptyDatabase();
    void testCaseReadSchedulesUserId();

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

private:
    void removeDatabaseFile();
    OpenTimeTracker::Server::User readUser(const qint64 &userId);
    OpenTimeTracker::Server::UserGroup readUserGroup(const qint64 &userGroupId);
    OpenTimeTracker::Server::UserMapping readUserMapping(const qint64 &userMappingId);

    const QString m_databaseFilePath;
};

DatabaseTest::DatabaseTest()
    : m_databaseFilePath("test.db")
{
}

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

OpenTimeTracker::Server::User DatabaseTest::readUser(const qint64 &userId)
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    QList<User> users = UserManagement::readUsers();

    // Find user
    User user;

    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].id() == userId)
        {
            user = users[i];
            break;
        }
    }

    return user;
}

OpenTimeTracker::Server::UserGroup DatabaseTest::readUserGroup(const qint64 &userGroupId)
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    QList<UserGroup> userGroups = UserManagement::readUserGroups();

    // Find user group
    UserGroup userGroup;

    for (int i = 0; i < userGroups.size(); i++)
    {
        if (userGroups[i].id() == userGroupId)
        {
            userGroup = userGroups[i];
            break;
        }
    }

    return userGroup;
}

OpenTimeTracker::Server::UserMapping DatabaseTest::readUserMapping(const qint64 &userMappingId)
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    QList<UserMapping> userMappings = UserManagement::readUserMappings();

    // Find user mapping
    UserMapping userMapping;

    for (int i = 0; i < userMappings.size(); i++)
    {
        if (userMappings[i].id() == userMappingId)
        {
            userMapping = userMappings[i];
            break;
        }
    }

    return userMapping;
}

// Initialization and cleanup **********************************************************************

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

void DatabaseTest::testCaseChangeSetting()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Check if setting exists
    const QString name("setting2");
    QMap<QString, QVariant> settingsOld = SettingsManagement::readSettings();
    QVERIFY(settingsOld.contains(name));

    // Make sure the setting will be changed
    const QVariant newValue(999);
    QVERIFY(settingsOld[name] != newValue);

    // Change setting
    QVERIFY(SettingsManagement::changeSetting(name, newValue));

    // Make sure the setting was changed
    QMap<QString, QVariant> settingsNew = SettingsManagement::readSettings();
    QVERIFY(settingsNew.contains(name));
    QCOMPARE(settingsNew[name], newValue);
}

void DatabaseTest::testCaseChangeSettingFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Nonexistent setting
    QVERIFY(!SettingsManagement::changeSetting(QString("settingX"), QString()));
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
    QCOMPARE(settings["setting2"], QVariant(999));

}

// Working days unit tests *************************************************************************

void DatabaseTest::testCaseReadWorkingDaysEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read working day for the specified timestamp
    const QDateTime timestamp(QDate(2016, 01, 02), QTime(15, 00, 00));
    const QPair<QDateTime, QDateTime> workingDay = ScheduleManagement::readWorkingDay(timestamp);

    // Working day time range should be invalid
    QVERIFY(workingDay.first.isValid() == false);
    QVERIFY(workingDay.second.isValid() == false);
}

void DatabaseTest::testCaseAddWorkingDay_data()
{
    QTest::addColumn<QDateTime>("start");
    QTest::addColumn<QDateTime>("end");

    QTest::newRow("1") << QDateTime(QDate(2016, 01, 01), QTime(7, 00, 00))
                       << QDateTime(QDate(2016, 01, 01), QTime(22, 59, 59));

    QTest::newRow("2") << QDateTime(QDate(2016, 01, 02), QTime(7, 00, 00))
                       << QDateTime(QDate(2016, 01, 02), QTime(22, 59, 59));
}

void DatabaseTest::testCaseAddWorkingDay()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add working day
    QFETCH(QDateTime, start);
    QFETCH(QDateTime, end);

    QVERIFY(ScheduleManagement::addWorkingDay(start, end));
}

void DatabaseTest::testCaseAddWorkingDayFail_data()
{
    QTest::addColumn<QDateTime>("start");
    QTest::addColumn<QDateTime>("end");

    // Invalid start and end timestamp
    QTest::newRow("1") << QDateTime()
                       << QDateTime(QDate(2016, 01, 01), QTime(22, 59, 59));

    QTest::newRow("2") << QDateTime(QDate(2016, 01, 02), QTime(7, 00, 00))
                       << QDateTime();

    // Start bigger than end
    QTest::newRow("3") << QDateTime(QDate(2016, 01, 01), QTime(22, 59, 59))
                       << QDateTime(QDate(2016, 01, 01), QTime(7, 00, 00));
}

void DatabaseTest::testCaseAddWorkingDayFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Add working day (expect failure)
    QFETCH(QDateTime, start);
    QFETCH(QDateTime, end);

    QVERIFY(!ScheduleManagement::addWorkingDay(start, end));
}

void DatabaseTest::testCaseReadWorkingDaysNonEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read working day 1
    QDateTime timestamp(QDate(2016, 01, 01), QTime(15, 00, 00));
    QPair<QDateTime, QDateTime> workingDay = ScheduleManagement::readWorkingDay(timestamp);

    QCOMPARE(workingDay.first, QDateTime(QDate(2016, 01, 01), QTime(7, 00, 00)));
    QCOMPARE(workingDay.second, QDateTime(QDate(2016, 01, 01), QTime(22, 59, 59)));

    // Timestamp a second before working day 2
    timestamp = QDateTime(QDate(2016, 01, 02), QTime(6, 59, 59));
    workingDay = ScheduleManagement::readWorkingDay(timestamp);

    QVERIFY(!workingDay.first.isValid());
    QVERIFY(!workingDay.second.isValid());

    // Read working day 2: start
    timestamp = QDateTime(QDate(2016, 01, 02), QTime(7, 00, 00));
    workingDay = ScheduleManagement::readWorkingDay(timestamp);

    QCOMPARE(workingDay.first, QDateTime(QDate(2016, 01, 02), QTime(7, 00, 00)));
    QCOMPARE(workingDay.second, QDateTime(QDate(2016, 01, 02), QTime(22, 59, 59)));

    // Read working day 2: middle
    timestamp = QDateTime(QDate(2016, 01, 02), QTime(15, 00, 00));
    workingDay = ScheduleManagement::readWorkingDay(timestamp);

    QCOMPARE(workingDay.first, QDateTime(QDate(2016, 01, 02), QTime(7, 00, 00)));
    QCOMPARE(workingDay.second, QDateTime(QDate(2016, 01, 02), QTime(22, 59, 59)));

    // Read working day 2: end
    timestamp = QDateTime(QDate(2016, 01, 02), QTime(22, 59, 59));
    workingDay = ScheduleManagement::readWorkingDay(timestamp);

    QCOMPARE(workingDay.first, QDateTime(QDate(2016, 01, 02), QTime(7, 00, 00)));
    QCOMPARE(workingDay.second, QDateTime(QDate(2016, 01, 02), QTime(22, 59, 59)));

    // Timestamp a second after working day 2
    timestamp = QDateTime(QDate(2016, 01, 02), QTime(23, 00, 00));
    workingDay = ScheduleManagement::readWorkingDay(timestamp);

    QVERIFY(!workingDay.first.isValid());
    QVERIFY(!workingDay.second.isValid());
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

void DatabaseTest::testCaseChangeUserName()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read user
    qint64 userId = 2LL;
    User userOld = readUser(userId);
    QVERIFY(userOld.isValid());

    // Make sure the name will be changed
    const QString newName("user2renamed");
    QVERIFY(userOld.name() != newName);

    // Change user name
    QVERIFY(UserManagement::changeUserName(userId, newName));

    // Re-read user
    User userNew = readUser(userId);
    QVERIFY(userNew.isValid());

    // Check if user name was changed
    QCOMPARE(userNew.name(), newName);
}

void DatabaseTest::testCaseChangeUserNameFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Invalid name
    qint64 userId = 2LL;
    QVERIFY(!UserManagement::changeUserName(userId, QString()));
    QVERIFY(!UserManagement::changeUserName(userId, QString("")));

    // Existing name
    QVERIFY(!UserManagement::changeUserName(userId, "user1"));
}

void DatabaseTest::testCaseChangeUserPassword()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read user
    qint64 userId = 2LL;
    User userOld = readUser(userId);
    QVERIFY(userOld.isValid());

    // Make sure the password will be changed
    const QString newPassword("password");
    QVERIFY(userOld.password() != newPassword);

    // Change user password
    QVERIFY(UserManagement::changeUserPassword(userId, newPassword));

    // Re-read user
    User userNew = readUser(userId);
    QVERIFY(userNew.isValid());

    // Check if user password was changed
    QCOMPARE(userNew.password(), newPassword);
}

void DatabaseTest::testCaseChangeUserPasswordFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Existing password
    qint64 userId = 2LL;
    QVERIFY(!UserManagement::changeUserPassword(userId, "111"));
}

void DatabaseTest::testCaseChangeUserEnableState()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read user
    qint64 userId = 2LL;
    User userOld = readUser(userId);
    QVERIFY(userOld.isValid());

    // Make sure the the user is enabled
    QVERIFY(!userOld.password().isEmpty());

    // Disable user
    QVERIFY(UserManagement::disableUser(userId));

    // Re-read user
    User userNew = readUser(userId);
    QVERIFY(userNew.isValid());

    // Check if user was disabled
    QVERIFY(userNew.password().isEmpty());
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
    QCOMPARE(users[1].name(), QString("user2renamed"));
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

void DatabaseTest::testCaseChangeUserGroupName()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read user group
    qint64 userGroupId = 2LL;
    UserGroup userGroupOld = readUserGroup(userGroupId);
    QVERIFY(userGroupOld.isValid());

    // Make sure the name will be changed
    const QString newName("userGroup2renamed");
    QVERIFY(userGroupOld.name() != newName);

    // Change user group name
    QVERIFY(UserManagement::changeUserGroupName(userGroupId, newName));

    // Re-read user group
    UserGroup userGroupNew = readUserGroup(userGroupId);
    QVERIFY(userGroupNew.isValid());

    // Check if user group name was changed
    QCOMPARE(userGroupNew.name(), newName);
}

void DatabaseTest::testCaseChangeUserGroupNameFail()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Invalid name
    qint64 userGroupId = 2LL;
    QVERIFY(!UserManagement::changeUserGroupName(userGroupId, QString()));
    QVERIFY(!UserManagement::changeUserGroupName(userGroupId, QString("")));

    // Existing name
    QVERIFY(!UserManagement::changeUserGroupName(userGroupId, "userGroup1"));
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
    QCOMPARE(userGroups[1].name(), QString("userGroup2renamed"));
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

    // Map user 2 to user groups 2 and 1
    QTest::newRow("3") << 2LL << 2LL;
    QTest::newRow("4") << 1LL << 2LL;
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

    // Existing
    QTest::newRow("5") << 1LL << 1LL;
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

void DatabaseTest::testCaseRemoveUserMapping()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Make sure the mapping exists
    qint64 userMappingId = 4LL;
    UserMapping userMappingOld = readUserMapping(userMappingId);
    QVERIFY(userMappingOld.isValid());

    // Remove user mapping
    QVERIFY(UserManagement::removeUserMapping(userMappingId));

    // Check if user mapping was removed
    UserMapping userMappingNew = readUserMapping(userMappingId);
    QVERIFY(!userMappingNew.isValid());
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

// Schedule unit tests *****************************************************************************

void DatabaseTest::testCaseReadSchedulesEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read schedules
    QList<Schedule> schedules = ScheduleManagement::readSchedules(
                                    QDateTime(QDate(2016, 01, 02), QTime(7, 00, 00)),
                                    QDateTime(QDate(2016, 01, 02), QTime(22, 59, 59)));

    QVERIFY(schedules.isEmpty());
}

void DatabaseTest::testCaseAddSchedule_data()
{
    QTest::addColumn<qint64>("userId");
    QTest::addColumn<QDateTime>("startTimestamp");
    QTest::addColumn<QDateTime>("endTimestamp");

    // Schedules for user 1 for working day 1
    QTest::newRow("1") << 1LL
                       << QDateTime(QDate(2016, 01, 01), QTime(8, 00, 00))
                       << QDateTime(QDate(2016, 01, 01), QTime(12, 00, 00));
    QTest::newRow("2") << 1LL
                       << QDateTime(QDate(2016, 01, 01), QTime(14, 00, 00))
                       << QDateTime(QDate(2016, 01, 01), QTime(18, 00, 00));

    // Schedules for user 1 for working day 2
    QTest::newRow("3") << 1LL
                       << QDateTime(QDate(2016, 01, 02), QTime(8, 00, 00))
                       << QDateTime(QDate(2016, 01, 02), QTime(16, 00, 00));

    // Schedules for user 2 for working day 1
    QTest::newRow("4") << 2LL
                       << QDateTime(QDate(2016, 01, 01), QTime(8, 00, 00))
                       << QDateTime(QDate(2016, 01, 01), QTime(12, 00, 00));
    QTest::newRow("5") << 2LL
                       << QDateTime(QDate(2016, 01, 01), QTime(14, 00, 00))
                       << QDateTime(QDate(2016, 01, 01), QTime(18, 00, 00));

    // Schedules for user 2 for working day 2
    QTest::newRow("6") << 2LL
                       << QDateTime(QDate(2016, 01, 02), QTime(14, 00, 00))
                       << QDateTime(QDate(2016, 01, 02), QTime(22, 00, 00));
}

void DatabaseTest::testCaseAddSchedule()
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
    QFETCH(qint64, userId);
    QFETCH(QDateTime, startTimestamp);
    QFETCH(QDateTime, endTimestamp);

    QVERIFY(ScheduleManagement::addSchedule(userId, startTimestamp, endTimestamp));
}

void DatabaseTest::testCaseAddScheduleFail_data()
{
    QTest::addColumn<qint64>("userId");
    QTest::addColumn<QDateTime>("startTimestamp");
    QTest::addColumn<QDateTime>("endTimestamp");

    // Invalid user ID
    QTest::newRow("1") << 5LL
                       << QDateTime()
                       << QDateTime(QDate(2016, 01, 01), QTime(12, 00, 00));

    // Invalid start timestamp
    QTest::newRow("2") << 1LL
                       << QDateTime()
                       << QDateTime(QDate(2016, 01, 01), QTime(12, 00, 00));

    // Invalid end timestamp
    QTest::newRow("3") << 1LL
                       << QDateTime(QDate(2016, 01, 01), QTime(8, 00, 00))
                       << QDateTime();
}

void DatabaseTest::testCaseAddScheduleFail()
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
    QFETCH(qint64, userId);
    QFETCH(QDateTime, startTimestamp);
    QFETCH(QDateTime, endTimestamp);

    QVERIFY(!ScheduleManagement::addSchedule(userId, startTimestamp, endTimestamp));
}

void DatabaseTest::testCaseRemoveSchedule()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Remove schedule (schedule for user 2 for working day 2)
    QVERIFY(ScheduleManagement::removeSchedule(6LL));
}

void DatabaseTest::testCaseReadSchedulesNonEmptyDatabase()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read schedules (all users, working day 1)
    QList<Schedule> schedules = ScheduleManagement::readSchedules(
                                    QDateTime(QDate(2016, 01, 01), QTime(7, 00, 00)),
                                    QDateTime(QDate(2016, 01, 01), QTime(22, 59, 59)));

    QCOMPARE(schedules.size(), 4);

    // User 1
    QCOMPARE(schedules[0].userId(), 1LL);
    QCOMPARE(schedules[0].startTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(8, 00, 00)));
    QCOMPARE(schedules[0].endTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(12, 00, 00)));

    QCOMPARE(schedules[1].userId(), 1LL);
    QCOMPARE(schedules[1].startTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(14, 00, 00)));
    QCOMPARE(schedules[1].endTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(18, 00, 00)));

    // User 2
    QCOMPARE(schedules[2].userId(), 2LL);
    QCOMPARE(schedules[2].startTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(8, 00, 00)));
    QCOMPARE(schedules[2].endTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(12, 00, 00)));

    QCOMPARE(schedules[3].userId(), 2LL);
    QCOMPARE(schedules[3].startTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(14, 00, 00)));
    QCOMPARE(schedules[3].endTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(18, 00, 00)));
}

void DatabaseTest::testCaseReadSchedulesUserId()
{
    using namespace OpenTimeTracker::Server;
    using namespace OpenTimeTracker::Server::Database;

    // Make sure the we are connected to the database
    if (DatabaseManagement::isConnected() == false)
    {
        QVERIFY(DatabaseManagement::connect(m_databaseFilePath));
        QCOMPARE(DatabaseManagement::isConnected(), true);
    }

    // Read schedules (user 1, working day 1)
    QList<Schedule> schedules = ScheduleManagement::readSchedules(
                                    1LL,
                                    QDateTime(QDate(2016, 01, 01), QTime(7, 00, 00)),
                                    QDateTime(QDate(2016, 01, 01), QTime(22, 59, 59)));

    QCOMPARE(schedules.size(), 2);

    QCOMPARE(schedules[0].userId(), 1LL);
    QCOMPARE(schedules[0].startTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(8, 00, 00)));
    QCOMPARE(schedules[0].endTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(12, 00, 00)));

    QCOMPARE(schedules[1].userId(), 1LL);
    QCOMPARE(schedules[1].startTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(14, 00, 00)));
    QCOMPARE(schedules[1].endTimestamp(), QDateTime(QDate(2016, 01, 01), QTime(18, 00, 00)));
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

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_DatabaseTest.moc"
