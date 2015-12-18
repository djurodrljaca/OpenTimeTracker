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

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Holds user's information
 */
class UserInfo
{
public:
    /*!
     * \brief   Constructor
     */
    UserInfo();

    /*!
     * \brief   Copy constructor
     * \param   other   Object to be copied
     */
    UserInfo(const UserInfo &other);

    /*!
     * \brief   operator =
     * \param   other   Object to be copied
     *
     * \return  Reference to the this object
     */
    UserInfo &operator =(const UserInfo &other);

    /*!
     * \brief   Checks if object is valid
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    bool isValid() const;

    /*!
     * \brief   Gets user's ID
     *
     * \return  User's ID
     */
    qint64 id() const;

    /*!
     * \brief   Sets user's new ID
     *
     * \param   newId   User's new ID
     */
    void setId(const qint64 &newId);

    /*!
     * \brief   Gets user's name
     *
     * \return  User's name
     */
    QString name() const;

    /*!
     * \brief   Sets user's new name
     *
     * \param   newName     User's new name
     */
    void setName(const QString &newName);

    /*!
     * \brief   Gets user's password
     *
     * \return  User's password
     *
     * The password is only
     */
    QString password() const;

    /*!
     * \brief   Sets user's new password
     *
     * \param   password    User's new password
     */
    void setPassword(const QString &password);

private:
    qint64 m_id;
    QString m_name;
    QString m_password;
};

}

#endif // USERINFO_HPP
