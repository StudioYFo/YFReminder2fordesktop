#include "localdatabasefile.h"
#include <QTextStream>
#include <QDataStream>
#include "affair.h"
#include "alldata.h"
#include "settings.h"
LocalDataBaseFile::LocalDataBaseFile()
{
    this->filename="SavedReminder.txt";
}

void LocalDataBaseFile::SaveToFile(QDate &date, QList<Affair*> &list)
{
    file.setFileName(filename);
    QTextStream stream(&file);
    if(file.open(QIODevice::WriteOnly)){
        stream<<version<<endl;
        stream<<date.toString("yyyy M d")<<endl;
        int len=list.length();
        if(0!=len){
            for(int i=0;i<len-1;i++){
                stream<<list.at(i)->remindstr<<endl;
                stream<<list.at(i)->finalModifyDateTime.toString("yyyy M d h m s")<<endl;
                if(list.at(i)->isCompleted){
                    stream<<1<<endl;
                }
                else{
                    stream<<0<<endl;
                }
            }
            stream<<list.at(len-1)->remindstr<<endl;
            stream<<list.at(len-1)->finalModifyDateTime.toString("yyyy M d h m s")<<endl;
            if(list.at(len-1)->isCompleted){
                stream<<1;
            }else{
                stream<<0;
            }
        }
        file.close();
    }

}

void LocalDataBaseFile::setFileName(QString &_filename)
{
    this->filename=_filename;
}


AllData LocalDataBaseFile::ReadFromFile()
{
    file.setFileName(filename);
    AllData data;
    int year,month,day,hour,minute,second;
    bool isCompleted;
    int intIsCompleted;
    QString remindstr;
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        stream>>data.version;
        stream>>year>>month>>day;
        data.date.setDate(year,month,day);
        while(!stream.atEnd()){
            remindstr=stream.readLine();
            stream>>year>>month>>day>>hour>>minute>>second;
            stream>>intIsCompleted;
            isCompleted=(intIsCompleted==1)?true:false;
            if(remindstr.isEmpty())continue;
            data.list.append(new Affair(remindstr,isCompleted,QDateTime(QDate(year,month,day),QTime(hour,minute,second))));
        }
        file.close();
    }
    else{
        data.version=-1;
    }
    return data;
}
