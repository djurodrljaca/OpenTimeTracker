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
#include "KeepAliveResponsePacket.hpp"

using namespace OpenTimeTracker::Server::Packets;

KeepAliveResponsePacket::KeepAliveResponsePacket()
    : Packet(),
      m_referenceId(0)
{
}

KeepAliveResponsePacket::~KeepAliveResponsePacket()
{
}

QString KeepAliveResponsePacket::type() const
{
    return KeepAliveResponsePacket::staticType();
}

QString KeepAliveResponsePacket::staticType()
{
    return QStringLiteral("KeepAliveResponse");
}

quint32 KeepAliveResponsePacket::referenceId() const
{
    return m_referenceId;
}

void KeepAliveResponsePacket::setReferenceId(const quint32 &referenceIdValue)
{
    m_referenceId = referenceIdValue;
}
