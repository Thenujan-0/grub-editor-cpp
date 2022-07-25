#include <iostream>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <KAuth/Action>
#include <KAuth/ExecuteJob>

#include <cli.h>

using std::cout;
using std::endl;

const QString defaultFileName("/etc/default/grub");



QString getValue(const QString key,QStringList &issues,const QString readFileName){

    QFile readFile(readFileName);
    QString value;
    bool foundCommented;
    QString comment_issue_string =key+QString(" is commented out in ")+readFileName;

    if (!readFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString("Coud not read file");

    QTextStream in(&readFile);
    while (!in.atEnd()){

        QString line = in.readLine();

        //remove all empty spaces
        line.remove(QChar(' '));

        if(line.startsWith(QChar('#')+key+QChar('='))){
            foundCommented=true;

        }else if (line.startsWith('#')){
            continue;

        }else if (line.startsWith(key+QChar('='))){

            value = line.remove(QRegExp(QString(key)+QChar('=')));

            break;
        }


    }
    readFile.close();

    //remove double quotes from it
    if(key=="GRUB_DEFAULT" && !value.isEmpty()){
        qWarning()<<"key is grub default and value is not null";
        if( value.contains(QRegExp(">"))&& !value.contains(QRegExp(" >")) ){
            value.replace(QString(">"),QString(" >"));
        }else if (value.contains(QRegExp(" >"))){
            //This is invalid so to make sure that other functions identify this as invalid
            value.replace(QString(" >"),QString(">"));
        }

        if (value!=QString("saved")){
            QRegExp allNum("\\d*");

            if (value.startsWith(QChar('"')) && value.endsWith(QChar('"'))){
                value.remove(value.length()-1,1);

                value.remove(0,1);

            //check if it is made out of numbers
            }else if ( !allNum.exactMatch(value.remove(QRegExp(" >"))) ){
                value= value+ QStringLiteral(" Missing \"");

            }
        }
    }
    if(key==QString("GRUB_DISABLE_OS_PROBER") && value.isNull()){
        if (foundCommented && !issues.contains(comment_issue_string)){
            issues.push_back(comment_issue_string);
        }else{
            issues.push_back(key+QString(" was not found in ")+readFileName);
        }
    }

    if(!value.isEmpty()){
        return value;
    }

    issues.push_back(key+QString(" was not found in ")+readFileName);
    return nullptr;


}

bool setValue(QString key ,QString val, QString readFileName){
    QFile readFile(readFileName);
    QString toWrite;
    bool replaced=false;
    if (!readFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&readFile);

    if (key==QString("GRUB_DEFAULT")){
        val.replace(QString(" >"),QString(">"));
    }

    while (!in.atEnd()){

        QString line = in.readLine();

        //remove all empty spaces
        line.remove(QChar(' '));

        if (line.startsWith('#')){
            toWrite+=QChar('\n')+in.readLine();
            continue;

        }else if (line.startsWith(key+QChar('=')) && !replaced){
            QString value;
            value = line.remove(QRegExp(QString(key)+QChar('=')));
            line.replace(value,val);
            toWrite+=QChar('\n')+line;
            replaced=true;
            continue;

            
        }else{
            toWrite+=QChar('\n')+in.readLine();
        }
    }
    return true;
}


bool initCache(QString readFileName){
    /* Creates a temp file in cache and copies the data from readFileName to that file */
    return false;

}
void runAction()
{
    action = KAuth::Action("org.kde.myapp.action");
    KAuth::ExecuteJob *job = action.execute();
    connect(job, &KAuth::ExecuteJob::result, this, &MyApp::actionResult);
    job->start();
}
 
void actionResult(KJob *kjob)
{
    auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
    qDebug() << job.error() << job.data();
}