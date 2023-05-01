#ifndef CALENDARCONTROL_H
#define CALENDARCONTROL_H

#include <vector>
#include <QDate>
#include <QWidget>
#include "checkinconfig.h"

namespace Ui {
class CalendarControl;
}

class CalendarControl : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarControl(QWidget *parent = nullptr);
    ~CalendarControl();
    void InitCheckinMonthBtn(std::vector<checkin_dateitem *> checkinItems,QString projectName);
    void initCurrentMonth();
private:
    Ui::CalendarControl *ui;
    QDate currendDisplayDate;//当前展示的日期
    void cacheAllDate();
    void initMonthBtnEvent();
    void monthBtn_clicked();
    void arrowLeftBtn_clicked();
    void arrowRightBtn_clicked();
    void fillDateTomainGrid(QDate startDate);
    std::vector<QDate> vector_allcache_date;
    std::vector<QDate> currentMonth_date;
};

#endif // CALENDARCONTROL_H
