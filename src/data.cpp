#include <data.h>
#include <cli.h>
#include <QDebug>
#include <QProcess>
#include <KConfig>
#include <QDir>
#include <QFile>


QStringList getAllOsEntries_(){
    QStringList entries;
    QProcess getEntries;
    QStringList arguments;
    arguments.push_back(QString("-F\'" ));
    arguments.push_back(QString("/menuentry / {print $2}"));
    arguments.push_back(QString("/boot/grub/grub.cfg"));
    getEntries.start("awk",arguments);
    getEntries.waitForFinished(2000);
    QString output = getEntries.readAllStandardOutput();
    QStringList tempEntries = output.split('\n',Qt::SkipEmptyParts);

    QString lastMain = nullptr;
    QRegExp pattern("\\(Kernel: \\d.\\d\\d");
    for (QString entry : qAsConst(tempEntries)){
        qWarning()<<"iterating"<<entry;
        bool contains = entry.contains(pattern);
        if (contains){
            entries.push_back(lastMain+QStringLiteral(" >")+entry);

        }else{
            lastMain = entry;
            entries.push_back(entry);
        }


    }
//     qWarning()<<"output is "<<output;
//     qWarning()<<"tempEntries"<<tempEntries;
//     qWarning()<<"entries"<<entries;
    return entries;
}




Data::Data(QObject *parent)
    :m_issues(QStringList()),
    m_currFileName(QString("/etc/default/grub"))
{
//      showMenu = true;
    Data::load();
    qWarning()<<"loaded everything";
    qWarning()<<m_osEntries;
}


void Data::set(QString cob_osEntries, bool chb_showMenu){
    QString path("/home/thenujan/.config/plasma-workspace/startup/");
    QDir dir;
    qWarning()<<dir;
    if (!dir.exists(path)){
        dir.mkpath(path);
        qWarning()<<"folder doesnt exist"<<dir;
    }
    

    QFile file(path+"main.conf");
    file.open(QIODevice::WriteOnly);
    qWarning()<<file;

    KConfig conf("/home/thenujan/.config/plasma-workspace/startup/main.conf");
    qWarning()<<"Yes it is working"<<cob_osEntries<<chb_showMenu;
}













void Data::load(){


    m_osEntries= getAllOsEntries_();

    // get grub default
    QString grubDefault = getValue(QStringLiteral("GRUB_DEFAULT"),m_issues,m_currFileName);
    getAllOsEntries();
    if (grubDefault=="saved"){
        m_defaultEntryType=Data::defaultEntryType::PreviouslyBooted;
    }else if (grubDefault==nullptr){
        m_defaultEntryType= Data::defaultEntryType::Predefined;

    }


    QString grubTimeout=  getValue(QString("GRUB_TIMEOUT"),m_issues,m_currFileName);
    m_grubTimeout =grubTimeout.toFloat();

    QString timeoutStyle = getValue(QStringLiteral("GRUB_TIMEOUT_STYLE"),m_issues,m_currFileName);

    if (timeoutStyle==QString("hidden")) {
        m_showMenu=false;
    } else if (timeoutStyle==QString("menu")){
        m_showMenu=true;
    }else{
        Q_ASSERT(0);
    }

    qWarning()<<"grubTimeout"<<m_grubTimeout<<grubTimeout;

    if( m_grubTimeout==-1  ){
        m_bootDefaultAfter=false;
    }else{
        m_bootDefaultAfter=true;
    }

    QString disableOsProber= getValue("GRUB_DISABLE_OS_PROBER",m_issues,m_currFileName);
    if (disableOsProber=="false"){
        m_lookForOtherOs=true;
    }else{
        m_lookForOtherOs=false;
    }

}
QStringList Data::getAllOsEntries(){
    //This function is necessary because m_osEntries could have different value than getAllOsEntries_ function that is defined outside of this scope
    // as invalid entries will get appended to m_osEntries variable

    qWarning()<<"private function was called";


    return m_osEntries;
}
bool Data::predefined(){
    return m_defaultEntryType==Data::defaultEntryType::Predefined;
}
bool Data::previouslyBooted(){
    return m_defaultEntryType==Data::defaultEntryType::PreviouslyBooted;
}

bool Data::bootDefault(){
    return m_bootDefaultAfter;
}

float Data::grubTimeout(){
    return m_grubTimeout;
}

Data::defaultEntryType Data::getDefaultEntryType()
{
    return m_defaultEntryType;
}

bool Data::showMenu(){
    return m_showMenu;
}

bool Data::lookForOtherOs(){
    return m_lookForOtherOs;
}