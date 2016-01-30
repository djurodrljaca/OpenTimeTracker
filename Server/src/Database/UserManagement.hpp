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
#ifndef OPENTIMETRACKER_SERVER_DATABASE_USERMANAGEMENT_HPP
#define OPENTIMETRACKER_SERVER_DATABASE_USERMANAGEMENT_HPP

#include "../User.hpp"
#include "../UserGroup.hpp"
#include "../UserMapping.hpp"

namespace OpenTimeTracker
{
namespace Server
{
namespace Database
{

/*!
 * \brief   Manages access to the users, user groups and user mappings in the database
 */
class UserManagement
{
public:
    /*!
     * \brief   Reads all users from the database
     *
     * \return  List of users
     */
    static QList<User> readUsers();

    /*!
     * \brief   Reads all user groups from the database
     *
     * \return  List of user groups
     */
    static QList<UserGroup> readUserGroups();

    /*!
     * \brief   Reads all user mappings from the database
     *
     * \return  List of user mappings
     */
    static QList<UserMapping> readUserMappings();

    /*!
     * \brief   Adds a new user to the database
     *
     * \param   name        User's name
     * \param   password    User's password
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    A user with an empty password shall be considered to be disabled!
     */
    static bool addUser(const QString &name, const QString &password);

    /*!
     * \brief   Adds a new user group to the database
     *
     * \param   name    User group's name
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool addUserGroup(const QString &name);

    /*!
     * \brief   Adds a new user mapping to the database
     *
     * \param   userGroupId User group's ID to map to a user
     * \param   userId      User's ID to map to a user group
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool addUserMapping(const qint64 &userGroupId, const qint64 &userId);

    /*!
     * \brief   Changes a user name in the database
     *
     * \param   userId  ID of the user
     * \param   newName New user's name
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool changeUserName(const qint64 &userId, const QString &newName);

    /*!
     * \brief   Changes a user password in the database
     *
     * \param   userId      ID of the user
     * \param   newPassword New user's password
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    A user with an empty password is considered to be disabled!
     */
    static bool changeUserPassword(const qint64 &userId, const QString &newPassword);

    /*!
     * \brief   Disables a user in the database
     *
     * \param   userId  ID of the user
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * A user in the database is disabled by setting its password to "null".
     *
     * This method just calls UserManagement::changeUserPassword() with a "null" string as the new
     * password.
     *
     * To re-enable a user in the database, just change its password to a non-empty value.
     */
    static bool disableUser(const quint64 &userId);

    /*!
     * \brief   Changes a user group name in the database
     *
     * \param   userGroupId ID of the user group
     * \param   newName     New user group's name
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool changeUserGroupName(const qint64 &userGroupId, const QString &newName);

    /*!
     * \brief   Removes user mapping from the database
     *
     * \param   userMappingId   ID of the user mapping
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool removeUserMapping(const qint64 &userMappingId);

private:
    /*!
     * \brief   Constructor is disabled
     */
    UserManagement();
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_DATABASE_USERMANAGEMENT_HPP
