#include "weekjudge.h"

WeekJudge::WeekJudge()
{
}

bool WeekJudge::isSingleWeek(const QDate &startDate)
{
    int intervalDays=QDate::currentDate().daysTo(startDate);
    if((int)((int)(intervalDays/7)%2)){
        return false;//Ë«ÖÜ
    }
    else{
        return true;//µ¥ÖÜ
    }
}

int WeekJudge::whichWeek(const QDate &startDate)
{
    //return QDate::currentDate().daysTo(startDate)/7+1;
    return startDate.daysTo(QDate::currentDate())/7+1;
}
