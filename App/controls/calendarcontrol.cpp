#include "calendarcontrol.h"
#include "ui_calendarcontrol.h"
#include <QDate>
#include <QPalette>
#include "util.h"

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


    initMonthBtnEvent();
    initDate();
}

void CalendarControl::initMonthBtnEvent()
{
    for(int index=0;index<ui->mainGridWidget->layout()->count();index++)
    {
        QToolButton* childWidget =dynamic_cast<QToolButton*>(ui->mainGridWidget->layout()->itemAt(index)->widget());
        if(childWidget)
        {
            connect(childWidget,&QToolButton::clicked,this,&CalendarControl::monthBtn_clicked);
        }
    }
}
void CalendarControl::monthBtn_clicked()
{
    QToolButton *button = qobject_cast<QToolButton*>(sender());

    QPalette palette = button->palette();
    QColor textColor = palette.color(QPalette::ButtonText);

    if (textColor == Qt::white)
    {
        button->setStyleSheet("background-color:rgb(239,239,239);color:black");
    }
    else
    {
        button->setStyleSheet("background-color:rgb(72,114,251);color:white");
    }
}

void CalendarControl::initDate()
{
    // 获取当前日期
    QDate currentDate = QDate::currentDate();
    qDebug() << "当前日期: " << currentDate.toString(Qt::ISODate);

    // 获取前一年的日期
    QDate lastYearDate = currentDate.addYears(-1);
    // 获取后一年的日期
    QDate nextYearDate = currentDate.addYears(1);

    // 打印前一年到后一年的每一天
    QDate dateIterator = lastYearDate;
    while (dateIterator <= nextYearDate) {
        qDebug() << dateIterator.toString(Qt::ISODate);
        dateIterator = dateIterator.addDays(1);
        vector_date.push_back(dateIterator);
    }
    // 获取当前月份的第一天
    QDate firstDayOfMonth = QDate(currentDate.year(), currentDate.month(), 1);
    // 计算日期间距
    int daysDifference = firstDayOfMonth.daysTo(currentDate);
    qDebug() << "Days difference:" << daysDifference;
    int weekDay= firstDayOfMonth.dayOfWeek();
    qDebug() << "Days week:" << weekDay;
    int weekDistance= weekDay-1;

    QDate startDate = currentDate.addDays(-(daysDifference + weekDistance));
    // 创建新的vector对象
    std::vector<QDate> currentMonth_date;

    // 循环添加日期到vector中
    for (int i = 0; i < 42; i++) {
        currentMonth_date.push_back(startDate.addDays(i));
    }
    for(int index=0;index<ui->mainGridWidget->layout()->count();index++)
    {
         QToolButton* childWidget = util::findWidget<QToolButton>(ui->mainGridWidget,"toolButton_"+QString::number(index+1));
         if (childWidget)
         {
             QString dayNumber=QString::number(currentMonth_date[index].day());
             qDebug() << "Days dayNumber:" << dayNumber;
             childWidget->setText(dayNumber);
         }
    }

}

CalendarControl::~CalendarControl()
{
    delete ui;
}
