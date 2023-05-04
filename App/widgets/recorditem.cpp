#include <algorithm>
#include "recorditem.h"
#include "ui_recorditem.h"
#include "calendarcentral.h"
#include "checkinconfig.h"

RecordItem::RecordItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordItem)
{
    ui->setupUi(this);
    ui->numberLabel->setStyleSheet(QString(""));
    ui->numberLabel->setFont(QFont("Arial", 25, QFont::Normal));
    ui->textLabel->setFont(QFont("Arial", 12, QFont::Normal));
    ui->textLabel->setStyleSheet("color:rgb(160,160,160)");

    // 绑定成员函数到实例
    auto bindFunctionOfreceiveBtnChecked = std::bind(&RecordItem::receiveBtnChecked, this, std::placeholders::_1);
    // 注册全局事件
    CalendarCentral::getInstance().registerGlobalEvent(bindFunctionOfreceiveBtnChecked);
}

RecordItem::~RecordItem()
{
    delete ui;
}

void RecordItem::setRecordType(RecordType type)
{
    ui->textLabel->setText(recordMap[type]);
    record_type=type;
}

void RecordItem::setCheckinData(const std::vector<checkin_dateitem *> &checkinItems)
{
    if(record_type==TotalCheckinCount)
    {
        int totalCount=checkinItems.size();
        ui->numberLabel->setText(QString::number(totalCount));
    }
    else if(record_type==NewContinuousCheckin)
    {
        int continuousCount=find_current_consecutive_dates(checkinItems);
        ui->numberLabel->setText(QString::number(continuousCount));
    }
    else if(record_type==CurrentMonthRatio)
    {
        double ratio=calculate_current_month_ratio(checkinItems);
        ui->numberLabel->setText(QString::number(ratio)+"<span style='font-size:15pt;'>%</span>");
    }
}




double RecordItem::calculate_current_month_ratio(const std::vector<checkin_dateitem*>& items)
{
    // 获取当前日期
    QDate current_date = QDate::currentDate();
    int current_month = current_date.month();
    int current_year = current_date.year();

    // 计算当前月份的天数
    int days_in_current_month = current_date.daysInMonth();

    // 统计属于当前月份的 date 的数量
    int count = 0;
    for (const checkin_dateitem* item : items)
    {
        if (item->date.month() == current_month && item->date.year() == current_year)
        {
            count++;
        }
    }

    // 计算并返回比率
    auto ratio= static_cast<double>(count)/static_cast<double>(days_in_current_month);
    double percentage = std::round(ratio * 100.0) ;
    return percentage;
}

void RecordItem::receiveBtnChecked(checkin_dateitem* item)
{
    auto result= CheckinConfig::getInstance().LoadCheckinConfig();
    //加载habititem
    for(auto item :result.project_list)
    {
        if(item->selected)
        {
            setCheckinData(result.checkin_map[item->project_name]);
        }
    }
}



int RecordItem::find_max_consecutive_dates(const std::vector<checkin_dateitem*> &items)
{
    if (items.empty()) {
        return 0;
    }
    // 对 items 中的日期进行排序
    std::vector<checkin_dateitem*> sorted_items(items);
    std::sort(sorted_items.begin(), sorted_items.end(), [](checkin_dateitem* a, checkin_dateitem* b){
       return  a->date < b->date;
    });

    int max_consecutive_dates = 1;
    int current_consecutive_dates = 1;

    for (size_t i = 1; i < sorted_items.size(); ++i)
    {
        if (sorted_items[i]->date == sorted_items[i - 1]->date.addDays(1))
        {
            // 如果当前日期和前一个日期相差一天，那么增加连续日期计数
            ++current_consecutive_dates;
        }
        else if (sorted_items[i]->date != sorted_items[i - 1]->date)
        {
            // 如果当前日期不等于前一个日期，则更新最大连续日期
            max_consecutive_dates = std::max(max_consecutive_dates, current_consecutive_dates);
            current_consecutive_dates = 1;
        }
    }
    // 检查最后一个连续日期段
    max_consecutive_dates = std::max(max_consecutive_dates, current_consecutive_dates);
    return max_consecutive_dates;
}

int RecordItem::find_current_consecutive_dates(const std::vector<checkin_dateitem *> &items)
{
    if (items.empty()) {
        return 0;
    }
    // 对 items 中的日期进行排序
    std::vector<checkin_dateitem*> sorted_items(items);
    std::sort(sorted_items.begin(), sorted_items.end(), [](checkin_dateitem* a, checkin_dateitem* b){
       return  a->date < b->date;
    });

    int current_consecutive_dates = 1;

    for (size_t i = sorted_items.size()-1; i >0; --i)
    {
        if(sorted_items[sorted_items.size()-1]->date!=QDate::currentDate())
        {
            return 0;
        }
        if (sorted_items[i]->date == sorted_items[i - 1]->date.addDays(1))
        {
            // 如果当前日期和前一个日期相差一天，那么增加连续日期计数
            ++current_consecutive_dates;
        }
        else
        {
            break;
        }
    }
    return current_consecutive_dates;
}
