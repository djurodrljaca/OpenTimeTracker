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

#ifndef USERINFO_HPP
#define USERINFO_HPP

#include <QtCore/QString>

class UserInfo
{
public:
    UserInfo();

    qint64 id() const;
    void setId(const qint64 &id);

    QString name() const;
    void setName(const QString &name);

    QString password() const;
    void setPassword(const QString &password);

private:
    qint64 m_id;
    QString m_name;
    QString m_password;
};

#endif // USERINFO_HPP
