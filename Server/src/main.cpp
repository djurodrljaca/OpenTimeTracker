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
#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>
#include "Database/DatabaseManagement.hpp"
#include "Database/SettingsManagement.hpp"
#include "Server.hpp"

int main(int argc, char *argv[])
{
    using namespace OpenTimeTracker::Server;

    QCoreApplication app(argc, argv);

    Server server(&app);

    // Open database
    bool success;

    if (!Database::DatabaseManagement::isConnected())
    {
        success = Database::DatabaseManagement::connect("database.db");
    }
    else
    {
        success = true;
    }

    // Read connection settings
    QMap<QString, QVariant> settings;

    if (success)
    {
        settings = Database::SettingsManagement::readSettings();

        // Add default settings to the database if necessary
        if (settings.isEmpty())
        {
            settings["port"] = 61234U;

            success = Database::SettingsManagement::addSettings(settings);
        }
    }

    // Start server
    if (success)
    {
        success = false;

        if (settings.contains("port"))
        {
            const quint32 valuePort = settings["port"].toUInt(&success);

            if (success && (valuePort <= UINT16_MAX))
            {
                const quint16 port = static_cast<quint16>(valuePort);
                success = server.start(port);
            }
        }
    }

    return app.exec();
}
