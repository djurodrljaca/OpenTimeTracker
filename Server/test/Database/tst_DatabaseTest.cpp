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
#include <QFile>
#include "../../src/Database.hpp"

class DatabaseTest : public QObject
{
    Q_OBJECT

public:
    DatabaseTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    //void testCaseOpen_data();
    void testCaseOpen();

private:
    OpenTimeTracker::Server::Database m_database;
    QString m_databaseFilePath;
};

DatabaseTest::DatabaseTest()
    : m_database(),
      m_databaseFilePath("test.db")
{
}

void DatabaseTest::initTestCase()
{
    if (QFile::exists(m_databaseFilePath))
    {
        QVERIFY2(QFile::remove(m_databaseFilePath), "Database not removed");
    }
}

void DatabaseTest::cleanupTestCase()
{
    if (QFile::exists(m_databaseFilePath))
    {
        QVERIFY2(QFile::remove(m_databaseFilePath), "Database not removed");
    }
}

//void DatabaseTest::testCaseOpen_data()
//{
//    QTest::addColumn<QString>("data");
//    QTest::newRow("0") << QString();
//}

void DatabaseTest::testCaseOpen()
{
    //QFETCH(QString, data);
    QVERIFY2(m_database.open(m_databaseFilePath), "Failed to open database");
}

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_DatabaseTest.moc"
