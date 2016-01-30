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
#ifndef OPENTIMETRACKER_CLIENT_CLIENT_HPP
#define OPENTIMETRACKER_CLIENT_CLIENT_HPP

#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>

namespace OpenTimeTracker
{
namespace Client
{

/*!
 * \brief   Client object for communication with the Open Time Tracker Server
 */
class Client : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief   Constructor
     *
     * \param   parent  Pointer to the parent object
     */
    explicit Client(QObject *parent = 0);

signals:

private slots:
    /*!
     * \brief   Handles the situation when the socket is disconnected
     */
    //void handleDisconnect();

private:
    /*!
     * \brief   Holds the TCP socket
     */
    QTcpSocket *m_socket;
};

}
}

#endif // OPENTIMETRACKER_CLIENT_CLIENT_HPP
