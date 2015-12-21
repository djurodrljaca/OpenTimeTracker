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
#include "Database.hpp"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QRegularExpression>

using namespace OpenTimeTracker::Server;

const qint32 Database::m_version = 1;

Database::Database()
{
}

Database::~Database()
{
    if (isConnected())
    {
        disconnect();
    }
}

bool Database::isConnected()
{
    return QSqlDatabase::contains();
}

bool Database::connect(const QString &databaseFilePath)
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
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(databaseFilePath);
            success = db.open();
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
                        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                        db.setDatabaseName(databaseFilePath);
                        success = db.open();
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

void Database::disconnect()
{
    if (isConnected())
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::connectionNames().at(0));
    }
}

bool Database::addUser(const QString &name, const QString &password, const bool enabled)
{
    bool success = false;

    if (isConnected())
    {
        // Read command
        const QString command = readSqlCommandFromResource(QStringLiteral("Users/Add.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            QMap<QString, QVariant> values;
            values[":name"] = name;
            values[":password"] = password;
            values[":enabled"] = enabled;

            success = executeSqlCommand(command, values);
        }
    }

    return success;
}

QList<UserInfo> Database::readAllUsers(const bool enableState)
{
    QList<UserInfo> userList;

    if (isConnected())
    {
        // Read command
        const QString command = readSqlCommandFromResource(QStringLiteral("Users/ReadAll.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command
            QSqlQuery query;

            if (query.prepare(command))
            {
                query.bindValue(QStringLiteral(":enabled"), enableState);

                if (query.exec())
                {
                    // Get all users from the query
                    bool success = false;

                    while (query.next())
                    {
                        UserInfo user;

                        // Get user ID
                        QVariant value = query.value("id");

                        if (value.canConvert<qint64>())
                        {
                            user.setId(value.toLongLong(&success));
                        }
                        else
                        {
                            success = false;
                        }

                        // Get user name
                        if (success)
                        {
                            value = query.value("name");

                            if (value.canConvert<QString>())
                            {
                                user.setName(value.toString());
                            }
                            else
                            {
                                success = false;
                            }
                        }

                        // Get user password
                        if (success)
                        {
                            value = query.value("password");

                            if (value.canConvert<QString>())
                            {
                                user.setPassword(value.toString());
                            }
                            else
                            {
                                success = false;
                            }
                        }

                        // Get user enabled state
                        if (success)
                        {
                            value = query.value("enabled");

                            if (value.canConvert<bool>())
                            {
                                user.setEnabled(value.toBool());
                            }
                            else
                            {
                                success = false;
                            }
                        }

                        // Add user to list
                        if (success && user.isValid())
                        {
                            userList.append(user);
                        }
                    }
                }
            }
        }
    }

    return userList;
}

bool Database::initialize()
{
    bool success = false;

    if (isConnected())
    {
        // Create table: Users
        success = createTable(QStringLiteral("Users"));

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

        // TODO: implement creation of the rest of the tables

        // Write the database version
        if (success)
        {
            success = writeVersion();
        }
    }

    return success;
}

qint32 Database::readVersion()
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

bool Database::writeVersion()
{
    bool success = false;

    if (isConnected())
    {
        success = writePragmaValue(QStringLiteral("user_version"), m_version);
    }

    return success;
}

QVariant Database::readPragmaValue(const QString &name)
{
    // Initialize pragma value to an invalid value
    QVariant pragmaValue;

    // Read the pragma value
    if (isConnected() && (!name.isEmpty()))
    {
        // Execute query
        QSqlQuery query;

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

bool Database::writePragmaValue(const QString &name, const QVariant &value)
{
    bool success = false;

    if (isConnected() && (!name.isEmpty()) && value.isValid())
    {
        // Execute query
        QSqlQuery query;

        success = query.exec(QString("PRAGMA %1=%2;").arg(name, value.toString()));
    }

    return success;
}

QString Database::readSqlCommandFromResource(const QString &commandPath) const
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

bool Database::executeSqlCommand(const QString &command, const QMap<QString, QVariant> &values)
{
    bool success = false;

    if (isConnected() && (!command.isEmpty()))
    {
//        QSqlDatabase db = QSqlDatabase::database();
//        qDebug() << db.databaseName();

        // Prepare SQL command
        QSqlQuery query;

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
        }
    }

    return success;
}

bool Database::createTable(const QString &tableName)
{
    bool success = false;

    if (isConnected() && (!tableName.isEmpty()))
    {
        // Read command
        const QString command = readSqlCommandFromResource(tableName +
                                                           QStringLiteral("/CreateTable.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            success = executeSqlCommand(command);
        }
    }

    return success;
}
