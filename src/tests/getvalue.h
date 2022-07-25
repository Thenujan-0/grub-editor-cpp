#include <QtTest/QtTest>

class TestGetValue: public QObject
{
    Q_OBJECT
private slots:
    void commented();
    void simple();
    void grub_disable_os_prober();
};
