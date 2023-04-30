#include <QPalette>
#include "calendarcontrol.h"
#include "util.h"
#include "ui_calendarcontrol.h"


CalendarControl::CalendarControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarControl)
{
    ui->setupUi(this);

    this->setStyleSheet(QString("QToolButton:hover {border: 0px; background-color: rgb(224, 224, 224); }"
                                "QToolButton{"
                                "border: 0px;"
                                "width: %1 ; min-width: %1; max-width: %1;"
                                "height: %1 ; min-height: %1; max-height: %1;"
                                "border-radius: %2;"
                                "background-color: rgb(239,239,239);"
                                "font-size: 12px;"
                                "padding: 0;"
                                "margin: 0;}"
                                "QToolButton#arrowLeftBtn{"      //左箭头样式
                                "width: %4 ; min-width: %4; max-width: %4;"
                                "height: %4; min-height: %4; max-height: %4;"
                                "border-radius: 5px;"
                                "background-color: transparent;"
                                "}"
                                "QToolButton#arrowLeftBtn:hover{"
                                "background-color: rgb(224, 224, 224);}"
                                "QToolButton#arrowRightBtn{"         //右箭头样式
                                "width: %4 ; min-width: %4; max-width: %4;"
                                "height: %4; min-height: %4; max-height: %4;"
                                "border-radius: 5px;"
                                "background-color: transparent;"
                                "}"
                                "QToolButton#arrowRightBtn:hover{"
                                "background-color: rgb(224, 224, 224);}"
                              ).arg("34px","17px","22px","12px"));

    ui->arrowLeftBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->arrowLeftBtn->setIcon(QIcon(":/icons/res/checkin/arrow-left.png"));
    ui->arrowRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->arrowRightBtn->setIcon(QIcon(":/icons/res/checkin/arrow-right.png"));

    ui->titleBarWidget->setStyleSheet("QLabel#datetimeLabel{font-size: 16pt;}");
    ui->weekDayWidget->setStyleSheet(
        "QLabel {"
        "width: 34px ; min-width: 34px; max-width: 34px;"
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
}


void CalendarControl::InitCheckinMonthBtn(std::vector<checkin_dateitem *> checkinItems,QString projectName)
{
    for(int i=0;i<ui->mainGridWidget->layout()->count();i++)
    {
        monthButton* btn= dynamic_cast<monthButton*>(ui->mainGridWidget->layout()->itemAt(i)->widget());
        QDate date=btn->date;
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
    //若为当前月份，则直接调用initCurrentMonth
    if(currendDisplayDate.year()==currentDate.year()&&currendDisplayDate.month()==currentDate.month())
    {
       // initCurrentMonth();
        //return;
    }
    // 计算日期间距
    int weekDistance= currendDisplayDate.dayOfWeek()-1;

    QDate startDate = currendDisplayDate.addDays(-(weekDistance));
    fillDateTomainGrid(startDate);

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
    //若为当前月份，则直接调用initCurrentMonth
    if(currendDisplayDate.year()==currentDate.year()&&currendDisplayDate.month()==currentDate.month())
    {
        //initCurrentMonth();
        //return;
    }
    // 计算日期间距
    int weekDistance= currendDisplayDate.dayOfWeek()-1;
    QDate startDate = currendDisplayDate.addDays(-(weekDistance));
    fillDateTomainGrid(startDate);
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
             monthBtn->date=currentMonth_date[index];
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
