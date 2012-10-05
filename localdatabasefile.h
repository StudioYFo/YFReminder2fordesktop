#ifndef LOCALDATABASEFILE_H
#define LOCALDATABASEFILE_H
#include <QDate>
#include <QList>
#include <QString>
#include <QFile>
class Affair;
class AllData;
class LocalDataBaseFile
{
public:
    LocalDataBaseFile();
    AllData ReadFromFile();
    void SaveToFile(QDate &date,QList<Affair*> &list);
    void setFileName(QString &_filename);
private:
    QString filename;
    QFile file;
};

#endif // LOCALDATABASEFILE_H
