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
#include "UserManagement.hpp"

using namespace OpenTimeTracker::Server;

QList<User> Database::UserManagement::readUsers()
{
    QList<User> users;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Users/ReadAll.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command
            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, QMap<QString, QVariant>(), &results))
            {
                // Get all users from the query
                for (int i = 0; i < results.size(); i++)
                {
                    User user = User::fromMap(results.at(i));

                    if (user.isValid())
                    {
                        // Add user to list
                        users.append(user);
                    }
                    else
                    {
                        // On error stop reading the results and clear them
                        users.clear();
                        break;
                    }
                }
            }
        }
    }

    return users;
}

QList<UserGroup> Database::UserManagement::readUserGroups()
{
    QList<UserGroup> userGroups;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("UserGroups/ReadAll.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command
            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, QMap<QString, QVariant>(), &results))
            {
                // Get all user groups from the query
                for (int i = 0; i < results.size(); i++)
                {
                    UserGroup userGroup = UserGroup::fromMap(results.at(i));

                    if (userGroup.isValid())
                    {
                        // Add user group to list
                        userGroups.append(userGroup);
                    }
                    else
                    {
                        // On error stop reading the results and clear them
                        userGroups.clear();
                        break;
                    }
                }
            }
        }
    }

    return userGroups;
}

QList<UserMapping> Database::UserManagement::readUserMappings()
{
    QList<UserMapping> userMappings;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("UserMapping/ReadAll.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command
            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, QMap<QString, QVariant>(), &results))
            {
                // Get all user mappings from the query
                for (int i = 0; i < results.size(); i++)
                {
                    UserMapping userMapping = UserMapping::fromMap(results.at(i));

                    if (userMapping.isValid())
                    {
                        // Add user mapping to list
                        userMappings.append(userMapping);
                    }
                    else
                    {
                        // On error stop reading the results and clear them
                        userMappings.clear();
                        break;
                    }
                }
            }
        }
    }

    return userMappings;
}

bool Database::UserManagement::addUser(const QString &name, const QString &password)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Users/Add.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            QMap<QString, QVariant> values;
            values[":name"] = name;

            if (password.isEmpty())
            {
                // User with an empty password shall be considered to be disabled
                values[":password"] = QString();
            }
            else
            {
                // User with an empty password shall be considered to be enabled
                values[":password"] = password;
            }

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command, values, NULL, &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}

bool Database::UserManagement::addUserGroup(const QString &name)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("UserGroups/Add.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            QMap<QString, QVariant> values;
            values[":name"] = name;

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command, values, NULL, &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}

bool Database::UserManagement::addUserMapping(const qint64 &userGroupId, const qint64 &userId)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("UserMapping/Add.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            QMap<QString, QVariant> values;
            values[":userGroupId"] = userGroupId;
            values[":userId"] = userId;

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command, values, NULL, &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}

bool Database::UserManagement::changeUserName(const qint64 &userId, const QString &newName)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Users/UpdateName.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            // Execute the command
            QMap<QString, QVariant> values;
            values[":id"] = userId;
            values[":name"] = newName;

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command, values, NULL, &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}

bool Database::UserManagement::changeUserPassword(const qint64 &userId, const QString &newPassword)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Users/UpdatePassword.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            // Execute the command
            QMap<QString, QVariant> values;
            values[":id"] = userId;
            values[":password"] = newPassword;

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command, values, NULL, &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}

bool Database::UserManagement::disableUser(const quint64 &userId)
{
    return changeUserPassword(userId, QString());
}

bool Database::UserManagement::changeUserGroupName(const qint64 &userGroupId,
                                                   const QString &newName)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("UserGroups/UpdateName.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            // Execute the command
            QMap<QString, QVariant> values;
            values[":id"] = userGroupId;
            values[":name"] = newName;

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command, values, NULL, &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}

bool Database::UserManagement::removeUserMapping(const qint64 &userMappingId)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("UserMapping/Remove.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            // Execute the command
            QMap<QString, QVariant> values;
            values[":id"] = userMappingId;

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command, values, NULL, &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}
