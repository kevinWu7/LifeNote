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
    // 注册theme全局事件
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
            logger->log(QString("%1 %2 set is true").arg(projectName).arg(date.toString()));
        }
        else
        {
            btn->setWeekButtonClicked(false);
            logger->log(QString("%1 %2 set is false").arg(projectName).arg(date.toString()));
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
