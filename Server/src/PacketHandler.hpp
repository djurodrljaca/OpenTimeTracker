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
#ifndef OPENTIMETRACKER_SERVER_PACKETHANDLER_HPP
#define OPENTIMETRACKER_SERVER_PACKETHANDLER_HPP

#include <QtCore/QByteArray>
#include <QtCore/QScopedPointer>
#include "Packets/Packet.hpp"

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Enables reading of a packet object from a byte array and writing a packet object to a
 *          byte array
 *
 * A packet object is a native representation of a packet contained in a JSON document.
 *
 * Each valid packet must contain at least these parameters:
 * - type
 * - ID
 *
 * An example of a JSON document that contains a valid packet:
 * \code{.unparsed}
 *  {
 *      "type": "KeepAliveRequest",
 *      "id": 12345
 *  }
 * \endcode
 */
class PacketHandler
{
public:
    /*!
     * \brief   Enumerates the parsing result
     */
    enum Result
    {
        Result_Success,         /*!< Successfully parsed a packet */
        Result_NeedMoreData,    /*!< More data is needed */
        Result_Error            /*!< Error occurred */
    };

    /*!
     * \brief   Constructor
     */
    PacketHandler();

    /*!
     * \brief   Adds data to the data buffer
     *
     * \param   data    New data to add to the data buffer
     */
    void addData(const QByteArray &data);

    /*!
     * \brief   Reads the data buffer and tries to extract a packet from it
     *
     * \return  Result_Success      A packet was successfully read
     * \return  Result_NeedMoreData More data is needed to be able to read the packet
     * \return  Result_Error        Reading of a packet failed
     */
    Result read();

    /*!
     * \brief   Takes the last parsed packet and returns it
     *
     * \return  Parsed packet or nullptr
     */
    Packets::Packet *takePacket();

    /*!
     * \brief   Converts the packet to a byte array
     *
     * \param   packet  Packet
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static QByteArray toByteArray(const Packets::Packet &packet);

private:
    /*!
     * \brief   Converts the packet payload into a packet object
     *
     * \param   packetPayload   Packet payload
     *
     * \return  Packet object or nullptr
     */
    Packets::Packet *fromPacketPayload(const QByteArray &packetPayload) const;

    /*!
     * \brief   Holds the raw packet data buffer
     */
    QByteArray m_dataBuffer;

    /*!
     * \brief   Holds the read packet
     */
    QScopedPointer<Packets::Packet> m_readPacket;

    // TODO: create packet ID generator (static data members + static methods)
};

}
}

#endif // OPENTIMETRACKER_SERVER_PACKETHANDLER_HPP
