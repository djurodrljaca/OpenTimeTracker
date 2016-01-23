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
//    void testCaseOnBreak();
//    void testCaseOnBreakFail();
//    void testCaseFromBreak();
//    void testCaseFromBreakFail();
//    void testCaseNormalFlow();
//    void testCaseNormalFlowDoubleBreak();
//    void testCaseNormalFlowDoubleAll();
//    void testCaseResetState();
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

    timeTracker.setUserId(1LL);
    QVERIFY(timeTracker.isValid());

    // TODO: check also other parameters?
}

void TimeTrackerTest::testCaseInvalid()
{
    using namespace OpenTimeTracker::Server;

    TimeTracker timeTracker;

    timeTracker.setUserId(0LL);
    QVERIFY(!timeTracker.isValid());
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
    QVERIFY(!timeTracker.startBreak(timestamp));
    QVERIFY(!timeTracker.endBreak(timestamp));
    QVERIFY(!timeTracker.stopWorking(timestamp));
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

    // Start working (1 h before start of workday)
    const qint32 offset = 60 * 60;
    QVERIFY(timeTracker.startWorking(startOfWorkday.addSecs(-offset)));
    QCOMPARE(timeTracker.state(), TimeTracker::State_Working);

    // Check working and break times (1 h after start of workday)
    const QDateTime currentTimestamp = startOfWorkday.addSecs(offset);

    const qint32 workingTime = timeTracker.calculateWorkingTime(currentTimestamp);
    const qint32 breakTime = timeTracker.calculateBreakTime(currentTimestamp);
    const qint32 totalWorkingTime = timeTracker.calculateTotalWorkingTime(currentTimestamp);

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
    QVERIFY(!timeTracker.startBreak(QDateTime()));
    QVERIFY(!timeTracker.startBreak(timestamp.addSecs(-1)));
    QVERIFY(!timeTracker.endBreak(timestamp.addSecs(1)));
    QVERIFY(!timeTracker.stopWorking(QDateTime()));

    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);
}

//void TimeTrackerTest::testCaseOnBreak()
//{
//    OpenTimeTracker::Server::TimeTracker timeTracker;
//    timeTracker.setUserId(1LL);

//    const QDateTime currentTimestamp = QDateTime::currentDateTime();

//    // Start working (10 min before current time)
//    const qint32 offsetWorking = 10 * 60;
//    const QDateTime timestampWorking = currentTimestamp.addSecs(-offsetWorking);
//    QVERIFY(timeTracker.startWorking(timestampWorking));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break (5 min before current time)
//    const qint32 offsetBreak = 5 * 60;
//    const QDateTime timestampBreak = currentTimestamp.addSecs(-offsetBreak);
//    QVERIFY(timeTracker.startBreak(timestampBreak));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // Check working and break times
//    const qint32 workingTime = timeTracker.workingTime();
//    const qint32 breakTime = timeTracker.breakTime();
//    const qint32 breakTimeMax = timestampBreak.secsTo(QDateTime::currentDateTime());

//    QCOMPARE(workingTime, (offsetWorking - offsetBreak));
//    QVERIFY(offsetBreak <= breakTime);
//    QVERIFY(breakTime <= breakTimeMax);
//}

//void TimeTrackerTest::testCaseOnBreakFail()
//{
//    OpenTimeTracker::Server::TimeTracker timeTracker;
//    timeTracker.setUserId(1LL);

//    const QDateTime timestamp = QDateTime::currentDateTime();

//    // Start working (2 min before current time)
//    const QDateTime timestampWorking = timestamp.addSecs(-120);
//    QVERIFY(timeTracker.startWorking(timestampWorking));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break (1 min before current time)
//    const QDateTime timestampBreak = timestamp.addSecs(-60);
//    QVERIFY(timeTracker.startBreak(timestampBreak));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // Check for failure
//    QVERIFY(!timeTracker.startWorking(timestamp));
//    QVERIFY(!timeTracker.startBreak(timestamp));
//    QVERIFY(!timeTracker.endBreak(QDateTime()));
//    QVERIFY(!timeTracker.endBreak(timestampBreak.addSecs(-1)));
//    QVERIFY(!timeTracker.stopWorking(timestamp));

//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);
//}

//void TimeTrackerTest::testCaseFromBreak()
//{
//    OpenTimeTracker::Server::TimeTracker timeTracker;
//    timeTracker.setUserId(1LL);

//    const QDateTime currentTimestamp = QDateTime::currentDateTime();

//    // Start working (15 min before current time)
//    const qint32 offsetWorking = 15 * 60;
//    const QDateTime timestampWorking = currentTimestamp.addSecs(-offsetWorking);
//    QVERIFY(timeTracker.startWorking(timestampWorking));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break (10 min before current time)
//    const qint32 offsetOnBreak = 10 * 60;
//    const QDateTime timestampOnBreak = currentTimestamp.addSecs(-offsetOnBreak);
//    QVERIFY(timeTracker.startBreak(timestampOnBreak));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // End break (5 min before current time)
//    const qint32 offsetFromBreak = 5 * 60;
//    const QDateTime timestampFromBreak = currentTimestamp.addSecs(-offsetFromBreak);
//    QVERIFY(timeTracker.endBreak(timestampFromBreak));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Check working and break times
//    const qint32 workingTime = timeTracker.workingTime();
//    const qint32 breakTime = timeTracker.breakTime();
//    const qint32 workingTimeMin = offsetWorking - offsetOnBreak;
//    const qint32 workingTimeMax = workingTimeMin +
//                                  timestampFromBreak.secsTo(QDateTime::currentDateTime());

//    QVERIFY(workingTimeMin <= workingTime);
//    QVERIFY(workingTime <= workingTimeMax);
//    QCOMPARE(breakTime, (offsetOnBreak - offsetFromBreak));
//}

//void TimeTrackerTest::testCaseFromBreakFail()
//{
//    OpenTimeTracker::Server::TimeTracker timeTracker;
//    timeTracker.setUserId(1LL);

//    const QDateTime timestamp = QDateTime::currentDateTime();

//    // Start working (3 min before current time)
//    const QDateTime timestampWorking = timestamp.addSecs(-180);
//    QVERIFY(timeTracker.startWorking(timestampWorking));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break (2 min before current time)
//    const QDateTime timestampOnBreak = timestamp.addSecs(-120);
//    QVERIFY(timeTracker.startBreak(timestampOnBreak));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // End break (1 min before current time)
//    const QDateTime timestampFromBreak = timestamp.addSecs(-60);
//    QVERIFY(timeTracker.endBreak(timestampFromBreak));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Check for failure
//    QVERIFY(!timeTracker.startWorking(timestamp));
//    QVERIFY(!timeTracker.startBreak(QDateTime()));
//    QVERIFY(!timeTracker.startBreak(timestampFromBreak.addSecs(-1)));
//    QVERIFY(!timeTracker.endBreak(timestamp));
//    QVERIFY(!timeTracker.stopWorking(QDateTime()));
//    QVERIFY(!timeTracker.stopWorking(timestampFromBreak.addSecs(-1)));

//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);
//}

//void TimeTrackerTest::testCaseNormalFlow()
//{
//    OpenTimeTracker::Server::TimeTracker timeTracker;
//    timeTracker.setUserId(1LL);

//    // Start working
//    QDateTime timestamp = QDateTime::currentDateTime();
//    QVERIFY(timeTracker.startWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.startBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // End break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.endBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Stop working 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.stopWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

//    // Check cumulative times
//    QCOMPARE(timeTracker.workingTime(), 10 * 60);
//    QCOMPARE(timeTracker.breakTime(), 5 * 60);
//}

//void TimeTrackerTest::testCaseNormalFlowDoubleBreak()
//{
//    OpenTimeTracker::Server::TimeTracker timeTracker;
//    timeTracker.setUserId(1LL);

//    // Start working
//    QDateTime timestamp = QDateTime::currentDateTime();
//    QVERIFY(timeTracker.startWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.startBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // End break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.endBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.startBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // End break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.endBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Stop working 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.stopWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

//    // Check cumulative times
//    QCOMPARE(timeTracker.workingTime(), 15 * 60);
//    QCOMPARE(timeTracker.breakTime(), 10 * 60);
//}

//void TimeTrackerTest::testCaseNormalFlowDoubleAll()
//{
//    OpenTimeTracker::Server::TimeTracker timeTracker;
//    timeTracker.setUserId(1LL);

//    // First normal flow

//    // Start working
//    QDateTime timestamp = QDateTime::currentDateTime();
//    QVERIFY(timeTracker.startWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.startBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // End break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.endBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Stop working 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.stopWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

//    // Second normal flow

//    // Start working 30 min later
//    timestamp = timestamp.addSecs(30 * 60);
//    QVERIFY(timeTracker.startWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.startBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // End break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.endBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Stop working 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.stopWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

//    // Check cumulative times
//    QCOMPARE(timeTracker.workingTime(), 20 * 60);
//    QCOMPARE(timeTracker.breakTime(), 10 * 60);
//}

//void TimeTrackerTest::testCaseResetState()
//{
//    OpenTimeTracker::Server::TimeTracker timeTracker;
//    timeTracker.setUserId(1LL);

//    // Start working
//    QDateTime timestamp = QDateTime::currentDateTime();
//    QVERIFY(timeTracker.startWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Start break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.startBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_OnBreak);

//    // End break 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.endBreak(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Stop working 5 min later
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.stopWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);

//    // Check cumulative times
//    QCOMPARE(timeTracker.workingTime(), 10 * 60);
//    QCOMPARE(timeTracker.breakTime(), 5 * 60);

//    // Start working again (state needs to be something other than "not working")
//    timestamp = timestamp.addSecs(5 * 60);
//    QVERIFY(timeTracker.startWorking(timestamp));
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_Working);

//    // Reset state
//    timeTracker.resetState();

//    // Check if state was really reset
//    QVERIFY(timeTracker.isValid());
//    QCOMPARE(timeTracker.userId(), 1LL);
//    QCOMPARE(timeTracker.state(), OpenTimeTracker::Server::TimeTracker::State_NotWorking);
//    QCOMPARE(timeTracker.workingTime(), 0);
//    QCOMPARE(timeTracker.breakTime(), 0);
//}

QTEST_APPLESS_MAIN(TimeTrackerTest)

#include "tst_TimeTrackerTest.moc"
