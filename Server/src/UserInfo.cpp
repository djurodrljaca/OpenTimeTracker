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
#include "UserInfo.hpp"
#include <cstdint>

using namespace OpenTimeTracker::Server;

UserInfo::UserInfo()
    : m_id(-1LL),
      m_name(),
      m_password()
{
}

UserInfo::UserInfo(const UserInfo &other)
    : m_id(other.m_id),
      m_name(other.m_name),
      m_password(other.m_password)
{
}

UserInfo &UserInfo::operator =(const UserInfo &other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_name = other.m_name;
        m_password = other.m_password;
    }

    return *this;
}

bool UserInfo::isValid() const
{
    bool valid = true;

    if ((m_id < 0) ||
        m_name.isEmpty() ||
        (m_password.isEmpty() && m_enabled) ||
        (!m_password.isEmpty() && !m_enabled))
    {
        valid = false;
    }

    return valid;
}

qint64 UserInfo::id() const
{
    return m_id;
}

void UserInfo::setId(const qint64 &newId)
{
    m_id = newId;
}

QString UserInfo::name() const
{
    return m_name;
}

void UserInfo::setName(const QString &newName)
{
    m_name = newName;
}

QString UserInfo::password() const
{
    return m_password;
}

void UserInfo::setPassword(const QString &newPassword)
{
    m_password = newPassword;
}

bool UserInfo::isEnabled() const
{
    return m_enabled;
}

void UserInfo::setEnabled(bool enabled)
{
    m_enabled = enabled;
}
