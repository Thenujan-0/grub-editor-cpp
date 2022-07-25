#pragma once
#include <QObject>

QStringList getAllOsEntries();

class Data : public QObject{

    Q_OBJECT
    Q_PROPERTY(float grubTimeout READ grubTimeout)
    Q_PROPERTY(bool predefined READ predefined)
    Q_PROPERTY(bool previouslyBooted READ previouslyBooted)
    Q_PROPERTY(defaultEntryType defaultEntryType READ getDefaultEntryType)
    Q_PROPERTY(QStringList osEntries READ getAllOsEntries);
    Q_PROPERTY(bool showMenu READ showMenu)
    Q_PROPERTY(bool bootDefault READ bootDefault)
    Q_PROPERTY(bool lookForOtherOs READ lookForOtherOs)
    



public:
    explicit Data(QObject *parent = nullptr);
    QStringList bootEntries;

    enum defaultEntryType{
            Predefined = 0,
            PreviouslyBooted = 1
    };
    Q_ENUM(defaultEntryType)

    Q_INVOKABLE void set(QString cob_osEntries , bool chb_showMenu);

private:
    void load();
    bool predefined();
    bool previouslyBooted();

    Data::defaultEntryType getDefaultEntryType();
    QStringList getAllOsEntries();
    bool showMenu();
    bool bootDefault();
    float grubTimeout();
    bool lookForOtherOs();



    bool m_bootDefaultAfter;
    bool m_lookForOtherOs;
    float m_grubTimeout;
    bool m_showMenu;
    Data::defaultEntryType m_defaultEntryType;
    QString m_grubDefault;
    QStringList m_osEntries;
    QStringList m_issues;
    QString m_currFileName;

};
