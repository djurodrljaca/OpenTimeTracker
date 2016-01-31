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
#ifndef OPENTIMETRACKER_SERVER_PACKETPARSER_HPP
#define OPENTIMETRACKER_SERVER_PACKETPARSER_HPP

#include <QtCore/QByteArray>
#include <QtCore/QString>

namespace OpenTimeTracker
{
namespace Server
{

/*!
 * \brief   Reads raw packet data and parses it. It can also convert a packet into raw packet data.
 */
class PacketParser
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
    PacketParser();

    /*!
     * \brief   Clears the parser
     */
    void clear();

    /*!
     * \brief   Adds data to the raw packet data buffer
     *
     * \param   data    New data to add to the raw packet data buffer
     */
    void addData(const QByteArray &data);

    /*!
     * \brief   Parses the raw packet data buffer for a valid packet
     *
     * \return  Result_Success      A packet was successfully parsed and it is available
     * \return  Result_NeedMoreData More data is needed to be able to parse the packet
     * \return  Result_Error        Parsing failed
     */
    Result parse();

    /*!
     * \brief   Takes the last parsed packet payload and returns it
     *
     * \return  Parsed packet payload
     *
     * \note    After this method is called the parsed packet payload is cleared
     */
    QString takePacketPayload();

    /*!
     * \brief   Converts packet payload to raw packet data
     *
     * \param   packetPayload   Packet payload
     *
     * \retval  true    Success
     * \retval  false   Error
     */
    static QByteArray toRawPacketData(const QString &packetPayload);

private:
    /*!
     * \brief   Holds the raw packet data buffer
     */
    QByteArray m_packetDataBuffer;

    /*!
     * \brief   Holds parsed packet payload
     */
    QString m_parsedPacketPayload;
};

}
}

#endif // OPENTIMETRACKER_SERVER_PACKETPARSER_HPP
