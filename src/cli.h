#pragma once
#include <QObject>
#include <QString>
#ifndef CLI_H
#define CLI_H

QString getValue(QString key , QStringList &issues,QString readFileName = QString("/etc/default/grub"));
bool setValue(QString key , QString val,QString readFileName = QString("/etc/default/grub"));
bool initCache(QString readFileName = QString("/etc/default/grub"));
#endif
