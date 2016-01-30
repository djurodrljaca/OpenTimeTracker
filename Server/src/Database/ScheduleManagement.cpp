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
#include "ScheduleManagement.hpp"
#include "DatabaseManagement.hpp"

using namespace OpenTimeTracker::Server;

QPair<QDateTime, QDateTime> Database::ScheduleManagement::readWorkingDay(const QDateTime &timestamp)
{
    QPair<QDateTime, QDateTime> workingDay;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("WorkingDays/Read.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command
            QMap<QString, QVariant> values;
            values[":timestamp"] = timestamp.toUTC().toString(Qt::ISODate);

            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, values, &results))
            {
                // Get working day from the query
                if (results.size() == 1)
                {
                    bool success = false;
                    const QMap<QString, QVariant> map = results.at(0);

                    // Get start timestamp
                    QVariant value = map["startTimestamp"];

                    if (value.canConvert<QString>())
                    {
                        // Since start timestamp in the database is stored in UTC it is converted to
                        // local time here
                        QDateTime startTimestamp = QDateTime::fromString(value.toString(),
                                                                         Qt::ISODate);
                        startTimestamp.setTimeSpec(Qt::UTC);
                        workingDay.first = startTimestamp;

                        success = true;
                    }

                    // Get end timestamp
                    if (success)
                    {
                        value = map["endTimestamp"];

                        if (value.canConvert<QString>())
                        {
                            // Since end timestamp in the database is stored in UTC it is converted
                            // to local time here
                            QDateTime endTimestamp = QDateTime::fromString(value.toString(),
                                                                           Qt::ISODate);
                            endTimestamp.setTimeSpec(Qt::UTC);
                            workingDay.second = endTimestamp;
                        }
                        else
                        {
                            success = false;
                        }
                    }
                }
            }
        }
    }

    return workingDay;
}

QList<Schedule> Database::ScheduleManagement::readSchedules(const qint64 &userId,
                                                            const QDateTime &startTimestamp,
                                                            const QDateTime &endTimestamp)
{
    QList<Schedule> schedules;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Schedules/ReadSingleUser.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command (timestamps in the database are stored in UTC)
            QMap<QString, QVariant> values;
            values[":userId"] = userId;
            values[":startOfWorkingDay"] = startTimestamp.toUTC().toString(Qt::ISODate);
            values[":endOfWorkingDay"] = endTimestamp.toUTC().toString(Qt::ISODate);

            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, values, &results))
            {
                // Get all schedules from the query
                for (int i = 0; i < results.size(); i++)
                {
                    Schedule schedule = Schedule::fromMap(results.at(i));

                    if (schedule.isValid())
                    {
                        // Add schedule to list
                        schedules.append(schedule);
                    }
                    else
                    {
                        // On error stop reading the results and clear them
                        schedules.clear();
                        break;
                    }
                }
            }
        }
    }

    return schedules;
}

QList<Schedule> Database::ScheduleManagement::readSchedules(const QDateTime &startTimestamp,
                                                            const QDateTime &endTimestamp)
{
    QList<Schedule> schedules;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Schedules/ReadAllUsers.sql"));

        if (command.isEmpty() == false)
        {
            // Execute SQL command (timestamps in the database are stored in UTC)
            QMap<QString, QVariant> values;
            values[":startOfWorkingDay"] = startTimestamp.toUTC().toString(Qt::ISODate);
            values[":endOfWorkingDay"] = endTimestamp.toUTC().toString(Qt::ISODate);

            QList<QMap<QString, QVariant> > results;

            if (DatabaseManagement::executeSqlCommand(command, values, &results))
            {
                // Get all schedules from the query
                for (int i = 0; i < results.size(); i++)
                {
                    Schedule schedule = Schedule::fromMap(results.at(i));

                    if (schedule.isValid())
                    {
                        // Add schedule to list
                        schedules.append(schedule);
                    }
                    else
                    {
                        // On error stop reading the results and clear them
                        schedules.clear();
                        break;
                    }
                }
            }
        }
    }

    return schedules;
}

bool Database::ScheduleManagement::addWorkingDay(const QDateTime &startTimestamp,
                                                 const QDateTime &endTimestamp)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("WorkingDays/Add.sql"));

        // Execute command
        if ((!command.isEmpty()) && startTimestamp.isValid() && endTimestamp.isValid())
        {
            QMap<QString, QVariant> values;
            values[":startTimestamp"] = startTimestamp.toUTC().toString(Qt::ISODate);
            values[":endTimestamp"] = endTimestamp.toUTC().toString(Qt::ISODate);

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command,
                                                            values,
                                                            nullptr,
                                                            &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}

bool Database::ScheduleManagement::addSchedule(const qint64 &userId,
                                               const QDateTime &startTimestamp,
                                               const QDateTime &endTimestamp)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Schedules/Add.sql"));

        // Execute command
        if ((!command.isEmpty()) && startTimestamp.isValid() && endTimestamp.isValid())
        {
            QMap<QString, QVariant> values;
            values[":userId"] = userId;
            values[":startTimestamp"] = startTimestamp.toUTC().toString(Qt::ISODate);
            values[":endTimestamp"] = endTimestamp.toUTC().toString(Qt::ISODate);

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command, values, nullptr, &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}

bool Database::ScheduleManagement::removeSchedule(const qint64 &scheduleId)
{
    bool success = false;

    if (DatabaseManagement::isConnected())
    {
        // Read command
        const QString command = DatabaseManagement::readSqlCommandFromResource(
                                    QStringLiteral("Schedules/Remove.sql"));

        // Execute command
        if (command.isEmpty() == false)
        {
            // Execute the command
            QMap<QString, QVariant> values;
            values[":id"] = scheduleId;

            int rowsAffected = -1;
            success = DatabaseManagement::executeSqlCommand(command, values, nullptr, &rowsAffected);

            if (success)
            {
                if (rowsAffected != 1)
                {
                    success = false;
                }
            }
        }
    }

    return success;
}
