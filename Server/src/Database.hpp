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
#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QtSql/QSqlDatabase>
#include <QtCore/QVariant>

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Manages access to the database
 *
 * \todo    Create some backup procedure for the database file? On open?
 */
class Database
{
public:
    /*!
     * \brief   Constructor
     */
    Database();

    /*!
     * \brief   Destructor
     */
    ~Database();

    /*!
     * \brief   Checks if database is open
     *
     * \retval  true    Database is open
     * \retval  false   Database is not open
     */
    bool isOpen();

    /*!
     * \brief   Opens the database
     *
     * \param   databaseFilePath    Path to the database file
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool open(const QString &databaseFilePath);

    /*!
     * \brief   Closes the database
     */
    void close();

private:
    /*!
     * \brief   Initializes the database
     *
     * \retval  true    Success
     * \retval  false   Error
     *
     * \note    Initialization is only executed when a database with version 0 is opened. It will
     *          first clear the database and then initialize it.
     */
    bool initialize();

    /*!
     * \brief   Reads the database's version
     *
     * \param[out]  success     Optional execution status information
     *
     * \return  Database's version
     */
    qint32 readVersion(bool *success = NULL);

    /*!
     * \brief   Writes the database version to the database
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool writeVersion();

    /*!
     * \brief   Reads a pragma's value
     *
     * \param       pragmaName  Name of the pragma
     * \param[out]  success     Optional execution status information
     *
     * \return  Pragma's value
     */
    QVariant readPragmaValue(const QString &pragmaName, bool *success = NULL);

    /*!
     * \brief   Reads a pragma's value
     *
     * \param   pragmaName  Name of the pragma
     * \param   pragmaValue Value to write to the pragma
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool writePragmaValue(const QString &pragmaName, const QVariant &pragmaValue);

    /*!
     * \brief   Creates a table
     *
     * \param   tableName   Name of the table to create
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool createTable(const QString &tableName);

    /*!
     * \brief   Holds the database connection
     */
    QSqlDatabase m_database;

    /*!
     * \brief   Holds the database's version
     *
     * This class is able to work with a specific version of the database. In case the database
     * version is smaller than the supported version an attempt to upgrade the database should be
     * made.
     *
     * \note    Database version is incremented every time an incompatibility in the database schema
     *          is introduced.
     */
    static const qint32 m_version;
};

}
}

#endif // DATABASE_HPP
