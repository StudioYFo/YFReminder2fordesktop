#ifndef ALLDATA_H
#define ALLDATA_H
#include "affair.h"
#include <QList>
#include <QDate>
class AllData
{
public:
    AllData();
    int version;
    QDate date;
    QList<Affair*> list;
    void clearlist();
};

#endif // ALLDATA_H
