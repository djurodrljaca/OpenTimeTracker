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
#ifndef OPENTIMETRACKER_SERVER_DATABASE_DATABASEMANAGEMENT_HPP
#define OPENTIMETRACKER_SERVER_DATABASE_DATABASEMANAGEMENT_HPP

#include <QtSql/QSqlDatabase>
#include <QtCore/QVariant>

namespace OpenTimeTracker
{
namespace Server
{
namespace Database
{

/*!
 * \brief   Manages access to the database
 *
 * \todo    Create some backup procedure for the database file? On open?
 */
class DatabaseManagement
{
public:
    /*!
     * \brief   Checks if connected to a database
     *
     * \retval  true    Connected
     * \retval  false   Disconnected
     */
    static bool isConnected();

    /*!
     * \brief   Connects to the database
     *
     * \param   databaseFilePath    Path to the database file
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool connect(const QString &databaseFilePath);

    /*!
     * \brief   Disconnects the database
     */
    static void disconnect();

    /*!
     * \brief   Begins a transaction on the database
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool beginTransaction();

    /*!
     * \brief   Commits the transaction on the database
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool commitTransaction();

    /*!
     * \brief   Rolls back the transaction on the database
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool rollbackTransaction();

    /*!
     * \brief   Reads SQL command from built-in resources
     *
     * \param   commandPath     Relative path to the to the SQL command resource
     *
     * \return  Command text
     *
     * The SQL command resources are located in path ":/Database/<Relative Path>".
     */
    static QString readSqlCommandFromResource(const QString &commandPath);

    /*!
     * \brief   Reads SQL commands from built-in resources
     *
     * \param   commandPath     Relative path to the to the SQL command resource
     *
     * \return  List of command texts
     *
     * The SQL command resources are located in path ":/Database/<Relative Path>".
     */
    static QStringList readSqlCommandsFromResource(const QString &commandPath);

    /*!
     * \brief   Executes SQL command
     *
     * \param   commands        SQL command
     * \param   values          List of values that can be bound to the command
     * \param   results         Optional parameter for results of the executed command
     * \param   rowsAffected    Optional parameter for number of affected rows of the executed
     *                          command
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Only SELECT statements can produce results
     */
    static bool executeSqlCommand(const QString &command,
                                  const QMap<QString, QVariant> &values = QMap<QString, QVariant>(),
                                  QList<QMap<QString, QVariant> > *results = NULL,
                                  int *rowsAffected = NULL);

private:
    /*!
     * \brief   Constructor is disabled
     */
    DatabaseManagement();

    /*!
     * \brief   Adds the database connection
     *
     * \return  Database object
     */
    static QSqlDatabase addDatabase();

    /*!
     * \brief   Gets the database object
     *
     * \return  Database object
     */
    static QSqlDatabase database();

    /*!
     * \brief   Initialize all needed pragmas
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool initializePragmas();

    /*!
     * \brief   Initializes the database
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Initialization is only executed when a database with version 0 is opened. It will
     *          first clear the database and then initialize it.
     */
    static bool initialize();

    /*!
     * \brief   Reads the database's version
     *
     * \return  Database's version
     *
     * \note    If the database version is less than "1" then the database is invalid. The database
     *          file should be deleted and the database reinitialized.
     */
    static qint32 readVersion();

    /*!
     * \brief   Writes the database version to the database
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool writeVersion();

    /*!
     * \brief   Reads a pragma's value
     *
     * \param   name    Name of the pragma
     *
     * \return  Pragma's value
     */
    static QVariant readPragmaValue(const QString &name);

    /*!
     * \brief   Reads a pragma's value
     *
     * \param   name    Name of the pragma
     * \param   value   Value to write to the pragma
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool writePragmaValue(const QString &name, const QVariant &value);

    /*!
     * \brief   Creates a table
     *
     * \param   tableName   Name of the table to create
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool createTable(const QString &tableName);

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

    /*!
     * \brief   Holds the database's connection name
     */
    static const QString m_connectionName;
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_DATABASE_DATABASEMANAGEMENT_HPP
