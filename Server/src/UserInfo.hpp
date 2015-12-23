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
#ifndef OPENTIMETRACKER_SERVER_USERINFO_HPP
#define OPENTIMETRACKER_SERVER_USERINFO_HPP

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>

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
     */
    QString password() const;

    /*!
     * \brief   Sets user's new password
     *
     * \param   newPassword     User's new password
     */
    void setPassword(const QString &newPassword);

    /*!
     * \brief   Creates an object from a map
     *
     * \param   map     Map that contains the user values
     *
     * \return  A new object
     *
     * \note    Created object is invalid if the values in the map cannot be used to create a valid
     *          object.
     */
    static UserInfo fromMap(const QMap<QString, QVariant> &map);

private:
    /*!
     * \brief   Holds the user's ID
     */
    qint64 m_id;

    /*!
     * \brief   Holds the user's name
     */
    QString m_name;

    /*!
     * \brief   Holds the user's password
     */
    QString m_password;
};

}
}

#endif // OPENTIMETRACKER_SERVER_USERINFO_HPP
