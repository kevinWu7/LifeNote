#include <QMouseEvent>
#include "ui_habititem.h"
#include "habititem.h"
#include "util.h"
#include "weektoolbutton.h"
#include "checkinconfig.h"
#include "logger.h"
#include "roundedtooltiphelper.h"


HabitItem::HabitItem(QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HabitItem)
{
    ui->setupUi(this);
    setContextMenuPolicy(Qt::CustomContextMenu);
    projectName=name;
    ui->imgBtn->setStyleSheet(QString("QToolButton#imgBtn{"
                           "background-color: transparent;"
                           "width: %1 ; min-width: %1; max-width: %1;"
                           "height: %1 ; min-height: %1; max-height: %1;}"
                          ).arg("20px"));
    //this->setStyleSheet("background-color:red");
    ui->imgBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->imgBtn->setIconSize(QSize(20,20));
    ui->nameLabel->setFont(QFont("Arial", 14, QFont::Normal));
    InitWeekButtons();
    ui->nameLabel->setText(name);
    //connect(this, &HabbitItem::mousePressEvent, this, &HabbitItem::onMousePress);
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

//讲habit中的btn初始化是否checked
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
            logger->log(QString("%1 %2 set is true").arg(projectName).arg(date.toString()));
        }
        else
        {
            btn->setWeekButtonClicked(false);
            logger->log(QString("%1 %2 set is false").arg(projectName).arg(date.toString()));
        }
    }
}

void HabitItem::InitWeekButtons()
{
    auto thisWeek=util::getThisWeek();
    for(int i=0;i<ui->weekWidget->layout()->count();i++)
    {
        WeekToolButton* btn= dynamic_cast<WeekToolButton*>(ui->weekWidget->layout()->itemAt(i)->widget());
        RoundedToolTipHelper::installHelper(btn,RoundedToolTipHelper::Top);

        btn->setDate(thisWeek[i]);
        btn->project_name=projectName;
        btn->setToolTip(thisWeek[i].toString("M月d号，ddd"));
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
    delete ui;
}
