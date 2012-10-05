#include "alldata.h"
#include <QDate>
AllData::AllData()
{
    this->version=-1;
    this->date=QDate::currentDate();
}

void AllData::clearlist()
{
    while(!this->list.isEmpty()){
        delete list.takeFirst();
    }
}
