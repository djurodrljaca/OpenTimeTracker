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
#include "UserMappingInfo.hpp"

using namespace OpenTimeTracker::Server;

UserMappingInfo::UserMappingInfo()
    : m_id(-1LL),
      m_userGroupId(-1LL),
      m_userId(-1LL)
{
}

UserMappingInfo::UserMappingInfo(const UserMappingInfo &other)
    : m_id(other.m_id),
      m_userGroupId(other.m_userGroupId),
      m_userId(other.m_userId)
{
}

UserMappingInfo &UserMappingInfo::operator =(const UserMappingInfo &other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_userGroupId = other.m_userGroupId;
        m_userId = other.m_userId;
    }

    return *this;
}

bool UserMappingInfo::isValid() const
{
    bool valid = true;

    if ((m_id < 0) || (m_userGroupId < 0) || (m_userId < 0))
    {
        valid = false;
    }

    return valid;
}

qint64 UserMappingInfo::userGroupId() const
{
    return m_userGroupId;
}

void UserMappingInfo::setUserGroupId(const qint64 &newUserGroupId)
{
    m_userGroupId = newUserGroupId;
}

qint64 UserMappingInfo::userId() const
{
    return m_userId;
}

void UserMappingInfo::setUserId(const qint64 &newUserId)
{
    m_userId = newUserId;
}
