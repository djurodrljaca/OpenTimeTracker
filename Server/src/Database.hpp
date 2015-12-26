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
#ifndef OPENTIMETRACKER_SERVER_DATABASE_HPP
#define OPENTIMETRACKER_SERVER_DATABASE_HPP

#include <QtSql/QSqlDatabase>
#include <QtCore/QVariant>
#include "Event.hpp"
#include "EventChangeLogItem.hpp"
#include "User.hpp"
#include "UserGroup.hpp"
#include "UserMapping.hpp"

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Manages access to the database
 *
 * \todo    Create some backup procedure for the database file? On open?
 */
class Database
{
public:
    /*!
     * \brief   Constructor
     */
    Database();

    /*!
     * \brief   Destructor
     */
    ~Database();

    /*!
     * \brief   Checks if connected to a database
     *
     * \retval  true    Connected
     * \retval  false   Disconnected
     */
    bool isConnected();

    /*!
     * \brief   Connects to the database
     *
     * \param   databaseFilePath    Path to the database file
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool connect(const QString &databaseFilePath);

    /*!
     * \brief   Disconnects the database
     */
    void disconnect();

    /*!
     * \brief   Writes a setting to the system
     *
     * \param   name    Name of the setting
     * \param   value   Value of the setting
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * Setting is inserted if it doesn't exist, but if it exists it is updated if necessary
     */
    bool writeSetting(const QString &name, const QVariant &value);
    // TODO: split into "add" and "change"?

    /*!
     * \brief   Reads all settings from the system
     *
     * \return  Settings
     */
    QMap<QString, QVariant> readAllSettings();

    /*!
     * \brief   Adds a new user to the system
     *
     * \param   name        User's name
     * \param   password    User's password
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool addUser(const QString &name, const QString &password);
    // TODO: implement changeUserName();
    // TODO: implement changeUserPassword();
    // TODO: implement enableUser();
    // TODO: implement disableUser();

    /*!
     * \brief   Reads all users from the system
     *
     * \return  List of users
     */
    QList<User> readAllUsers();

    /*!
     * \brief   Adds a new user group to the system
     *
     * \param   name    User group's name
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool addUserGroup(const QString &name);
    // TODO: implement changeUserGroupName();

    /*!
     * \brief   Reads all user groups from the system
     *
     * \return  List of user groups
     */
    QList<UserGroup> readAllUserGroups();

    /*!
     * \brief   Adds a new user mapping to the system
     *
     * \param   userGroupId User group's ID to map to a user
     * \param   userId      User's ID to map to a user group
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool addUserMapping(const qint64 &userGroupId, const qint64 &userId);

    /*!
     * \brief   Reads all user mappings from the system
     *
     * \return  List of user mappings
     */
    QList<UserMapping> readAllUserMappings();

    /*!
     * \brief   Adds a new event to the system
     *
     * \param   timestamp   Event's timestamp
     * \param   userId      Event's user ID
     * \param   type        Event's type
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    All new events are by default enabled
     */
    bool addEvent(const QDateTime &timestamp, const qint64 &userId, const Event::Type type);

    /*!
     * \brief   Reads an specific event from the system
     *
     * \param   eventId     Event's ID
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    Event readEvent(const qint64 &eventId);

    /*!
     * \brief   Reads events from the system for a specific time range and user
     *
     * \param   startTimestamp  Read events from and including this timestamp
     * \param   endTimestamp    Read events up to and including this timestamp
     * \param   userId          Read events for the selected user
     *
     * \return  List of events
     */
    QList<Event> readEvents(const QDateTime &startTimestamp,
                            const QDateTime &endTimestamp,
                            const qint64 &userId);

    /*!
     * \brief   Changes the timestamp in an event in the system
     *
     * \param   eventId         ID of the event to change
     * \param   newTimestamp    New timestamp
     * \param   userId          ID of the user that requested the change
     * \param   comment         Explanation why the change was requested
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Comment must not be empty
     */
    bool changeEventTimestamp(const qint64 &eventId,
                              const QDateTime &newTimestamp,
                              const qint64 &userId,
                              const QString &comment);

    /*!
     * \brief   Changes the type in an event in the system
     *
     * \param   eventId     ID of the event to change
     * \param   newType     New type
     * \param   userId      ID of the user that requested the change
     * \param   comment     Explanation why the change was requested
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Comment must not be empty
     */
    bool changeEventType(const qint64 &eventId,
                         const Event::Type &newType,
                         const qint64 &userId,
                         const QString &comment);

    /*!
     * \brief   Changes the enable state in an event in the system
     *
     * \param   eventId     ID of the event to change
     * \param   enable      New enable state
     * \param   userId      ID of the user that requested the change
     * \param   comment     Explanation why the change was requested
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Comment must not be empty
     */
    bool changeEventEnableState(const qint64 &eventId,
                                const bool enable,
                                const qint64 &userId,
                                const QString &comment);

    /*!
     * \brief   Reads event change log from the system for a specific event
     *
     * \param   eventId     Read event change log for the selected event
     *
     * \return  List of event change log items
     */
    QList<EventChangeLogItem> readEventChangeLog(const qint64 &eventId);

private:
    /*!
     * \brief   Initialize all needed pragmas
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool initializePragmas();

    /*!
     * \brief   Initializes the database
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Initialization is only executed when a database with version 0 is opened. It will
     *          first clear the database and then initialize it.
     */
    bool initialize();

    /*!
     * \brief   Reads the database's version
     *
     * \return  Database's version
     *
     * \note    If the database version is less than "1" then the database is invalid. The database
     *          file should be deleted and the database reinitialized.
     */
    qint32 readVersion();

    /*!
     * \brief   Writes the database version to the database
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool writeVersion();

    /*!
     * \brief   Reads a pragma's value
     *
     * \param   name    Name of the pragma
     *
     * \return  Pragma's value
     */
    QVariant readPragmaValue(const QString &name);

    /*!
     * \brief   Reads a pragma's value
     *
     * \param   name    Name of the pragma
     * \param   value   Value to write to the pragma
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool writePragmaValue(const QString &name, const QVariant &value);

    /*!
     * \brief   Reads SQL command from built-in resources
     *
     * \param   commandPath     Relative path to the to the SQL command resource
     *
     * \return  Command text
     *
     * The SQL command resources are located in path ":/Database/<Relative Path>".
     */
    QString readSqlCommandFromResource(const QString &commandPath) const;

    /*!
     * \brief   Reads SQL commands from built-in resources
     *
     * \param   commandPath     Relative path to the to the SQL command resource
     *
     * \return  List of command texts
     *
     * The SQL command resources are located in path ":/Database/<Relative Path>".
     */
    QStringList readSqlCommandsFromResource(const QString &commandPath) const;

    /*!
     * \brief   Executes SQL command
     *
     * \param   commands    SQL command
     * \param   values      List of values that can be bound to the command
     * \param   results     Optional results of the executed command
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Only SELECT statements can produce results
     */
    bool executeSqlCommand(const QString &command,
                           const QMap<QString, QVariant> &values = QMap<QString, QVariant>(),
                           QList<QMap<QString, QVariant> > *results = NULL);

    /*!
     * \brief   Creates a table
     *
     * \param   tableName   Name of the table to create
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool createTable(const QString &tableName);

    /*!
     * \brief   Holds the database's version
     *
     * This class is able to work with a specific version of the database. In case the database
     * version is smaller than the supported version an attempt to upgrade the database should be
     * made.
     *
     * \note    Database version is incremented every time an incompatibility in the database schema
     *          is introduced.
     */
    static const qint32 m_version;

    // TODO: split into multiple classes
};

}
}

#endif // OPENTIMETRACKER_SERVER_DATABASE_HPP
