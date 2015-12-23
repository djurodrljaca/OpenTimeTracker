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
#include "UserGroupInfo.hpp"

using namespace OpenTimeTracker::Server;

UserGroupInfo::UserGroupInfo()
    : m_id(0LL),
      m_name()
{
}

UserGroupInfo::UserGroupInfo(const UserGroupInfo &other)
    : m_id(other.m_id),
      m_name(other.m_name)
{
}

UserGroupInfo &UserGroupInfo::operator =(const UserGroupInfo &other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_name = other.m_name;
    }

    return *this;
}

bool UserGroupInfo::isValid() const
{
    bool valid = true;

    if ((m_id < 1LL) || m_name.isEmpty())
    {
        valid = false;
    }

    return valid;
}

qint64 UserGroupInfo::id() const
{
    return m_id;
}

void UserGroupInfo::setId(const qint64 &newId)
{
    m_id = newId;
}

QString UserGroupInfo::name() const
{
    return m_name;
}

void UserGroupInfo::setName(const QString &newName)
{
    m_name = newName;
}

UserGroupInfo UserGroupInfo::fromMap(const QMap<QString, QVariant> &map)
{
    UserGroupInfo userGroup;

    if (map.size() == 2)
    {
        bool success = false;

        // Get user group ID
        QVariant value = map["id"];

        if (value.canConvert<qint64>())
        {
            userGroup.setId(value.toLongLong(&success));
        }

        // Get user group name
        if (success)
        {
            value = map["name"];

            if (value.canConvert<QString>())
            {
                userGroup.setName(value.toString());
            }
            else
            {
                success = false;
            }
        }

        // On error clear the object
        if (!success)
        {
            userGroup = UserGroupInfo();
        }
    }

    return userGroup;
}
