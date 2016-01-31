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
#include <QtCore/QList>
#include "Packets/Packet.hpp"
#include "Packets/PacketReader.hpp"
#include "Packets/PacketWriter.hpp"

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
     * \brief   Destructor
     */
    ~PacketHandler();

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
    QByteArray toByteArray(const Packets::Packet &packet);

    /*!
     * \brief   Registers the packet reader
     *
     * \param   reader  Packet reader
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool registerPacketReader(Packets::PacketReader *reader);

    /*!
     * \brief   Registers the packet writer
     *
     * \param   writer  Packet writer
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    bool registerPacketWriter(Packets::PacketWriter *writer);

    /*!
     * \brief   Creates a packet ID
     *
     * \return  Packet ID
     *
     * This method shall be used for generation of packet IDs. Every time this method is called a
     * new packet ID is generated.
     */
    static quint32 createPacketId();

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

    /*!
     * \brief   Holds registered packet readers
     */
    QList<Packets::PacketReader *> m_registeredPacketReaders;

    /*!
     * \brief   Holds registered packet writers
     */
    QList<Packets::PacketWriter *> m_registeredPacketWriters;

    /*!
     * \brief   Holds the next packet ID
     */
    static quint32 m_nextPacketId;
};

}
}

#endif // OPENTIMETRACKER_SERVER_PACKETHANDLER_HPP
