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
#ifndef OPENTIMETRACKER_SERVER_USERMAPPINGINFO_HPP
#define OPENTIMETRACKER_SERVER_USERMAPPINGINFO_HPP

#include <QtCore/QtGlobal>

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Holds information for mapping users to user groups
 */
class UserMappingInfo
{
public:
    /*!
     * \brief   Constructor
     */
    UserMappingInfo();

    /*!
     * \brief   Copy constructor
     * \param   other   Object to be copied
     */
    UserMappingInfo(const UserMappingInfo &other);

    /*!
     * \brief   operator =
     * \param   other   Object to be copied
     *
     * \return  Reference to the this object
     */
    UserMappingInfo &operator =(const UserMappingInfo &other);

    /*!
     * \brief   Checks if object is valid
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    bool isValid() const;

    /*!
     * \brief   Gets user mapping ID
     *
     * \return  User mapping ID
     */
    qint64 id() const;

    /*!
     * \brief   Sets user mapping new ID
     *
     * \param   newId   User mapping new ID
     */
    void setId(const qint64 &newId);

    /*!
     * \brief   Gets user group ID
     *
     * \return  User group ID
     */
    qint64 userGroupId() const;

    /*!
     * \brief   Sets new user group ID
     *
     * \param   newUserGroupId  New user group ID
     */
    void setUserGroupId(const qint64 &newUserGroupId);

    /*!
     * \brief   Gets user ID
     *
     * \return  User ID
     */
    qint64 userId() const;

    /*!
     * \brief   Sets new user ID
     *
     * \param   newGroupId  New user ID
     */
    void setUserId(const qint64 &newUserId);

private:
    /*!
     * \brief   Holds the user mapping ID
     */
    qint64 m_id;

    /*!
     * \brief   Holds the user group ID
     */
    qint64 m_userGroupId;

    /*!
     * \brief   Holds the user ID
     */
    qint64 m_userId;
};

}
}

#endif // OPENTIMETRACKER_SERVER_USERMAPPINGINFO_HPP
