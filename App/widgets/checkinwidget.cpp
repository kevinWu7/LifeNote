#include <QFile>
#include <QXmlStreamReader>
#include "checkinwidget.h"
#include "logger.h"
#include "ui_checkinwidget.h"



#define CHECKIN_CONFIG_PATH "/config/checkin.xml"



checkinWidget::checkinWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::checkinWidget)
{
    ui->setupUi(this);
    ui->addItemBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->addItemBtn->setIcon(QIcon(":/icons/res/note/addnew.png"));
    ui->addItemBtn->setToolTip("添加打卡项目");
    ui->addItemBtn->setIconSize(QSize(22,22));

    ui->iconBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->iconBtn->setIcon(QIcon(":/icons/res/checkin/sun.png"));
    ui->iconBtn->setIconSize(QSize(27,27));
    ui->dateTimeLabel->setFont(QFont("Arial", 16, QFont::Normal));
    ui->leftTitleStatus->setSpacing(2);


    ui->titleBarControl->setStyleSheet("QToolButton:hover { background-color: rgb(218, 218, 218); }"
                                     "QToolButton{width:32px;min-width:32px;max-width:32px;height:32px; min-height:32px; max-height:32px;"
                                       "margin:0px; border:none;}");
    this->setStyleSheet("QWidget#mainWidget {background-color:#FFFFFF;border-radius:7px}"
                        "QWidget#mainWidget * {background-color: transparent;border-radius:0px}");

    ui->mainSplitter->setStyleSheet("QSplitter::handle { background-color: transparent; }"
                                    "QSplitter::handle:horizontal {"
                                    "  width: 1px;" // 将宽度设置为3像素，以便在左右边框之间留出空间
                                    "  margin: 0px;"
                                    "  border-right: none;" // 设置左边框颜色和宽度
                                    "  border-left:1px solid rgb(240,239,239);" // 设置右边框颜色和宽度
                                    "}");
    connect(ui->addItemBtn,&QToolButton::clicked,this,&checkinWidget::addItemBtn_clicked);



    auto result= CheckinConfig::getInstance().LoadCheckinConfig();
    //加载habititem
    for(auto item :result.project_list)
    {
        HabbitItem* habit= addHabitItem(item);
        habit->InitCheckinBtn(result.checkin_map[item->project_name]);
        if(item->selected)//初始化将选中habit高亮
        {
            habit->isSelected=true;
            habit->setStyleSheet("QWidget#mainWidget{background-color:rgba(234,240,255,0.7)}");
            ui->calendarWidget->setHabitItem(result.checkin_map[item->project_name],item->project_name,
                    habit->iconIndex);
        }
    }
    //set the splitter default-ratio,total=7,左侧=2,右侧=7.
    ui->mainSplitter->setStretchFactor(0,5); //代表第0个控件，即左边所占比例为2
    ui->mainSplitter->setStretchFactor(1,4);//代表第1个控件，即右边所占比例为7.一共是9
    InitCurrentDate();
}

void checkinWidget::timerOutTriggered()
{
    QDate newCurrentDate=QDate::currentDate();
    if(newCurrentDate==currentDate)
    {
         return;
    }
    currentDate=newCurrentDate;
    QString labelText=QString("%1年%2月%3号").arg(QString::number(currentDate.year()),
                  QString::number(currentDate.month()),QString::number(currentDate.day()));
    ui->dateTimeLabel->setText(labelText);
    auto result= CheckinConfig::getInstance().LoadCheckinConfig();
    //habit 更新
    for(int i=0;i< ui->leftNavigateWidget->layout()->count();i++)
    {
        auto control=dynamic_cast<HabbitItem*>(ui->leftNavigateWidget->layout()->itemAt(i)->widget());
        if(control!=nullptr)
        {
            control->InitWeekButtons();
            //加载habititem
            control->InitCheckinBtn(result.checkin_map[control->projectName]);
        }
    }
}

void checkinWidget::InitCurrentDate()
{
    currentDate=QDate::currentDate();
    // Connect the timeout signal to a lambda function
    connect(&timer, &QTimer::timeout,this, &checkinWidget::timerOutTriggered);
    timer.start(5000);
}

void checkinWidget::addItemBtn_clicked()
{
    if(!this->habitForm)
    {
        habitForm=new NewHabitForm;
        connect(habitForm,&NewHabitForm::sendSelectDataToParent,this, &checkinWidget::onReceiveNewHabitFormData);
    }
    if(this->window())
    {
         habitForm->move(this->window()->frameGeometry().topLeft() +this->window()->rect().center() -habitForm->rect().center());//使子窗体居中
    }
    habitForm->show();
}

HabbitItem* checkinWidget::addHabitItem(project_info *project)
{
    HabbitItem *habit =new HabbitItem(project->project_name);
    habit->setIconIndex(project->iconIndex.toInt());

    int index = ui->leftNavigateWidget->layout()->count()-1; // Replace with the desired index where you want to insert the item
    QBoxLayout *boxLayout = dynamic_cast<QBoxLayout*>(ui->leftNavigateWidget->layout());
    if (boxLayout)
    {
        boxLayout->insertWidget(index, dynamic_cast<QWidget*>(habit));
    }
    connect(habit,&HabbitItem::triggerMousePressEvent,this,&checkinWidget::onReceiveHabitMousePressed);
    return habit;
}

void checkinWidget::onReceiveNewHabitFormData(QString name, int iconIndex)
{
    project_info *project=new project_info;
    project->iconIndex=QString::number(iconIndex);
    project->project_name=name;
    addHabitItem(project);
    CheckinConfig::getInstance().updateHabitXml(AddHabit, project);
}

void checkinWidget::onReceiveHabitMousePressed(HabbitItem *habit)
{
    if(habit->isSelected)
    {
        return;
    }
    habit->setStyleSheet("QWidget#mainWidget{background-color:rgba(234,240,255,0.7)}");
    habit->isSelected=true;
    for(int i=0;i< ui->leftNavigateWidget->layout()->count();i++)
    {
        auto control=dynamic_cast<HabbitItem*>(ui->leftNavigateWidget->layout()->itemAt(i)->widget());
        if(control!=nullptr&&control!=habit)
        {
            control->isSelected=false;
            control->setStyleSheet("QWidget#mainWidget{background-color:white}");
        }
    }
    //修改配置文件
    project_info *info=new project_info;
    info->project_name=habit->projectName;
    info->selected=habit->isSelected;

    CheckinConfig::getInstance().updateHabitXml(ChangeSelectedHabit,info);
    //重新加载配置文件
    auto result= CheckinConfig::getInstance().LoadCheckinConfig();
    //加载habititem
    for(auto item :result.project_list)
    {
        if(item->selected)//切换右侧日历本到选中的habit
        {
            ui->calendarWidget->setHabitItem(result.checkin_map[item->project_name],item->project_name,
                    habit->iconIndex);
        }
    }
}

checkinWidget::~checkinWidget()
{
    delete ui;
}
