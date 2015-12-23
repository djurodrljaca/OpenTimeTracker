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
#include "UserMapping.hpp"

using namespace OpenTimeTracker::Server;

UserMapping::UserMapping()
    : m_id(0LL),
      m_userGroupId(0LL),
      m_userId(0LL)
{
}

UserMapping::UserMapping(const UserMapping &other)
    : m_id(other.m_id),
      m_userGroupId(other.m_userGroupId),
      m_userId(other.m_userId)
{
}

UserMapping &UserMapping::operator =(const UserMapping &other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_userGroupId = other.m_userGroupId;
        m_userId = other.m_userId;
    }

    return *this;
}

bool UserMapping::isValid() const
{
    bool valid = true;

    if ((m_id < 1LL) || (m_userGroupId < 1LL) || (m_userId < 1LL))
    {
        valid = false;
    }

    return valid;
}

qint64 UserMapping::id() const
{
    return m_id;
}

void UserMapping::setId(const qint64 &newId)
{
    m_id = newId;
}

qint64 UserMapping::userGroupId() const
{
    return m_userGroupId;
}

void UserMapping::setUserGroupId(const qint64 &newUserGroupId)
{
    m_userGroupId = newUserGroupId;
}

qint64 UserMapping::userId() const
{
    return m_userId;
}

void UserMapping::setUserId(const qint64 &newUserId)
{
    m_userId = newUserId;
}

UserMapping UserMapping::fromMap(const QMap<QString, QVariant> &map)
{
    UserMapping userMapping;

    if (map.size() == 3)
    {
        bool success = false;

        // Get user mapping ID
        QVariant value = map["id"];

        if (value.canConvert<qint64>())
        {
            userMapping.setId(value.toLongLong(&success));
        }

        // Get user group ID
        if (success)
        {
            value = map["userGroupId"];

            if (value.canConvert<qint64>())
            {
                userMapping.setUserGroupId(value.toLongLong(&success));
            }
            else
            {
                success = false;
            }
        }

        // Get user ID
        if (success)
        {
            value = map["userId"];

            if (value.canConvert<qint64>())
            {
                userMapping.setUserId(value.toLongLong(&success));
            }
            else
            {
                success = false;
            }
        }

        // On error clear the object
        if (!success)
        {
            userMapping = UserMapping();
        }
    }

    return userMapping;
}
