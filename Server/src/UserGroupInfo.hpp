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
#ifndef OPENTIMETRACKER_SERVER_USERGROUPINFO_HPP
#define OPENTIMETRACKER_SERVER_USERGROUPINFO_HPP

#include <QtCore/QString>

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Holds user group's information
 */
class UserGroupInfo
{
public:
    /*!
     * \brief   Constructor
     */
    UserGroupInfo();

    /*!
     * \brief   Copy constructor
     * \param   other   Object to be copied
     */
    UserGroupInfo(const UserGroupInfo &other);

    /*!
     * \brief   operator =
     * \param   other   Object to be copied
     *
     * \return  Reference to the this object
     */
    UserGroupInfo &operator =(const UserGroupInfo &other);

    /*!
     * \brief   Checks if object is valid
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    bool isValid() const;

    /*!
     * \brief   Gets user group's ID
     *
     * \return  User group's ID
     */
    qint64 id() const;

    /*!
     * \brief   Sets user group's new ID
     *
     * \param   newId   User group's new ID
     */
    void setId(const qint64 &newId);

    /*!
     * \brief   Gets user group's name
     *
     * \return  User group's name
     */
    QString name() const;

    /*!
     * \brief   Sets user group's new name
     *
     * \param   newName     User group's new name
     */
    void setName(const QString &newName);

    /*!
     * \brief   Checks if user group is enabled in the system
     *
     * \retval  true    Enabled
     * \retval  false   Not enabled
     */
    bool isEnabled() const;

    /*!
     * \brief   Sets enable state of the user in the system
     *
     * \param   enabled     New enable state
     */
    void setEnabled(bool enabled);

private:
    /*!
     * \brief   Holds the user group's ID
     */
    qint64 m_id;

    /*!
     * \brief   Holds the user group's name
     */
    QString m_name;

    /*!
     * \brief   Holds the flag if user group is enabled in the system
     */
    bool m_enabled;
};

}
}

#endif // OPENTIMETRACKER_SERVER_USERGROUPINFO_HPP