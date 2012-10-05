#ifndef AFFAIR_H
#define AFFAIR_H
#include <QString>
#include <QDateTime>
//
//if qt can commit this project,i will see this
//
class Affair
{
public:
    Affair();
    Affair(const QString &_str,bool _isCompleted,const QDateTime &datetime);
    QString remindstr;
    bool isCompleted;
    QDateTime finalModifyDateTime;
};

#endif // AFFAIR_H
