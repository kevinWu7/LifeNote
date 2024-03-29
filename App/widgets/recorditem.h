#ifndef RECORDITEM_H
#define RECORDITEM_H


#include <QWidget>
#include "checkinconfig.h"
#include "baseinfo.h"

namespace Ui {
class RecordItem;
}

enum RecordType
{
    TotalCheckinCount, //总打卡
    NewContinuousCheckin, //当前最新连续打卡，按照周期来算，比如每月打卡1次。则按月算
    CurrentMonthRatio,//当月打卡比率
    HighContinuousCheckin , //历史最高连续
    TotalCheckinRatio, //总打卡比率
};

class RecordItem : public QWidget
{
    Q_OBJECT

public:
    explicit RecordItem(QWidget *parent = nullptr);
    RecordType record_type;
    CheckinRule * checkinRule;
    ~RecordItem();
    void setRecordType(RecordType type);
    void setCheckinData(const std::vector<checkin_dateitem*> &checkinItems);
    void receiveBtnChecked(checkin_dateitem* item);

private:
    Ui::RecordItem *ui;
    QDate getFirstPeriodDay(QDate date);
    std::vector<checkin_dateitem*>  getAllPeriodDay(const std::vector<checkin_dateitem *> &checkinItems);
    int find_max_consecutive_dates(const std::vector<checkin_dateitem*>& items);
    int find_current_consecutive_dates(const std::vector<checkin_dateitem*>& items);
    double calculate_current_month_ratio(const std::vector<checkin_dateitem*>& items);
    std::function<void(checkin_dateitem*)> bindFunctionOfreceiveBtnChecked;
    std::map<RecordType,QString> recordMap=
    {
        {TotalCheckinCount,"总打卡"},
        {NewContinuousCheckin,"当前连续"},
        {CurrentMonthRatio,"当月打卡率"},
        {HighContinuousCheckin,"最高连续"},
        {TotalCheckinRatio,"总打卡率"},
    };
};

#endif // RECORDITEM_H
