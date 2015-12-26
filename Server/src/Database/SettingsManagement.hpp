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
#ifndef OPENTIMETRACKER_SERVER_DATABASE_SETTINGSMANAGEMENT_HPP
#define OPENTIMETRACKER_SERVER_DATABASE_SETTINGSMANAGEMENT_HPP

#include <QtCore/QMap>
#include <QtCore/QVariant>

namespace OpenTimeTracker
{
namespace Server
{
namespace Database
{

/*!
 * \brief   Manages access to the settings in the database
 */
class SettingsManagement
{
public:
    /*!
     * \brief   Reads all settings from the database
     *
     * \return  Settings
     */
    static QMap<QString, QVariant> readSettings();

    /*!
     * \brief   Add settings to the database
     *
     * \param   settings    Settings to add to the database
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool addSettings(const QMap<QString, QVariant> &settings);

    /*!
     * \brief   Changes a setting in the database
     *
     * \param   name        Name of the setting
     * \param   newValue    New value of the setting
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static bool changeSetting(const QString &name, const QVariant &newValue);

private:
    /*!
     * \brief   Constructor is disabled
     */
    SettingsManagement();
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_DATABASE_SETTINGSMANAGEMENT_HPP
