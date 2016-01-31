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
#ifndef OPENTIMETRACKER_SERVER_PACKETS_PACKETWRITER_HPP
#define OPENTIMETRACKER_SERVER_PACKETS_PACKETWRITER_HPP

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include "Packet.hpp"

namespace OpenTimeTracker
{
namespace Server
{
namespace Packets
{

/*!
 * \brief   Abstraction of a packet writer
 */
class PacketWriter
{
public:
    /*!
     * \brief   Constructor
     */
    PacketWriter();

    /*!
     * \brief   Destructor
     */
    virtual ~PacketWriter() = 0;

    /*!
     * \brief   Gets packet type that can be written by this class
     *
     * \return  Packet type
     */
    virtual QString packetType() const = 0;

    /*!
     * \brief   Converts packet object into a byte array
     *
     * \param   packet  Packet object
     *
     * \return  Packet or nullptr
     */
    QByteArray toByteArray(const Packet &packet) const;

protected:
    /*!
     * \brief   Writes the packet body to the packet JSON object
     *
     * \param       packet          Packet that whose body is being written
     * \param[out]  packetObject    Packet JSON object
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    virtual bool writeBody(const Packet &packet, QJsonObject &packetObject) const = 0;

private:
    /*!
     * \brief   Writes the packet header to the packet JSON object
     *
     * \param       packet          Packet that whose header is being written
     * \param[out]  packetObject    Packet JSON object
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool writeHeader(const Packet &packet, QJsonObject &packetObject) const;
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_PACKETS_PACKETWRITER_HPP
