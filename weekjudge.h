#ifndef WEEKJUDGE_H
#define WEEKJUDGE_H
#include <QDate>
class WeekJudge
{
public:
    WeekJudge();
    static bool isSingleWeek(const QDate &startDate);
    static int whichWeek(const QDate &startDate);
};

#endif // WEEKJUDGE_H
