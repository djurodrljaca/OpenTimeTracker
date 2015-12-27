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
#include "DatabaseManagement.hpp"
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtDebug>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QRegularExpression>

using namespace OpenTimeTracker::Server::Database;

const qint32 DatabaseManagement::m_version = 1;
const QString DatabaseManagement::m_connectionName("OpenTimeTracker::Server");

bool DatabaseManagement::isConnected()
{
    return QSqlDatabase::contains(m_connectionName);
}

bool DatabaseManagement::connect(const QString &databaseFilePath)
{
    bool success = false;

    // Check if already connected
    if (isConnected())
    {
        // Error, already connected
    }
    else if (databaseFilePath.isEmpty())
    {
        // Error, invalid database file path
    }
    else
    {
        // First check if the database file already exists
        const bool databseFileExists = QFile::exists(databaseFilePath);

        // Connect to database (if it doesn't exist it will be created)
        {
            QSqlDatabase db = addDatabase();
            db.setDatabaseName(databaseFilePath);
            success = db.open();

            // Initialize all pragmas
            if (success)
            {
                success = initializePragmas();
            }
        }

        if (success)
        {
            if (!databseFileExists)
            {
                // Database did not exist, initialize it
                success = initialize();
            }
            else
            {
                // Database already existed, check its version
                const qint32 databaseVersion = readVersion();

                if (databaseVersion == m_version)
                {
                    // Latest supported database version detected
                    success = true;
                }
                else if (databaseVersion < 1)
                {
                    // Invalid version detected
                    // Disconnect from the database
                    disconnect();

                    // Remove the database file
                    if (QFile::remove(databaseFilePath))
                    {
                        QSqlDatabase db = addDatabase();
                        db.setDatabaseName(databaseFilePath);
                        success = db.open();

                        // Initialize all pragmas
                        if (success)
                        {
                            success = initializePragmas();
                        }
                    }
                    else
                    {
                        // Error, failed to clear the database
                        success = false;
                    }

                    // Reinitialize the database
                    if (success)
                    {
                        success = initialize();
                    }
                }
                else
                {
                    // Error, unsupported version
                }
            }
        }

        // In case of error disconnect from the database
        if (!success)
        {
            disconnect();
        }
    }

    return success;
}

void DatabaseManagement::disconnect()
{
    if (isConnected())
    {
        QSqlDatabase::removeDatabase(m_connectionName);
    }
}

bool DatabaseManagement::beginTransaction()
{
    bool success = false;

    if (isConnected())
    {
        success = database().transaction();
    }

    return success;
}

bool DatabaseManagement::commitTransaction()
{
    bool success = false;

    if (isConnected())
    {
        success = database().commit();
    }

    return success;
}

bool DatabaseManagement::rollbackTransaction()
{
    bool success = false;

    if (isConnected())
    {
        success = database().rollback();
    }

    return success;
}

QString DatabaseManagement::readSqlCommandFromResource(const QString &commandPath)
{
    QString command;

    if (commandPath.isEmpty() == false)
    {
        // Read SQL command
        QFile file(QStringLiteral(":/Database/") + commandPath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // Read the command text from the file
            command = QString::fromUtf8(file.readAll());
        }
    }

    return command;
}

QStringList DatabaseManagement::readSqlCommandsFromResource(const QString &commandPath)
{
    QStringList commands;

    if (commandPath.isEmpty() == false)
    {
        // Read SQL commands
        QFile file(QStringLiteral(":/Database/") + commandPath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // Read the command text from the file
            commands = QString::fromUtf8(file.readAll()).split(QRegularExpression("\\s*;\\s*"),
                                                               QString::SkipEmptyParts);
        }
    }

    return commands;
}

bool DatabaseManagement::executeSqlCommand(const QString &command,
                                           const QMap<QString, QVariant> &values,
                                           QList<QMap<QString, QVariant> > *results,
                                           int *rowsAffected)
{
    bool success = false;

    if (isConnected() && (!command.isEmpty()))
    {
        // Prepare SQL command
        QSqlQuery query(database());

        if (query.prepare(command))
        {
            // Bind all needed values
            success = true;
            const QMap<QString, QVariant> boundValues = query.boundValues();

            foreach (const QString &key, boundValues.keys())
            {
                if (values.contains(key))
                {
                    // Bind value
                    query.bindValue(key, values[key]);
                }
                else
                {
                    // Error, missing value
                    success = false;
                    break;
                }
            }
        }

        // Execute SQL command
        if (success)
        {
            success = query.exec();

            // Optionally get results
            if (results != NULL)
            {
                // Read the results
                const QSqlRecord record = query.record();
                results->clear();

                while (query.next())
                {
                    QMap<QString, QVariant> result;

                    for (int i = 0; i < record.count(); i++)
                    {
                        // Add values to the the result for the selected column
                        result[record.fieldName(i)] = query.value(i);
                    }

                    results->append(result);
                }
            }

            // Optionally get affected rows
            if (rowsAffected != NULL)
            {
                *rowsAffected = query.numRowsAffected();
            }
        }
    }

    return success;
}

QSqlDatabase DatabaseManagement::addDatabase()
{
    return QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
}

QSqlDatabase DatabaseManagement::database()
{
    return QSqlDatabase::database(m_connectionName);
}

bool DatabaseManagement::initializePragmas()
{
    bool success = false;

    // Enable support for foreign keys
    success = writePragmaValue("foreign_keys", 1);

    return success;
}

bool DatabaseManagement::initialize()
{
    bool success = false;

    if (isConnected())
    {
        // Create table: Settings
        success = createTable(QStringLiteral("Settings"));

        // Create table: WorkingDays
        if (success)
        {
            success = createTable(QStringLiteral("WorkingDays"));
        }

        // Create table: Users
        if (success)
        {
            success = createTable(QStringLiteral("Users"));
        }

        // Create table: UserGroups
        if (success)
        {
            success = createTable(QStringLiteral("UserGroups"));
        }

        // Create table: UserMapping
        if (success)
        {
            success = createTable(QStringLiteral("UserMapping"));
        }

        // Create table: Events
        if (success)
        {
            success = createTable(QStringLiteral("Events"));
        }

        // Create table: EventChangeLog
        if (success)
        {
            success = createTable(QStringLiteral("EventChangeLog"));
        }

        // TODO: implement creation of the rest of the tables

        // Write the database version
        if (success)
        {
            success = writeVersion();
        }
    }

    return success;
}

qint32 DatabaseManagement::readVersion()
{
    // Initialize the version to an invalid value
    qint32 version = 0;

    // Read "user_version" pragma's value
    if(isConnected())
    {
        const QVariant pragmaValue = readPragmaValue(QStringLiteral("user_version"));

        if (pragmaValue.isValid())
        {
            if (pragmaValue.canConvert<qint32>())
            {
                version = pragmaValue.value<qint32>();
            }
        }
    }

    return version;
}

bool DatabaseManagement::writeVersion()
{
    bool success = false;

    if (isConnected())
    {
        success = writePragmaValue(QStringLiteral("user_version"), m_version);
    }

    return success;
}

QVariant DatabaseManagement::readPragmaValue(const QString &name)
{
    // Initialize pragma value to an invalid value
    QVariant pragmaValue;

    // Read the pragma value
    if (isConnected() && (!name.isEmpty()))
    {
        // Execute query
        QSqlQuery query(database());

        if (query.exec(QString("PRAGMA %1;").arg(name)))
        {
            if (query.next())
            {
                // Read the pragma value
                pragmaValue = query.value(0);
            }
        }
    }

    return pragmaValue;
}

bool DatabaseManagement::writePragmaValue(const QString &name, const QVariant &value)
{
    bool success = false;

    if (isConnected() && (!name.isEmpty()) && value.isValid())
    {
        // Execute query
        QSqlQuery query(database());

        success = query.exec(QString("PRAGMA %1=%2;").arg(name, value.toString()));
    }

    return success;
}

bool DatabaseManagement::createTable(const QString &tableName)
{
    bool success = false;

    if (isConnected() && (!tableName.isEmpty()))
    {
        // Read command
        const QStringList commands = readSqlCommandsFromResource(
                                         tableName + QStringLiteral("/CreateTable.sql"));

        // Execute command
        if (commands.isEmpty() == false)
        {
            foreach (const QString &command, commands)
            {
                success = executeSqlCommand(command);

                if (!success)
                {
                    break;
                }
            }
        }
    }

    return success;
}
