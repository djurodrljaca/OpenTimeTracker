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
#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QtSql/QSqlDatabase>
#include <QtCore/QVariant>
#include "Event.hpp"
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
     * \brief   Adds a new user to the system
     *
     * \param   name        User's name
     * \param   password    User's password
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool addUser(const QString &name, const QString &password);

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
     * \brief   Reads all events from the system
     *
     * \return  List of events
     */
    QList<Event> readAllEvents();
    // TODO: add search parameters? (time range, user ID)

    /*!
     * \brief   Changes a value in an event in the system
     *
     * \param   eventId     ID of the event to change
     * \param   fieldName   Name of the field to change
     * \param   newValue    New value
     * \param   userId      ID of the user that requested the change
     * \param   comment     Explanation why the change was requested
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Comment must not be empty
     */
    bool changeEvent(const qint64 &eventId,
                     const QString &fieldName,
                     const QVariant &newValue,
                     const qint64 &userId,
                     const QString &comment);

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
};

}
}

#endif // DATABASE_HPP
