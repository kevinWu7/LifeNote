#ifndef CALENDARCONTROL_H
#define CALENDARCONTROL_H

#include <vector>
#include <QDate>
#include <QWidget>
#include "checkinconfig.h"
#include "baseinfo.h"

namespace Ui {
class CalendarControl;
}

class CalendarControl : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarControl(QWidget *parent = nullptr);
    ~CalendarControl();
    void InitCheckinMonthBtn(const std::vector<checkin_dateitem *> &checkinItems,QString projectName);
    void initCurrentMonth();
    QString project_name;
    CheckinRule* checkinRule;
    void setHabitItem(const std::vector<checkin_dateitem *> &checkinItems,QString projectName,int iconIndex,CheckinRule* checkinRule);//设置为当前选中项目
    void editHabitItem(QString projectName,int iconIndex);//编辑habit，需要对应修改CalendarControl
    void receiveBtnChecked(checkin_dateitem * item);
private:
    Ui::CalendarControl *ui;
    QDate currentFirstDisplayDate;//当前面板的当月第一天
    void cacheAllDate();
    void initMonthBtnEvent();
    void monthBtn_clicked();
    void arrowLeftBtn_clicked();
    void arrowRightBtn_clicked();
    void fillDateTomainGrid(QDate startDate);
    void CheckinAllPeriodBtn(QDate checkedBtn,const std::vector<checkin_dateitem *> &checkinItems);//满足周期打卡条件后，全部选中
    //根据规则周期，筛选出范围内的日期。checkinItems是所有的打卡日期，返回则是当前比如6月份内的打卡日期
    std::vector<QDate> GetPeriodDates(CheckinPeriod period,QDate checkedDate, const std::vector<checkin_dateitem *> &checkinItems);
    std::vector<QDate> vector_allcache_date;
    std::vector<QDate> currentMonth_date;
    std::function<void(checkin_dateitem *)> bindFunctionOfreceiveBtnChecked;
};

#endif // CALENDARCONTROL_H
