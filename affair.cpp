#include "affair.h"

Affair::Affair()
{
    this->isCompleted=false;
    this->remindstr="";
    this->finalModifyDateTime=QDateTime::currentDateTime();
}
Affair::Affair(const QString &_str, bool _isCompleted,const QDateTime &datetime){
    this->remindstr=_str;
    this->isCompleted=_isCompleted;
    this->finalModifyDateTime=datetime;
}
