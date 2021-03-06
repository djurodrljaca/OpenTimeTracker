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
#include "SettingsManagement.hpp"
#include "DatabaseManagement.hpp"

using namespace OpenTimeTracker::Server;

QMap<QString, QVariant> Database::SettingsManagement::readSettings()
{
    QMap<QString, QVariant> settings;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Settings/ReadAll.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command
            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, QMap<QString, QVariant>(), &results))
            {
                // Get all settings from the query
                for (int i = 0; i < results.size(); i++)
                {
                    const QMap<QString, QVariant> &item = results.at(i);

                    if (item.contains("name") && item.contains("value"))
                    {
                        // Get setting
                        const QString name = item["name"].toString();
                        settings[name] = item["value"];
                    }
                    else
                    {
                        // Error, item doesn't contain all of the needed information
                        settings.clear();
                        break;
                    }
                }
            }
        }
    }

    return settings;
}

bool Database::SettingsManagement::addSettings(const QMap<QString, QVariant> &settings)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Settings/Add.sql"));

        // Execute command for each setting
        if (command.isEmpty() == false)
        {
            // Begin transaction
            if(DatabaseManagement::beginTransaction())
            {
                success = true;

                foreach (const QString &name, settings.keys())
                {
                    // Execute the command
                    QMap<QString, QVariant> values;
                    values[":name"] = name;
                    values[":value"] = settings[name];

                    int rowsAffected = -1;
                    success = DatabaseManagement::executeSqlCommand(command,
                                                                    values,
                                                                    nullptr,
                                                                    &rowsAffected);

                    if (success)
                    {
                        if (rowsAffected != 1)
                        {
                            success = false;
                        }
                    }

                    if (!success)
                    {
                        break;
                    }
                }

                // Finish the transaction
                if (success)
                {
                    // No error occurred, commit the transaction
                    success = DatabaseManagement::commitTransaction();
                }
                else
                {
                    // On error rollback the transaction
                    DatabaseManagement::rollbackTransaction();
                }
            }
        }
    }

    return success;
}

bool Database::SettingsManagement::changeSetting(const QString &name, const QVariant &newValue)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Settings/Update.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            // Execute the command
            QMap<QString, QVariant> values;
            values[":name"] = name;
            values[":value"] = newValue;

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command,
                                                            values,
                                                            nullptr,
                                                            &rowsAffected);

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
