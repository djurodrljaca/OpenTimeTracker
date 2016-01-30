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
#include <QString>
#include <QtTest>
#include "../../src/TimeTracker.hpp"
#include "../../src/BreakTimeCalculator.hpp"

class TimeTrackerTest : public QObject
{
    Q_OBJECT

public:
    TimeTrackerTest();

private Q_SLOTS:
    void testCaseUserId();
    void testCaseValid();
    void testCaseInvalid();
    void testCaseNotWorking();
    void testCaseNotWorkingFail();
    void testCaseWorking();
    void testCaseWorkingFail();
    void testCaseOnBreak();
    void testCaseOnBreakFail();
    void testCaseFromBreak();
    void testCaseFromBreakFail();
    void testCaseNormalFlow();
    void testCaseNormalFlowDoubleBreak();
    void testCaseNormalFlowDoubleAll();
    void testCaseNormalFlowStartEarly();
    void testCaseNormalFlowFinishLate();
    void testCaseNormalFlowTooLittleBreak();
    void testCaseNormalFlowTooMuchBreak();
};

TimeTrackerTest::TimeTrackerTest()
{
}

void TimeTrackerTest::testCaseUserId()
{
    using namespace OpenTimeTracker::Server;

    TimeTracker timeTracker;

    const qint64 userId = 1LL;

    timeTracker.setUserId(userId);
    QCOMPARE(timeTracker.userId(), userId);
}

void TimeTrackerTest::testCaseValid()
{
    using namespace OpenTimeTracker::Server;

    TimeTracker timeTracker;

    // When working day is not started
    const qint64 userId = 1LL;
    timeTracker.setUserId(userId);
    QVERIFY(timeTracker.isValid());

    // When working day is started
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));
    QVERIFY(timeTracker.isValid());
}

void TimeTrackerTest::testCaseInvalid()
{
    using namespace OpenTimeTracker::Server;

    TimeTracker timeTracker;

    // When working day is not started: Invalid user ID
    qint64 userId = 0LL;
    timeTracker.setUserId(userId);
    QVERIFY(!timeTracker.isValid());

    // Other situations are not possible if the rest of the API works correctly!
}

void TimeTrackerTest::testCaseNotWorking()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Check state
    QCOMPARE(timeTracker.state(), TimeTracker::State_NotWorking);

    // Check working and break times
    const QDateTime currentTimestamp = startOfWorkday.addSecs(60);

    const qint32 workingTime = timeTracker.calculateWorkingTime(currentTimestamp);
    const qint32 breakTime = timeTracker.calculateBreakTime(currentTimestamp);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(currentTimestamp);

    const qint32 expectedWorkingTime = 0;
    const qint32 expectedBreakTime = 0;
    const qint32 expectedTotalWorkingTime = 0;

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseNotWorkingFail()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Check state
    QCOMPARE(timeTracker.state(), TimeTracker::State_NotWorking);

    // Check for failure
    const QDateTime timestamp = QDateTime::currentDateTime();

    QVERIFY(!timeTracker.startWorking(QDateTime()));
    QCOMPARE(timeTracker.state(), TimeTracker::State_NotWorking);

    QVERIFY(!timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), TimeTracker::State_NotWorking);

    QVERIFY(!timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), TimeTracker::State_NotWorking);

    QVERIFY(!timeTracker.stopWorking(timestamp));
    QCOMPARE(timeTracker.state(), TimeTracker::State_NotWorking);
}

void TimeTrackerTest::testCaseWorking()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working
    QVERIFY(timeTracker.startWorking(startOfWorkday));
    QCOMPARE(timeTracker.state(), TimeTracker::State_Working);

    // Check working and break times (1 h after start of workday)
    const QDateTime timestamp = startOfWorkday.addSecs(60 * 60);

    const qint32 workingTime = timeTracker.calculateWorkingTime(timestamp);
    const qint32 breakTime = timeTracker.calculateBreakTime(timestamp);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(timestamp);

    const qint32 expectedWorkingTime = 60 * 60;
    const qint32 expectedBreakTime = 0;
    const qint32 expectedTotalWorkingTime = 60 * 60;

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseWorkingFail()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working
    QVERIFY(!timeTracker.startWorking(QDateTime()));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

    QVERIFY(timeTracker.startWorking(startOfWorkday));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Check for failure
    const QDateTime timestamp = startOfWorkday;

    QVERIFY(!timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    QVERIFY(!timeTracker.startBreak(QDateTime()));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    QVERIFY(!timeTracker.startBreak(timestamp.addSecs(-1)));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    QVERIFY(!timeTracker.endBreak(timestamp.addSecs(1)));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    QVERIFY(!timeTracker.stopWorking(QDateTime()));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);
}

void TimeTrackerTest::testCaseOnBreak()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Prepare expected times
    const qint32 expectedWorkingTime = 60 * 60;
    const qint32 expectedBreakTime = static_cast<qint32>(expectedWorkingTime * 0.5 / 7.5);
    const qint32 expectedTotalWorkingTime = expectedWorkingTime + expectedBreakTime;

    // Start working
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), TimeTracker::State_Working);

    // Start break
    timestamp = timestamp.addSecs(expectedWorkingTime);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // Check working and break times
    timestamp = timestamp.addSecs(expectedBreakTime);

    const qint32 workingTime = timeTracker.calculateWorkingTime(timestamp);
    const qint32 breakTime = timeTracker.calculateBreakTime(timestamp);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(timestamp);

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseOnBreakFail()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working (1 h before start of workday)
    const qint32 offset = 60 * 60;
    QVERIFY(timeTracker.startWorking(startOfWorkday.addSecs(-offset)));
    QCOMPARE(timeTracker.state(), TimeTracker::State_Working);

    // Start break (55 min after start of workday)
    const qint32 offsetBreak = 55 * 60;
    const QDateTime timestampBreak = startOfWorkday.addSecs(offsetBreak);
    QVERIFY(timeTracker.startBreak(timestampBreak));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // Check for failure
    const QDateTime timestamp = startOfWorkday.addSecs(1);

    QVERIFY(!timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    QVERIFY(!timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    QVERIFY(!timeTracker.endBreak(QDateTime()));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    QVERIFY(!timeTracker.endBreak(timestampBreak.addSecs(-1)));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    QVERIFY(!timeTracker.stopWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);
}

void TimeTrackerTest::testCaseFromBreak()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Prepare expected times
    const qint32 workingTime1 = 60 * 60;
    const qint32 workingTime2 = 30 * 60;
    const qint32 expectedWorkingTime = workingTime1 + workingTime2;
    const qint32 expectedBreakTime = static_cast<qint32>(expectedWorkingTime * 0.5 / 7.5);
    const qint32 expectedTotalWorkingTime = expectedWorkingTime + expectedBreakTime;

    // Start working
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), TimeTracker::State_Working);

    // Start break
    timestamp = timestamp.addSecs(workingTime1);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break
    timestamp = timestamp.addSecs(expectedBreakTime);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Check working and break times
    timestamp = timestamp.addSecs(workingTime2);

    const qint32 workingTime = timeTracker.calculateWorkingTime(timestamp);
    const qint32 breakTime = timeTracker.calculateBreakTime(timestamp);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(timestamp);

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseFromBreakFail()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), TimeTracker::State_Working);

    // Start break (55 min after start of workday)
    timestamp = timestamp.addSecs(55 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (1 h after start of workday)
    timestamp = timestamp.addSecs(60 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Check for failure
    QVERIFY(!timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    QVERIFY(!timeTracker.startBreak(QDateTime()));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    QVERIFY(!timeTracker.startBreak(timestamp.addSecs(-1)));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    QVERIFY(!timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    QVERIFY(!timeTracker.stopWorking(QDateTime()));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    QVERIFY(!timeTracker.stopWorking(timestamp.addSecs(-1)));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);
}

void TimeTrackerTest::testCaseNormalFlow()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Start break (3.5 hours later)
    timestamp = timestamp.addSecs((3 * 60 + 30) * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (30 min later))
    timestamp = timestamp.addSecs(30 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Stop working (after 8 hours)
    QVERIFY(timeTracker.stopWorking(endOfWorkday));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

    // Check working and break times
    const qint32 workingTime = timeTracker.calculateWorkingTime(endOfWorkday);
    const qint32 breakTime = timeTracker.calculateBreakTime(endOfWorkday);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(endOfWorkday);

    const qint32 expectedWorkingTime = (7 * 60 + 30) * 60;
    const qint32 expectedBreakTime = 30 * 60;
    const qint32 expectedTotalWorkingTime = expectedWorkingTime + expectedBreakTime;

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseNormalFlowDoubleBreak()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Start break (3 hours later)
    timestamp = timestamp.addSecs(3 * 60 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (25 min later))
    timestamp = timestamp.addSecs(25 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Start break (2 hours later)
    timestamp = timestamp.addSecs(2 * 60 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (5 min later))
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Stop working (after 8 hours)
    QVERIFY(timeTracker.stopWorking(endOfWorkday));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

    // Check working and break times
    const qint32 workingTime = timeTracker.calculateWorkingTime(endOfWorkday);
    const qint32 breakTime = timeTracker.calculateBreakTime(endOfWorkday);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(endOfWorkday);

    const qint32 expectedWorkingTime = (7 * 60 + 30) * 60;
    const qint32 expectedBreakTime = 30 * 60;
    const qint32 expectedTotalWorkingTime = expectedWorkingTime + expectedBreakTime;

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseNormalFlowDoubleAll()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // First normal flow

    // Start working
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Start break (1 hour later)
    timestamp = timestamp.addSecs(1 * 60 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (10 min later))
    timestamp = timestamp.addSecs(10 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Stop working (1 hour later)
    timestamp = timestamp.addSecs(1 * 60 * 60);
    QVERIFY(timeTracker.stopWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

    // Second normal flow

    // Start working (1 hour later)
    timestamp = timestamp.addSecs(1 * 60 * 60);
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Start break (1 hour later)
    timestamp = timestamp.addSecs(1 * 60 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (5 min later))
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Stop working (45 min later)
    timestamp = timestamp.addSecs(45 * 60);
    QVERIFY(timeTracker.stopWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

    // Check working and break times
    const qint32 workingTime = timeTracker.calculateWorkingTime(endOfWorkday);
    const qint32 breakTime = timeTracker.calculateBreakTime(endOfWorkday);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(endOfWorkday);

    const qint32 expectedWorkingTime = (3 * 60 + 45) * 60;
    const qint32 expectedBreakTime = 15 * 60;
    const qint32 expectedTotalWorkingTime = expectedWorkingTime + expectedBreakTime;

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseNormalFlowStartEarly()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working (30 min early)
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp.addSecs(-30 * 60)));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Start break (3.5 hours later)
    timestamp = timestamp.addSecs((3 * 60 + 30) * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (30 min later))
    timestamp = timestamp.addSecs(30 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Stop working (after 8 hours)
    QVERIFY(timeTracker.stopWorking(endOfWorkday));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

    // Check working and break times
    const qint32 workingTime = timeTracker.calculateWorkingTime(endOfWorkday);
    const qint32 breakTime = timeTracker.calculateBreakTime(endOfWorkday);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(endOfWorkday);

    const qint32 expectedWorkingTime = (7 * 60 + 30) * 60;
    const qint32 expectedBreakTime = 30 * 60;
    const qint32 expectedTotalWorkingTime = expectedWorkingTime + expectedBreakTime;

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseNormalFlowFinishLate()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Start break (3.5 hours later)
    timestamp = timestamp.addSecs((3 * 60 + 30) * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (30 min later))
    timestamp = timestamp.addSecs(30 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Stop working (after 8.5 hours)
    QVERIFY(timeTracker.stopWorking(endOfWorkday.addSecs(30 * 60)));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

    // Check working and break times
    const qint32 workingTime = timeTracker.calculateWorkingTime(endOfWorkday);
    const qint32 breakTime = timeTracker.calculateBreakTime(endOfWorkday);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(endOfWorkday);

    const qint32 expectedWorkingTime = (7 * 60 + 30) * 60;
    const qint32 expectedBreakTime = 30 * 60;
    const qint32 expectedTotalWorkingTime = expectedWorkingTime + expectedBreakTime;

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseNormalFlowTooLittleBreak()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Start break (3.5 hours later)
    timestamp = timestamp.addSecs((3 * 60 + 30) * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (20 min later))
    timestamp = timestamp.addSecs(20 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Stop working (after 8 hours)
    QVERIFY(timeTracker.stopWorking(endOfWorkday));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

    // Check working and break times
    const qint32 workingTime = timeTracker.calculateWorkingTime(endOfWorkday);
    const qint32 breakTime = timeTracker.calculateBreakTime(endOfWorkday);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(endOfWorkday);

    const qint32 expectedWorkingTime = (7 * 60 + 40) * 60;
    const qint32 expectedBreakTime = 20 * 60;
    const qint32 expectedTotalWorkingTime = expectedWorkingTime + expectedBreakTime;

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

void TimeTrackerTest::testCaseNormalFlowTooMuchBreak()
{
    using namespace OpenTimeTracker::Server;

    const qint64 userId = 1LL;
    TimeTracker timeTracker;
    timeTracker.setUserId(userId);

    // Set a schedule
    BreakTimeCalculator breakTimeCalculator;
    QVERIFY(breakTimeCalculator.initialize(7.5, 0.5));

    const QDateTime startOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(8, 00, 00));
    const QDateTime endOfWorkday = QDateTime(QDate(2016, 01, 16), QTime(16, 00, 00));

    Schedule schedule;
    schedule.setId(1LL);
    schedule.setUserId(userId);
    schedule.setStartTimestamp(startOfWorkday);
    schedule.setEndTimestamp(endOfWorkday);

    QList<Schedule> scheduleList;
    scheduleList << schedule;

    QVERIFY(timeTracker.startWorkday(breakTimeCalculator, scheduleList));

    // Start working
    QDateTime timestamp = startOfWorkday;
    QVERIFY(timeTracker.startWorking(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Start break (3.5 hours later)
    timestamp = timestamp.addSecs((3 * 60 + 30) * 60);
    QVERIFY(timeTracker.startBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

    // End break (40 min later))
    timestamp = timestamp.addSecs(40 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

    // Stop working (after 8 hours)
    QVERIFY(timeTracker.stopWorking(endOfWorkday));
    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

    // Check working and break times
    const qint32 workingTime = timeTracker.calculateWorkingTime(endOfWorkday);
    const qint32 breakTime = timeTracker.calculateBreakTime(endOfWorkday);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(endOfWorkday);

    const qint32 expectedWorkingTime = (7 * 60 + 20) * 60;
    const qint32 expectedBreakTime = 40 * 60;
    const qint32 allowedBreakTime = static_cast<qint32>(expectedWorkingTime * 0.5 / 7.5);
    const qint32 expectedTotalWorkingTime = expectedWorkingTime + allowedBreakTime;

    QCOMPARE(workingTime, expectedWorkingTime);
    QCOMPARE(breakTime, expectedBreakTime);
    QCOMPARE(totalWorkingTime, expectedTotalWorkingTime);
}

QTEST_APPLESS_MAIN(TimeTrackerTest)

#include "tst_TimeTrackerTest.moc"
