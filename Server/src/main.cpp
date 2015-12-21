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
#include <QCoreApplication>
#include <QtDebug>
#include "Database.hpp"

using namespace OpenTimeTracker::Server;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Database database;
    bool success = database.connect("test.db");

    if (success)
    {
        success = database.addUser("user1", "11");
    }

    if (success)
    {
        success = database.addUser("user2", "22");
    }

    QList<UserInfo> users;
    if (success)
    {
        users = database.readAllUsers();
    }

    return a.exec();
}
