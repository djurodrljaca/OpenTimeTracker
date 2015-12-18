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

using namespace OpenTimeTracker::Server;

const qint32 Database::m_version = 1;

Database::Database()
    : m_database()
{
}

Database::~Database()
{
    if (isOpen())
    {
        close();
    }
}

bool Database::isOpen()
{
    return m_database.isOpen();
}

bool Database::open(const QString &databaseFilePath)
{
    bool success = false;
    bool databaseFileAlreadyExists = false;

    // Check if the database connection needs to be opened
    if (isOpen())
    {
        // Error: database connection is already open
    }
    else
    {
        // Open the database
        if (databaseFilePath.isEmpty() == false)
        {
            // First check if the database file exists
            databaseFileAlreadyExists = QFile::exists(databaseFilePath);

            // Create the database connection and open it
            m_database = QSqlDatabase::addDatabase("QSQLITE");
            m_database.setDatabaseName(databaseFilePath);

            success = m_database.open();
        }
    }

    // Check the database version
    if (success)
    {
        const qint32 databaseVersion = readVersion(&success);

        if (success)
        {
            if (databaseVersion == m_version)
            {
                // Latest supported database version detected
                success = true;
            }
            else if (databaseVersion == 0)
            {
                // Clear the database if it already exists
                if (databaseFileAlreadyExists)
                {
                    // Close the database, delete the database file and reopen the database
                    m_database.close();

                    if (QFile::remove(databaseFilePath))
                    {
                        success = m_database.open();
                    }
                    else
                    {
                        // Error, failed to clear the database
                        success = false;
                    }
                }

                // Uninitialized database detected, initialize it
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

    return success;
}

void Database::close()
{
    if (isOpen())
    {
        m_database.close();
    }
}

bool Database::initialize()
{
    bool success = false;

    if (isOpen())
    {
        // Create table: Users
        success = createTable(QStringLiteral("Users"));

        // TODO: implement

        // Write the database version
        if (success)
        {
            success = writeVersion();
        }
    }

    return success;
}

qint32 Database::readVersion(bool *success)
{
    // Initialize output values
    qint32 version = 0;

    if (success != NULL)
    {
        *success = false;
    }

    // Read "user_version" pragma's value
    bool readingSuccessfull = false;
    const QVariant pragmaValue = readPragmaValue(QStringLiteral("user_version"),
                                                 &readingSuccessfull);

    if (readingSuccessfull && pragmaValue.isValid())
    {
        if (pragmaValue.canConvert<qint32>())
        {
            version = pragmaValue.value<qint32>();

            if (success != NULL)
            {
                *success = true;
            }
        }
    }

    return version;
}

bool Database::writeVersion()
{
    bool success = false;

    if (isOpen())
    {
        success = writePragmaValue(QStringLiteral("user_version"), m_version);
    }

    return success;
}

QVariant Database::readPragmaValue(const QString &pragmaName, bool *success)
{
    // Initialize output values
    QVariant pragmaValue;

    if (success != NULL)
    {
        *success = false;
    }

    // Read the pragma value
    if (isOpen() && (pragmaName.isEmpty() == false))
    {
        // Execute query
        QSqlQuery query(m_database);

        if (query.exec(QString("PRAGMA %1;").arg(pragmaName)))
        {
            if (query.next())
            {
                // Convert the result to an integer (if possible)
                pragmaValue = query.value(0);

                if (success != NULL)
                {
                    *success = pragmaValue.isValid();
                }
            }
        }
        else
        {
            qDebug() << "Database::readPragmaValue: failed to read pragma:" << query.lastQuery()
                     << query.lastError().text();
        }
    }

    return pragmaValue;
}

bool Database::writePragmaValue(const QString &pragmaName, const QVariant &pragmaValue)
{
    bool success = false;

    if (isOpen() && (pragmaName.isEmpty() == false) && pragmaValue.isValid())
    {
        // Execute query
        QSqlQuery query(m_database);

        success = query.exec(QString("PRAGMA %1=%2;").arg(pragmaName, pragmaValue.toString()));
    }

    return success;
}

bool Database::createTable(const QString &tableName)
{
    bool success = false;

    if (isOpen() && (tableName.isEmpty() == false))
    {
        // Read SQL command
        QFile commandFile(QString(":/Database/Tables/%1.sql").arg(tableName));

        if (commandFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString command = QString::fromUtf8(commandFile.readAll());

            // Create table with the SQL command
            QSqlQuery query(m_database);

            if (query.exec(command))
            {
                success = true;
            }
        }
        else
        {
            qDebug() << "Database::createTable: open error:" << commandFile.errorString();
        }
    }
    // TODO: implement
    return success;
}
