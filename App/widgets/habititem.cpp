#include <QMouseEvent>
#include "ui_habititem.h"
#include "habititem.h"
#include "util.h"
#include "weektoolbutton.h"
#include "checkinconfig.h"
#include "logger.h"
#include "theme.h"
#include "roundedtooltiphelper.h"



HabitItem::HabitItem(QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HabitItem)
{
    ui->setupUi(this);
    setContextMenuPolicy(Qt::CustomContextMenu);
    projectName=name;

    ui->imgBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->imgBtn->setIconSize(QSize(20,20));
    ui->nameLabel->setFont(QFont("Arial", 14, QFont::Normal));

    InitWeekButtons();
    ui->nameLabel->setText(name);


    // 绑定成员函数到实例
    bindFunctionOfreceiveBtnChecked = std::bind(&HabitItem::receiveBtnChecked, this, std::placeholders::_1);
    CalendarCentral::getInstance().registerGlobalEvent(bindFunctionOfreceiveBtnChecked);

    bindFunctionOfreceiveThemeChanged=std::bind(&HabitItem::receiveThemeChanged, this);
    ThemeManager::getInstance().registerThemeGlobalEvent(bindFunctionOfreceiveThemeChanged);
}

void HabitItem::mousePressEvent(QMouseEvent *event)
{
    // 判断鼠标事件类型
    if (event->type() == QMouseEvent::MouseButtonPress)
    {
        // 判断鼠标按钮类型
        if (event->button() == Qt::LeftButton)
        {
            // 处理左键点击事件
            // 获取鼠标点击位置的坐标
            emit triggerMousePressEvent(this);
            //QPoint pos = event->pos();
            // ...
        }
        else if (event->button() == Qt::RightButton)
        {
            // 处理右键点击事件
            // 获取鼠标点击位置的坐标
            QPoint pos = event->pos();
            // ...
        }
    }
}

//将habit中的btn初始化是否checked
void HabitItem::InitCheckinBtn(const std::vector<checkin_dateitem *> &checkinItems)
{
    auto uncheck_datelist=GetPeriodDates(QDate::currentDate(),checkinItems,false);
    auto check_datelist=GetPeriodDates(QDate::currentDate(),checkinItems,true);
    bool is_auto_checked=check_datelist.size()>=checkinRule->Times&&checkinRule->period!=CheckinPeriod::DayPeriod;
    for(int i=0;i<ui->weekWidget->layout()->count();i++)
    {
        WeekToolButton* btn= dynamic_cast<WeekToolButton*>(ui->weekWidget->layout()->itemAt(i)->widget());
        QDate date=btn->getDate();

        auto it = std::find_if(checkinItems.begin(), checkinItems.end(), [&date](checkin_dateitem* item)
        {
            return item->date == date;
        });

        if (it != checkinItems.end())
        {
            btn->setWeekButtonClicked(true);
            checkedCount++;
        }
        else
        {
            btn->setWeekButtonClicked(false);
        }
        //当配置文件中当前周期的签到数达到规则中的数据，则将该周期全部打卡
        //比如规则为每月打卡5天，找到配置文件中当月的所有打卡日期，若超过5，则将该月全部打卡
        if(is_auto_checked)
        {
            for(auto togetherCheck : uncheck_datelist)
            {
                if(togetherCheck==date)
                {
                    btn->setWeekButtonClicked(false,is_auto_checked);
                }
            }
        }
    }
    ui->countLabel->setText(QString("%1").arg(checkedCount));
}

void HabitItem::InitWeekButtons()
{
    auto thisWeek=util::getThisWeek();
    for(int i=0;i<ui->weekWidget->layout()->count();i++)
    {
        WeekToolButton* btn= dynamic_cast<WeekToolButton*>(ui->weekWidget->layout()->itemAt(i)->widget());
        RoundedToolTipHelper::installHelper(btn,RoundedToolTipHelper::ControlTop);
        btn->setDate(thisWeek[i]);
        btn->project_name=projectName;
        btn->setToolTip(thisWeek[i].toString("M月d号 ddd"));
    }
}
//isCheckd=true 返回周期范围内已经签到的
//false 返回周期范围内未签到的
std::vector<QDate> HabitItem::GetPeriodDates(QDate checkedDate,const std::vector<checkin_dateitem *> &checkinItems,bool isCheckd)
{
    std::vector<QDate> unCheckingDates;
    std::vector<QDate> CheckingDates;
    // 将checkinItems中的日期转为QDate对象，并存储到另一个向量中
    std::vector<QDate> tempList;
    for (auto item : checkinItems)
    {
        tempList.push_back(item->date);
    }

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
    return isCheckd? CheckingDates:unCheckingDates;
}


void HabitItem::ReSetCheckinStatus(checkin_dateitem * dateItem)
{
    if(dateItem->project_name!=projectName)
    {
        return;
    }
    auto result= CheckinConfig::getInstance().LoadCheckinConfig();
    std::vector<checkin_dateitem*> checkin_list=result.checkin_map[dateItem->project_name];
    //当配置文件中当前周期的签到数达到规则中的数据，则将该周期全部打卡
    //比如规则为每月打卡5天，找到配置文件中当月的所有打卡日期，若超过5，则将该月全部打卡
    auto uncheck_datelist=GetPeriodDates(dateItem->date,checkin_list,false);
    auto check_datelist=GetPeriodDates( dateItem->date,checkin_list,true);
    for(int i=0;i<ui->weekWidget->layout()->count();i++)
    {
        WeekToolButton* btn= dynamic_cast<WeekToolButton*>(ui->weekWidget->layout()->itemAt(i)->widget());
        QDate buttonDate=btn->getDate();
        //将传入的单独的日期先打卡
        if(dateItem->date==buttonDate)
        {
            btn->setWeekButtonClicked(dateItem->ischecked);
        }
        //判断是否满足条件
        bool is_auto_checked
                =check_datelist.size()>=checkinRule->Times&&checkinRule->period!=CheckinPeriod::DayPeriod;
        for(auto togetherCheck : uncheck_datelist)
        {
            if(togetherCheck==buttonDate)
            {
                btn->setWeekButtonClicked(false,is_auto_checked);
            }
        }
    }
}

//当按钮点击时，更新右侧x/7 label
void HabitItem::receiveBtnChecked(checkin_dateitem *dateItem)
{
    if(dateItem->project_name!=projectName)
    {
        return;
    }

    for(int i=0;i<ui->weekWidget->layout()->count();i++)
    {
        WeekToolButton* btn= dynamic_cast<WeekToolButton*>(ui->weekWidget->layout()->itemAt(i)->widget());
        QDate date=btn->getDate();
        if (date== dateItem->date)
        {
            checkedCount=dateItem->ischecked?checkedCount+1:checkedCount-1;
            ui->countLabel->setText(QString("%1").arg(checkedCount));
            break;
        }
    }
    ReSetCheckinStatus(dateItem);
}

void HabitItem::setHabitSelected(bool isSelect)
{
    isSelected=isSelect;
    updateHabitUiStatus();
}

void HabitItem::receiveThemeChanged()
{
    updateHabitUiStatus();
}

void HabitItem::updateHabitUiStatus()
{
    if(!isSelected)
    {
        this->setStyleSheet("QWidget#mainWidget{background-color:transparent}");
    }
    else
    {
        this->setStyleSheet(QString("QWidget#mainWidget{background-color:%1}").arg(currentTheme["CONTROL_SELECTED"]));
    }
}



void HabitItem::setIconIndex(int index)
{
    auto icon=QString::fromStdString(util::iconMap[index]);
    ui->imgBtn->setIcon(QIcon(QString(":/icons/res/checkin/%1").arg(icon)));
    iconIndex=index;
}

void HabitItem::setProjectName(QString name)
{
    ui->nameLabel->setText(name);
    projectName=name;
}




HabitItem::~HabitItem()
{
    CalendarCentral::getInstance().unregisterGlobalEvent(bindFunctionOfreceiveBtnChecked);
    ThemeManager::getInstance().unregisterThemeGlobalEvent(bindFunctionOfreceiveThemeChanged);
    delete ui;
}
