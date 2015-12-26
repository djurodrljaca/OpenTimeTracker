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
#include "DatabaseManagement.hpp"
#include "EventManagement.hpp"

using namespace OpenTimeTracker::Server;

QList<Event> Database::EventManagement::readEvents(const QDateTime &startTimestamp,
                                                   const QDateTime &endTimestamp,
                                                   const qint64 &userId)
{
    QList<Event> events;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Events/ReadTimeRange.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command (timestamps in the database are stored in UTC)
            QMap<QString, QVariant> values;
            values[":startTimestamp"] = startTimestamp.toUTC().toString(Qt::ISODate);
            values[":endTimestamp"] = endTimestamp.toUTC().toString(Qt::ISODate);
            values[":userId"] = userId;

            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, values, &results))
            {
                // Get all events from the query
                for (int i = 0; i < results.size(); i++)
                {
                    Event event = Event::fromMap(results.at(i));

                    if (event.isValid())
                    {
                        // Add event to list
                        events.append(event);
                    }
                    else
                    {
                        // On error stop reading the results and clear them
                        events.clear();
                        break;
                    }
                }
            }
        }
    }

    return events;
}

QList<EventChangeLogItem> Database::EventManagement::readEventChangeLog(const qint64 &eventId)
{
    QList<EventChangeLogItem> eventChangeLog;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("EventChangeLog/ReadAll.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command
            QMap<QString, QVariant> values;
            values[":eventId"] = eventId;

            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, values, &results))
            {
                // Get all event change log items from the query
                for (int i = 0; i < results.size(); i++)
                {
                    EventChangeLogItem item = EventChangeLogItem::fromMap(results.at(i));

                    if (item.isValid())
                    {
                        // Add item to list
                        eventChangeLog.append(item);
                    }
                    else
                    {
                        // On error stop reading the results and clear them
                        eventChangeLog.clear();
                        break;
                    }
                }
            }
        }
    }

    return eventChangeLog;
}

bool Database::EventManagement::addEvent(const QDateTime &timestamp,
                                         const qint64 &userId,
                                         const Event::Type type)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Events/Add.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            // Store the timestamp in UTC in the database
            QMap<QString, QVariant> values;
            values[":timestamp"] = timestamp.toUTC().toString(Qt::ISODate);
            values[":userId"] = userId;
            values[":type"] = static_cast<int>(type);
            values[":enabled"] = 1;

            // Execute the command
            success = DatabaseManagement::executeSqlCommand(command, values);
        }
    }

    return success;
}

bool Database::EventManagement::changeEventTimestamp(const qint64 &eventId,
                                                     const QDateTime &newTimestamp,
                                                     const qint64 &userId,
                                                     const QString &comment)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Begin transaction
        if(DatabaseManagement::beginTransaction())
        {
            // Read event
            Event event = readEvent(eventId);

            // Change event
            if (event.isValid())
            {
                // Read command
                const QString command = DatabaseManagement::readSqlCommandFromResource(
                                            QStringLiteral("Events/UpdateTimestamp.sql"));

                if (command.isEmpty() == false)
                {
                    // Execute SQL command
                    QMap<QString, QVariant> values;
                    values[":timestamp"] = newTimestamp.toUTC().toString(Qt::ISODate);
                    values[":id"] = event.id();

                    success = DatabaseManagement::executeSqlCommand(command, values);
                }
            }

            // Insert event change log item
            if (success)
            {
                // Read command
                const QString command = DatabaseManagement::readSqlCommandFromResource(
                                            QStringLiteral("EventChangeLog/Add.sql"));

                if (command.isEmpty() == false)
                {
                    // Execute SQL command
                    QMap<QString, QVariant> values;
                    values[":eventId"] = event.id();
                    values[":timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
                    values[":fieldName"] = QStringLiteral("timestamp");
                    values[":fromValue"] = event.timestamp().toUTC().toString(Qt::ISODate);
                    values[":toValue"] = newTimestamp.toUTC().toString(Qt::ISODate);
                    values[":userId"] = userId;
                    values[":comment"] = comment;

                    success = DatabaseManagement::executeSqlCommand(command, values);
                }
            }

            // Finish the transaction
            if (success)
            {
                // No error occurred, commit the transaction
                success = DatabaseManagement::commitTransaction();
            }
            else
            {
                // On error rollback the transaction
                DatabaseManagement::rollbackTransaction();
            }
        }
    }

    return success;
}

bool Database::EventManagement::changeEventType(const qint64 &eventId,
                                                const Event::Type &newType,
                                                const qint64 &userId,
                                                const QString &comment)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Begin transaction
        if(DatabaseManagement::beginTransaction())
        {
            // Read event
            Event event = readEvent(eventId);

            // Change event
            if (event.isValid())
            {
                // Read command
                const QString command = DatabaseManagement::readSqlCommandFromResource(
                                            QStringLiteral("Events/UpdateType.sql"));

                if (command.isEmpty() == false)
                {
                    // Execute SQL command
                    QMap<QString, QVariant> values;
                    values[":type"] = static_cast<int>(newType);
                    values[":id"] = event.id();

                    success = DatabaseManagement::executeSqlCommand(command, values);
                }
            }

            // Insert event change log item
            if (success)
            {
                // Read command
                const QString command = DatabaseManagement::readSqlCommandFromResource(
                                            QStringLiteral("EventChangeLog/Add.sql"));

                if (command.isEmpty() == false)
                {
                    // Execute SQL command
                    QMap<QString, QVariant> values;
                    values[":eventId"] = event.id();
                    values[":timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
                    values[":fieldName"] = QStringLiteral("type");
                    values[":fromValue"] = static_cast<int>(event.type());
                    values[":toValue"] = static_cast<int>(newType);
                    values[":userId"] = userId;
                    values[":comment"] = comment;

                    success = DatabaseManagement::executeSqlCommand(command, values);
                }
            }

            // Finish the transaction
            if (success)
            {
                // No error occurred, commit the transaction
                success = DatabaseManagement::commitTransaction();
            }
            else
            {
                // On error rollback the transaction
                DatabaseManagement::rollbackTransaction();
            }
        }
    }

    return success;
}

bool Database::EventManagement::changeEventEnableState(const qint64 &eventId,
                                                      const bool newEnableState,
                                                      const qint64 &userId,
                                                      const QString &comment)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Begin transaction
        if(DatabaseManagement::beginTransaction())
        {
            // Read event
            Event event = readEvent(eventId);

            // Change event
            if (event.isValid())
            {
                // Read command
                const QString command = DatabaseManagement::readSqlCommandFromResource(
                                            QStringLiteral("Events/UpdateEnabled.sql"));

                if (command.isEmpty() == false)
                {
                    // Execute SQL command
                    QMap<QString, QVariant> values;
                    values[":enabled"] = newEnableState;
                    values[":id"] = event.id();

                    success = DatabaseManagement::executeSqlCommand(command, values);
                }
            }

            // Insert event change log item
            if (success)
            {
                // Read command
                const QString command = DatabaseManagement::readSqlCommandFromResource(
                                            QStringLiteral("EventChangeLog/Add.sql"));

                if (command.isEmpty() == false)
                {
                    // Execute SQL command
                    QMap<QString, QVariant> values;
                    values[":eventId"] = event.id();
                    values[":timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
                    values[":fieldName"] = QStringLiteral("enabled");
                    values[":fromValue"] = event.isEnabled();
                    values[":toValue"] = newEnableState;
                    values[":userId"] = userId;
                    values[":comment"] = comment;

                    success = DatabaseManagement::executeSqlCommand(command, values);
                }
            }

            // Finish the transaction
            if (success)
            {
                // No error occurred, commit the transaction
                success = DatabaseManagement::commitTransaction();
            }
            else
            {
                // On error rollback the transaction
                DatabaseManagement::rollbackTransaction();
            }
        }
    }

    return success;
}

Event Database::EventManagement::readEvent(const qint64 &eventId)
{
    Event event;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Events/ReadSingle.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command
            QMap<QString, QVariant> values;
            values[":id"] = eventId;

            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, values, &results))
            {
                // Get event from the query
                event = Event::fromMap(results.at(0));
            }
        }
    }

    return event;
}
