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
#ifndef OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVERESPONSEPACKET_HPP
#define OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVERESPONSEPACKET_HPP

#include "Packet.hpp"

namespace OpenTimeTracker
{
namespace Server
{
namespace Packets
{

/*!
 * \brief   Packet: Keep Alive Response
 */
class KeepAliveResponsePacket : public Packet
{
public:
    /*!
     * \brief   Constructor
     */
    KeepAliveResponsePacket();

    /*!
     * \brief   Destructor
     */
    virtual ~KeepAliveResponsePacket();

    /*!
     * \copydoc Packet::type
     */
    virtual QString type() const;

    /*!
     * \copydoc Packet::type
     */
    static QString staticType();

    /*!
     * \brief   Gets the reference ID
     *
     * \return  Reference ID
     */
    quint32 referenceId() const;

    /*!
     * \brief   Sets reference ID
     *
     * \param   referenceIdValue    Reference ID value
     */
    void setReferenceId(const quint32 &referenceIdValue);

private:
    /*!
     * \brief   Holds the reference packet ID of the request packet
     */
    quint32 m_referenceId;
};

}
}
}

#endif // OPENTIMETRACKER_SERVER_PACKETS_KEEPALIVERESPONSEPACKET_HPP
