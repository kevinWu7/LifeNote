#include <QPalette>
#include "calendarcontrol.h"
#include "util.h"
#include "ui_calendarcontrol.h"
#include "recorditem.h"


CalendarControl::CalendarControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarControl)
{
    ui->setupUi(this);

    this->setStyleSheet(QString("QToolButton#arrowLeftBtn{"      //左箭头样式
                                "width: %1 ; min-width: %1; max-width: %1;"
                                "height: %1; min-height: %1; max-height: %1;"
                                "border-radius: 5px;"
                                "background-color: transparent;"
                                "}"
                                "QToolButton#arrowLeftBtn:hover{"
                                "background-color: rgb(224, 224, 224);}"
                                "QToolButton#arrowRightBtn{"         //右箭头样式
                                "width: %1; min-width: %1; max-width: %1;"
                                "height: %1; min-height: %1; max-height: %1;"
                                "border-radius: 5px;"
                                "background-color: transparent;"
                                "}"
                                "QToolButton#arrowRightBtn:hover{"
                                "background-color: rgb(224, 224, 224);}"
                                "QToolButton#projectIconBtn{"      //图标
                                 "background-color: transparent;}"
                              ).arg("12px"));

    ui->arrowLeftBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->arrowLeftBtn->setIcon(QIcon(":/icons/res/checkin/arrow-left.png"));
    ui->arrowRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->arrowRightBtn->setIcon(QIcon(":/icons/res/checkin/arrow-right.png"));

    ui->projectIconBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->projectIconBtn->setIconSize(QSize(30,30));
    ui->projectLabel->setFont(QFont("Arial", 22, QFont::Normal));

    ui->centerLine->setStyleSheet(QString("QFrame{border-top: 1px solid %1; border-bottom: none;}").arg(LINE_COLOR));


    ui->titleBarWidget->setStyleSheet("QLabel#datetimeLabel{font-size: 16pt;}");
    ui->weekDayWidget->setStyleSheet(
        "QLabel {"
        "width: 32px ; min-width: 32px; max-width: 32px;"
        "font-size: 12px;"            // 设置字体大小
        "color: rgba(0, 0, 0,0.3);}"  // 设置文本颜色为黑色，透明度为70%

        "QLabel#weeklabel_6{color: rgba(255,185,48,0.8);}"
        "QLabel#weeklabel_7{color: rgba(255,185,48,0.8);}"
                );
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
    ui->recordItem3->setRecordType(RecordType::CurrentMonthRatio);
}

void CalendarControl::setHabitItem(const std::vector<checkin_dateitem *> &checkinItems,QString projectName,int iconIndex)
{
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

    ui->projectLabel->setText(projectName);
    ui->recordItem1->setCheckinData(checkinItems);
    ui->recordItem2->setCheckinData(checkinItems);
    ui->recordItem3->setCheckinData(checkinItems);
}

void CalendarControl::editHabitItem(QString projectName, int iconIndex)
{
    //设置图标 和项目名
    auto icon=QString::fromStdString(util::iconMap[iconIndex]);
    ui->projectIconBtn->setIcon(QIcon(QString(":/icons/res/checkin/%1").arg(icon)));
    ui->projectLabel->setText(projectName);
    for(int i=0;i<ui->mainGridWidget->layout()->count();i++)
    {
        monthButton* btn= dynamic_cast<monthButton*>(ui->mainGridWidget->layout()->itemAt(i)->widget());
        btn->project_name=projectName;
    }
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
}

void  CalendarControl::arrowLeftBtn_clicked()
{
    QDate currentDate = QDate::currentDate();

    // 获取当前展示月份的第一天
    if(currendDisplayDate.month()==1)
    {
        currendDisplayDate = QDate(currendDisplayDate.year()-1, 12, 1);
    }
    else
    {
        currendDisplayDate = QDate(currendDisplayDate.year(), currendDisplayDate.month()-1, 1);
    }
    // 计算日期间距
    int weekDistance= currendDisplayDate.dayOfWeek()-1;

    QDate startDate = currendDisplayDate.addDays(-(weekDistance));
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
    QDate currentDate = QDate::currentDate();

    // 获取当前展示月份的第一天
    if(currendDisplayDate.month()==12)
    {
        currendDisplayDate = QDate(currendDisplayDate.year()+1, 1, 1);
    }
    else
    {
        currendDisplayDate = QDate(currendDisplayDate.year(), currendDisplayDate.month()+1, 1);
    }
    // 计算日期间距
    int weekDistance= currendDisplayDate.dayOfWeek()-1;
    QDate startDate = currendDisplayDate.addDays(-(weekDistance));
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
    currendDisplayDate = QDate(currentDate.year(), currentDate.month(), 1);

    // 计算日期间距
    int daysDifference = currendDisplayDate.daysTo(currentDate);
    int weekDay= currendDisplayDate.dayOfWeek();
    int weekDistance= weekDay-1;

    QDate startDate = currentDate.addDays(-(daysDifference + weekDistance));
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
             monthBtn->currendDisplayDate=currendDisplayDate;
             //设置当天的按钮，标记为特殊颜色
             if(currentMonth_date[index]==currentDate)
             {
                 monthBtn->setStyleSheet("color:orange;font-weight:bold;");
                 continue;
             }
             if(currentMonth_date[index].month()!=currendDisplayDate.month())
             {
                 monthBtn->setStyleSheet("color:rgb(158,158,158);");
                 continue;
             }
             monthBtn->setStyleSheet("color:black;");
         }
    }
    ui->datetimeLabel->setText(QString("%1年 %2月").arg(currendDisplayDate.year()).arg(currendDisplayDate.month()));
}

CalendarControl::~CalendarControl()
{
    delete ui;
}
