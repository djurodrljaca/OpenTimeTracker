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
#include "User.hpp"
#include <cstdint>

using namespace OpenTimeTracker::Server;

User::User()
    : m_id(0LL),
      m_name(),
      m_password()
{
}

User::User(const User &other)
    : m_id(other.m_id),
      m_name(other.m_name),
      m_password(other.m_password)
{
}

User &User::operator =(const User &other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_name = other.m_name;
        m_password = other.m_password;
    }

    return *this;
}

bool User::isValid() const
{
    bool valid = true;

    if ((m_id < 1LL) ||
        m_name.isEmpty() ||
        ((m_password.isNull() == false) && m_password.isEmpty()))
    {
        valid = false;
    }

    return valid;
}

qint64 User::id() const
{
    return m_id;
}

void User::setId(const qint64 &newId)
{
    m_id = newId;
}

QString User::name() const
{
    return m_name;
}

void User::setName(const QString &newName)
{
    m_name = newName;
}

QString User::password() const
{
    return m_password;
}

void User::setPassword(const QString &newPassword)
{
    m_password = newPassword;
}

User User::fromMap(const QMap<QString, QVariant> &map)
{
    User user;

    if (map.size() == 3)
    {
        bool success = false;

        // Get user ID
        QVariant value = map["id"];

        if (value.canConvert<qint64>())
        {
            user.setId(value.toLongLong(&success));
        }

        // Get user name
        if (success)
        {
            value = map["name"];

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
            value = map["password"];

            if (value.canConvert<QString>())
            {
                user.setPassword(value.toString());
            }
            else
            {
                success = false;
            }
        }

        // On error clear the object
        if (!success)
        {
            user = User();
        }
    }

    return user;
}
