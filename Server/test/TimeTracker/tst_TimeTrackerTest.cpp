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
};

TimeTrackerTest::TimeTrackerTest()
{
}

void TimeTrackerTest::testCaseUserId()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;

    const qint64 userId = 1LL;

    timeTracker.setUserId(userId);
    QCOMPARE(timeTracker.userId(), userId);
}

void TimeTrackerTest::testCaseNotWorking()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    QCOMPARE(timeTracker.workingTime(), 0);
    QCOMPARE(timeTracker.breakTime(), 0);
}

void TimeTrackerTest::testCaseNotWorkingFail()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    const QDateTime timestamp = QDateTime::currentDateTime();

    // Check for failure
    QVERIFY(!timeTracker.startWorking(QDateTime()));
    QVERIFY(!timeTracker.startBreak(timestamp));
    QVERIFY(!timeTracker.endBreak(timestamp));
    QVERIFY(!timeTracker.stopWorking(timestamp));
}

void TimeTrackerTest::testCaseWorking()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    // Start working (5 min before current time)
    const qint32 offset = 5 * 60;
    const QDateTime timestamp = QDateTime::currentDateTime().addSecs(-offset);
    QVERIFY(timeTracker.startWorking(timestamp));

    // Check working and break times
    const qint32 workingTime = timeTracker.workingTime();
    const qint32 breakTime = timeTracker.breakTime();
    const qint32 workingTimeMax = timestamp.secsTo(QDateTime::currentDateTime());

    QVERIFY(offset <= workingTime);
    QVERIFY(workingTime <= workingTimeMax);
    QCOMPARE(breakTime, 0);
}

void TimeTrackerTest::testCaseWorkingFail()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    const QDateTime timestamp = QDateTime::currentDateTime();

    // Start working (1 min before current time)
    const QDateTime timestampWorking = timestamp.addSecs(-60);
    QVERIFY(timeTracker.startWorking(timestampWorking));

    // Check for failure
    QVERIFY(!timeTracker.startWorking(timestamp));
    QVERIFY(!timeTracker.startBreak(QDateTime()));
    QVERIFY(!timeTracker.startBreak(timestampWorking.addSecs(-1)));
    QVERIFY(!timeTracker.endBreak(timestamp));
    QVERIFY(!timeTracker.stopWorking(QDateTime()));
    QVERIFY(!timeTracker.startBreak(timestampWorking.addSecs(-1)));
}

void TimeTrackerTest::testCaseOnBreak()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    const QDateTime currentTimestamp = QDateTime::currentDateTime();

    // Start working (10 min before current time)
    const qint32 offsetWorking = 10 * 60;
    const QDateTime timestampWorking = currentTimestamp.addSecs(-offsetWorking);
    QVERIFY(timeTracker.startWorking(timestampWorking));

    // Start break (5 min before current time)
    const qint32 offsetBreak = 5 * 60;
    const QDateTime timestampBreak = currentTimestamp.addSecs(-offsetBreak);
    QVERIFY(timeTracker.startBreak(timestampBreak));

    // Check working and break times
    const qint32 workingTime = timeTracker.workingTime();
    const qint32 breakTime = timeTracker.breakTime();
    const qint32 breakTimeMax = timestampBreak.secsTo(QDateTime::currentDateTime());

    QCOMPARE(workingTime, (offsetWorking - offsetBreak));
    QVERIFY(offsetBreak <= breakTime);
    QVERIFY(breakTime <= breakTimeMax);
}

void TimeTrackerTest::testCaseOnBreakFail()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    const QDateTime timestamp = QDateTime::currentDateTime();

    // Start working (2 min before current time)
    const QDateTime timestampWorking = timestamp.addSecs(-120);
    QVERIFY(timeTracker.startWorking(timestampWorking));

    // Start break (1 min before current time)
    const QDateTime timestampBreak = timestamp.addSecs(-60);
    QVERIFY(timeTracker.startBreak(timestampBreak));

    // Check for failure
    QVERIFY(!timeTracker.startWorking(timestamp));
    QVERIFY(!timeTracker.startBreak(timestamp));
    QVERIFY(!timeTracker.endBreak(QDateTime()));
    QVERIFY(!timeTracker.endBreak(timestampBreak.addSecs(-1)));
    QVERIFY(!timeTracker.stopWorking(timestamp));
}

void TimeTrackerTest::testCaseFromBreak()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    const QDateTime currentTimestamp = QDateTime::currentDateTime();

    // Start working (15 min before current time)
    const qint32 offsetWorking = 15 * 60;
    const QDateTime timestampWorking = currentTimestamp.addSecs(-offsetWorking);
    QVERIFY(timeTracker.startWorking(timestampWorking));

    // Start break (10 min before current time)
    const qint32 offsetOnBreak = 10 * 60;
    const QDateTime timestampOnBreak = currentTimestamp.addSecs(-offsetOnBreak);
    QVERIFY(timeTracker.startBreak(timestampOnBreak));

    // End break (5 min before current time)
    const qint32 offsetFromBreak = 5 * 60;
    const QDateTime timestampFromBreak = currentTimestamp.addSecs(-offsetFromBreak);
    QVERIFY(timeTracker.endBreak(timestampFromBreak));

    // Check working and break times
    const qint32 workingTime = timeTracker.workingTime();
    const qint32 breakTime = timeTracker.breakTime();
    const qint32 workingTimeMin = offsetWorking - offsetOnBreak;
    const qint32 workingTimeMax = workingTimeMin +
                                  timestampFromBreak.secsTo(QDateTime::currentDateTime());

    QVERIFY(workingTimeMin <= workingTime);
    QVERIFY(workingTime <= workingTimeMax);
    QCOMPARE(breakTime, (offsetOnBreak - offsetFromBreak));
}

void TimeTrackerTest::testCaseFromBreakFail()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    const QDateTime timestamp = QDateTime::currentDateTime();

    // Start working (3 min before current time)
    const QDateTime timestampWorking = timestamp.addSecs(-180);
    QVERIFY(timeTracker.startWorking(timestampWorking));

    // Start break (2 min before current time)
    const QDateTime timestampOnBreak = timestamp.addSecs(-120);
    QVERIFY(timeTracker.startBreak(timestampOnBreak));

    // End break (1 min before current time)
    const QDateTime timestampFromBreak = timestamp.addSecs(-60);
    QVERIFY(timeTracker.endBreak(timestampFromBreak));

    // Check for failure
    QVERIFY(!timeTracker.startWorking(timestamp));
    QVERIFY(!timeTracker.startBreak(QDateTime()));
    QVERIFY(!timeTracker.startBreak(timestampFromBreak.addSecs(-1)));
    QVERIFY(!timeTracker.endBreak(timestamp));
    QVERIFY(!timeTracker.stopWorking(QDateTime()));
    QVERIFY(!timeTracker.stopWorking(timestampFromBreak.addSecs(-1)));
}

void TimeTrackerTest::testCaseNormalFlow()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    // Start working
    QDateTime timestamp = QDateTime::currentDateTime();
    QVERIFY(timeTracker.startWorking(timestamp));

    // Start break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));

    // End break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));

    // Stop working 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.stopWorking(timestamp));

    // Check cumulative times
    QCOMPARE(timeTracker.workingTime(), 10 * 60);
    QCOMPARE(timeTracker.breakTime(), 5 * 60);
}

void TimeTrackerTest::testCaseNormalFlowDoubleBreak()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    // Start working
    QDateTime timestamp = QDateTime::currentDateTime();
    QVERIFY(timeTracker.startWorking(timestamp));

    // Start break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));

    // End break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));

    // Start break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));

    // End break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));

    // Stop working 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.stopWorking(timestamp));

    // Check cumulative times
    QCOMPARE(timeTracker.workingTime(), 15 * 60);
    QCOMPARE(timeTracker.breakTime(), 10 * 60);
}

void TimeTrackerTest::testCaseNormalFlowDoubleAll()
{
    OpenTimeTracker::Server::TimeTracker timeTracker;
    timeTracker.setUserId(1LL);

    // First normal flow

    // Start working
    QDateTime timestamp = QDateTime::currentDateTime();
    QVERIFY(timeTracker.startWorking(timestamp));

    // Start break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));

    // End break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));

    // Stop working 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.stopWorking(timestamp));

    // Second normal flow

    // Start working 30 min later
    timestamp = timestamp.addSecs(30 * 60);
    QVERIFY(timeTracker.startWorking(timestamp));

    // Start break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.startBreak(timestamp));

    // End break 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.endBreak(timestamp));

    // Stop working 5 min later
    timestamp = timestamp.addSecs(5 * 60);
    QVERIFY(timeTracker.stopWorking(timestamp));

    // Check cumulative times
    QCOMPARE(timeTracker.workingTime(), 20 * 60);
    QCOMPARE(timeTracker.breakTime(), 10 * 60);
}

QTEST_APPLESS_MAIN(TimeTrackerTest)

#include "tst_TimeTrackerTest.moc"
