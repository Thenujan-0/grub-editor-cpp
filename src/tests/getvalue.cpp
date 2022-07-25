#include "getvalue.h"
#include "cli.h"

QString PATH="/home/thenujan/projects/helloworld/src/tests/testCaseFiles/";

void TestGetValue::simple()
{
    QStringList issues;
    QString readFile=PATH+"commented.txt";
    QString value = getValue("GRUB_TIMEOUT_STYLE",issues,readFile);
    QCOMPARE(value,"menu");
    QCOMPARE(issues,QStringList());
}

void TestGetValue::commented()
{
    QStringList issues;
    QString readFile=PATH+"commented.txt";
    // /home/thenujan/projects/helloworld/src/tests/testCaseFiles/commented.txt
    QString value = getValue(QString("GRUB_TIMEOUT"),issues,readFile);
    QCOMPARE(value,QString(""));
    QCOMPARE(issues,QStringList("GRUB_TIMEOUT was not found in "+readFile));
    // QVERIFY(QString("hey")==QString("hello"));
}

void TestGetValue::grub_disable_os_prober(){
    QStringList issues;
    QString readFile=PATH+"commented.txt";
    
}

QTEST_MAIN(TestGetValue)
#include "getvalue.moc"