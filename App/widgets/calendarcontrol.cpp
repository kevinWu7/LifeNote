#include <QPalette>
#include "calendarcontrol.h"
#include "util.h"
#include "ui_calendarcontrol.h"
#include "recorditem.h"
#include "roundedtooltiphelper.h"
#include "calendarcentral.h"


CalendarControl::CalendarControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarControl)
{
    ui->setupUi(this);

    ui->arrowLeftBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->arrowLeftBtn->setIcon(QIcon(":/icons/res/checkin/arrow-left.png"));
    ui->arrowLeftBtn->setToolTip("向前");
    RoundedToolTipHelper::installHelper(ui->arrowLeftBtn);
    ui->arrowRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->arrowRightBtn->setIcon(QIcon(":/icons/res/checkin/arrow-right.png"));
    ui->arrowRightBtn->setToolTip("向后");
    RoundedToolTipHelper::installHelper(ui->arrowRightBtn);
    ui->projectIconBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->projectIconBtn->setIconSize(QSize(30,30));
    ui->projectLabel->setFont(QFont("Arial", 22, QFont::Normal));

    //设置label的文字居中对齐
    for(int i=0;i<ui->weekDayWidget->layout()->count();i++)
    {
        auto label= dynamic_cast<QLabel*>(ui->weekDayWidget->layout()->itemAt(i)->widget());
        label->setAlignment(Qt::AlignCenter);
    }

    connect(ui->arrowLeftBtn,&QToolButton::clicked,this,&CalendarControl::arrowLeftBtn_clicked);
    connect(ui->arrowRightBtn,&QToolButton::clicked,this,&CalendarControl::arrowRightBtn_clicked);
    cacheAllDate();
    initCurrentMonth();
    ui->recordItem1->setRecordType(RecordType::TotalCheckinCount);
    ui->recordItem2->setRecordType(RecordType::NewContinuousCheckin);
    ui->recordItem3->setRecordType(RecordType::TotalCheckinRatio);

    // 绑定成员函数到实例
    bindFunctionOfreceiveBtnChecked = std::bind(&CalendarControl::receiveBtnChecked, this, std::placeholders::_1);
    // 注册全局事件
    CalendarCentral::getInstance().registerGlobalEvent(bindFunctionOfreceiveBtnChecked);
}

void CalendarControl::receiveBtnChecked(checkin_dateitem * dateItem)
{
    ReSetCheckinStatus(dateItem);
}

void CalendarControl::ReSetCheckinStatus(checkin_dateitem * dateItem)
{
    if(dateItem->project_name!=project_name)
    {
        return;
    }
    auto result= CheckinConfig::getInstance().LoadCheckinConfig();
    std::vector<checkin_dateitem*> checkin_list=result.checkin_map[dateItem->project_name];
    //当配置文件中当前周期的签到数达到规则中的数据，则将该周期全部打卡
    //比如规则为每月打卡5天，找到配置文件中当月的所有打卡日期，若超过5，则将该月全部打卡
    auto uncheck_datelist=GetPeriodDates(checkinRule->period, dateItem->date,checkin_list,false);
    auto check_datelist=GetPeriodDates(checkinRule->period, dateItem->date,checkin_list,true);
    for(int i=0;i<ui->mainGridWidget->layout()->count();i++)
    {
        monthButton* btn= dynamic_cast<monthButton*>(ui->mainGridWidget->layout()->itemAt(i)->widget());
        QDate buttonDate=btn->getDate();
        //将传入的单独的日期先打卡
        if(dateItem->date==buttonDate)
        {
            btn->setMonthButtonClicked(dateItem->ischecked);
        }
        //判断是否满足条件
        bool is_auto_checked=check_datelist.size()>=checkinRule->Times&&checkinRule->period!=CheckinPeriod::DayPeriod;
        for(auto togetherCheck : uncheck_datelist)
        {
            if(togetherCheck==buttonDate)
            {
                btn->setMonthButtonClicked(false,is_auto_checked);
            }
        }
    }
}

//isCheckd=true 返回周期范围内已经签到的
//false 返回周期范围内未签到的
std::vector<QDate> CalendarControl::GetPeriodDates(CheckinPeriod period,QDate checkedDate,const std::vector<checkin_dateitem *> &checkinItems,bool isCheckd)
{
    std::vector<QDate> unCheckingDates;
    std::vector<QDate> CheckingDates;
    // 将checkinItems中的日期转为QDate对象，并存储到另一个向量中
    std::vector<QDate> tempList;
    for (auto item : checkinItems)
    {
        tempList.push_back(item->date);
    }
    if(period==MonthPeriod)
    {
        // 遍历checkinItems中的日期
        auto minMonthDay=checkedDate.addDays(1 - checkedDate.day()); //x月1号
        auto maxMonthDay=checkedDate.addDays(checkedDate.daysInMonth() - checkedDate.day()); //x月30号
        for (QDate date =minMonthDay;date <= maxMonthDay ; date = date.addDays(1))
        {
            // 检查日期是否属于checkinItems中的日期，如果不属于，则添加到matchingDates中
            if (std::find(tempList.begin(), tempList.end(), date) == tempList.end())
            {
                unCheckingDates.push_back(date);
            }
            else
            {
                CheckingDates.push_back(date);
            }
        }
    }
    else if(period==WeekPeriod)
    {
        QDate startDate = checkedDate.addDays(-checkedDate.dayOfWeek() + 1);
        QDate endDate = startDate.addDays(6);

        // 遍历当前周的日期范围
        for (QDate date = startDate; date <= endDate; date = date.addDays(1))
        {
            // 检查日期是否属于checkinItems中的日期，如果不属于，则添加到matchingDates中
            if (std::find(tempList.begin(), tempList.end(), date) == tempList.end())
            {
                unCheckingDates.push_back(date);
            }
            else
            {
                CheckingDates.push_back(date);
            }
        }
    }
    return isCheckd? CheckingDates:unCheckingDates;
}



void CalendarControl::setHabitItem(const std::vector<checkin_dateitem *> &checkinItems,QString projectName,int iconIndex,CheckinRule* m_checkinRule)
{
    project_name=projectName;
    checkinRule=m_checkinRule;
    InitCheckinMonthBtn(checkinItems,projectName);


    //设置图标 和项目名
    if(iconIndex==-1)
    {
         ui->projectIconBtn->setIcon(QIcon(""));
    }
    else
    {
        auto icon=QString::fromStdString(util::iconMap[iconIndex]);
        ui->projectIconBtn->setIcon(QIcon(QString(":/icons/res/checkin/%1").arg(icon)));
    }


    ui->recordItem1->checkinRule=m_checkinRule;
    ui->recordItem2->checkinRule=m_checkinRule;
    ui->recordItem3->checkinRule=m_checkinRule;
    ui->projectLabel->setText(projectName);
    ui->recordItem1->setCheckinData(checkinItems);
    ui->recordItem2->setCheckinData(checkinItems);
    ui->recordItem3->setCheckinData(checkinItems);
}

void CalendarControl::editHabitItem(QString projectName, int iconIndex,CheckinRule *rule)
{
    //设置图标 和项目名
    auto icon=QString::fromStdString(util::iconMap[iconIndex]);
    ui->projectIconBtn->setIcon(QIcon(QString(":/icons/res/checkin/%1").arg(icon)));
    ui->projectLabel->setText(projectName);
    checkinRule=rule;

    for(int i=0;i<ui->mainGridWidget->layout()->count();i++)
    {
        monthButton* btn= dynamic_cast<monthButton*>(ui->mainGridWidget->layout()->itemAt(i)->widget());
        btn->project_name=projectName;
    }
    auto result= CheckinConfig::getInstance().LoadCheckinConfig();
    //当规则发生改变时，重新设置按钮状态
    InitCheckinMonthBtn(result.checkin_map[projectName],projectName);
    ui->recordItem1->checkinRule=checkinRule;
    ui->recordItem2->checkinRule=checkinRule;
    ui->recordItem3->checkinRule=checkinRule;
    ui->recordItem1->setCheckinData(result.checkin_map[projectName]);
    ui->recordItem2->setCheckinData(result.checkin_map[projectName]);
    ui->recordItem3->setCheckinData(result.checkin_map[projectName]);
}

void CalendarControl::InitCheckinMonthBtn(const std::vector<checkin_dateitem *> &checkinItems,QString projectName)
{
    for(int i=0;i<ui->mainGridWidget->layout()->count();i++)
    {
        monthButton* btn= dynamic_cast<monthButton*>(ui->mainGridWidget->layout()->itemAt(i)->widget());
        QDate date=btn->getDate();
        btn->project_name=projectName;
        auto it = std::find_if(checkinItems.begin(), checkinItems.end(), [&date](checkin_dateitem* item)
        {
             return item->date == date;
        });
        if (it != checkinItems.end())
        {
            btn->setMonthButtonClicked(true);
        }
        else
        {
            btn->setMonthButtonClicked(false);
        }
    }
    if(checkinRule==nullptr)
    {
        return;
    }
    if(checkinRule->period==CheckinPeriod::MonthPeriod)
    {
        InitAutoCheckinStatus(startDate,checkinItems);
        InitAutoCheckinStatus(currentFirstDisplayDate,checkinItems);
        InitAutoCheckinStatus(startDate.addDays(41),checkinItems);
    }
    else if(checkinRule->period==CheckinPeriod::WeekPeriod)
    {
        for(int i=0;i<6;i++)
        {
            InitAutoCheckinStatus(startDate.addDays(7*i),checkinItems);
        }
    }
}

void CalendarControl::InitAutoCheckinStatus(QDate current_date,const std::vector<checkin_dateitem *> &checkinItems)
{
    //当配置文件中当前周期的签到数达到规则中的数据，则将该周期全部打卡
    //比如规则为每月打卡5天，找到配置文件中当月的所有打卡日期，若超过5，则将该月全部打卡
    auto uncheck_datelist=GetPeriodDates(checkinRule->period, current_date,checkinItems,false);
    auto check_datelist=GetPeriodDates(checkinRule->period, current_date,checkinItems,true);
    //判断是否满足条件
    bool is_auto_checked=check_datelist.size()>=checkinRule->Times&&checkinRule->period!=CheckinPeriod::DayPeriod;
    if(!is_auto_checked)
    {
        return;
    }
    for(int i=0;i<ui->mainGridWidget->layout()->count();i++)
    {
        monthButton* btn= dynamic_cast<monthButton*>(ui->mainGridWidget->layout()->itemAt(i)->widget());
        QDate buttonDate=btn->getDate();

        for(auto togetherCheck : uncheck_datelist)
        {
            if(togetherCheck==buttonDate)
            {
                btn->setMonthButtonClicked(false,is_auto_checked);
            }
        }
    }
}

void  CalendarControl::arrowLeftBtn_clicked()
{
    // 获取当前展示月份的第一天
    if(currentFirstDisplayDate.month()==1)
    {
        currentFirstDisplayDate = QDate(currentFirstDisplayDate.year()-1, 12, 1);
    }
    else
    {
        currentFirstDisplayDate = QDate(currentFirstDisplayDate.year(), currentFirstDisplayDate.month()-1, 1);
    }
    // 计算日期间距
    int weekDistance= currentFirstDisplayDate.dayOfWeek()-1;

    startDate = currentFirstDisplayDate.addDays(-(weekDistance));
    fillDateTomainGrid(startDate);
    //设置checked状态
    auto result= CheckinConfig::getInstance().LoadCheckinConfig();
    for(auto item :result.project_list)
    {
        if(item->selected)//初始化将选中habit高亮
        {
            InitCheckinMonthBtn(result.checkin_map[item->project_name],item->project_name);
        }
    }
}

void  CalendarControl::arrowRightBtn_clicked()
{
    // 获取当前展示月份的第一天
    if(currentFirstDisplayDate.month()==12)
    {
        currentFirstDisplayDate = QDate(currentFirstDisplayDate.year()+1, 1, 1);
    }
    else
    {
        currentFirstDisplayDate = QDate(currentFirstDisplayDate.year(), currentFirstDisplayDate.month()+1, 1);
    }
    // 计算日期间距
    int weekDistance= currentFirstDisplayDate.dayOfWeek()-1;
    startDate = currentFirstDisplayDate.addDays(-(weekDistance));
    fillDateTomainGrid(startDate);
    //设置checked状态
    auto result= CheckinConfig::getInstance().LoadCheckinConfig();
    for(auto item :result.project_list)
    {
        if(item->selected)//初始化将选中habit高亮
        {
            InitCheckinMonthBtn(result.checkin_map[item->project_name],item->project_name);
        }
    }
}



void CalendarControl::initCurrentMonth()
{
    QDate currentDate = QDate::currentDate();
    // 获取当前月份的第一天
    currentFirstDisplayDate = QDate(currentDate.year(), currentDate.month(), 1);

    // 计算日期间距
    int daysDifference = currentFirstDisplayDate.daysTo(currentDate);
    int weekDay= currentFirstDisplayDate.dayOfWeek();
    int weekDistance= weekDay-1;

    startDate = currentDate.addDays(-(daysDifference + weekDistance));
    fillDateTomainGrid(startDate);
}



void CalendarControl::cacheAllDate()
{
    QDate currentDate = QDate::currentDate();
    // 获取前一年的日期
    QDate lastYearDate = currentDate.addYears(-1);
    // 获取后一年的日期
    QDate nextYearDate = currentDate.addYears(1);

    // 打印前一年到后一年的每一天
    QDate dateIterator = lastYearDate;
    while (dateIterator <= nextYearDate) {
        dateIterator = dateIterator.addDays(1);
        vector_allcache_date.push_back(dateIterator);
    }
}

void CalendarControl::fillDateTomainGrid(QDate startDate)
{
    currentMonth_date.clear();
    QDate currentDate = QDate::currentDate();
    // 循环添加日期到vector中
    for (int i = 0; i < 42; i++)
    {
        currentMonth_date.push_back(startDate.addDays(i));
    }
    for(int index=0;index<ui->mainGridWidget->layout()->count();index++)
    {
         monthButton* monthBtn = util::findWidget<monthButton>(ui->mainGridWidget,"toolButton_"+QString::number(index+1));
         if (monthBtn)
         {
             QString dayNumber=QString::number(currentMonth_date[index].day());
             qDebug() << "Days dayNumber:" << dayNumber;
             monthBtn->setText(dayNumber);
             monthBtn->setDate(currentMonth_date[index]);
             monthBtn->currendDisplayDate=currentFirstDisplayDate;
             //设置当天的按钮，标记为特殊颜色
             if(currentMonth_date[index]==currentDate)
             {
                 monthBtn->setStyleSheet("color:orange;font-weight:bold;");
                 continue;
             }
             if(currentMonth_date[index].month()!=currentFirstDisplayDate.month())
             {
                monthBtn->setStyleSheet("color:rgb(158,158,158);");
                 continue;
             }
             monthBtn->setStyleSheet("color:black;");
         }
    }
    ui->datetimeLabel->setText(QString("%1年 %2月").arg(currentFirstDisplayDate.year()).arg(currentFirstDisplayDate.month()));
}

CalendarControl::~CalendarControl()
{
    CalendarCentral::getInstance().unregisterGlobalEvent(bindFunctionOfreceiveBtnChecked);
    delete ui;
}
