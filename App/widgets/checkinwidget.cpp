#include <QFile>
#include <QXmlStreamReader>
#include "checkinwidget.h"
#include "ui_checkinwidget.h"
#include "util.h"
#include "logger.h"


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

    std::vector<project_info*> projects= LoadCheckinConfig();
    for(auto item :projects)
    {
        addHabitItem(item);
    }
}

std::vector<project_info*> checkinWidget::LoadCheckinConfig()
{
    //设置输入文件
    QString filePath =STORAGE_PATH+ CHECKIN_CONFIG_PATH;
    QFile inputfile(filePath);
    std::vector<project_info*> vector_project;
    if(!inputfile.open(QIODevice::ReadOnly))
    {
        logger->log(QString("loadConfigXML: Open checkin.xml file failed"));
        return vector_project;
    }
    QXmlStreamReader reader(&inputfile);

    bool isTotalProjectRange=false;
    //开始解析
    while (!reader.atEnd())
    {
        QXmlStreamReader::TokenType token = reader.readNext();
        switch (token)
        {
        case QXmlStreamReader::StartDocument:
            break;
        case QXmlStreamReader::EndDocument:
            break;
        case QXmlStreamReader::StartElement:
            if(!reader.name().toString().isEmpty())
            {
                project_info *project = new project_info();
                if(reader.name().toString()==NODENAME_TOTALPROJECT)
                {
                    isTotalProjectRange=true;
                }
                else if(reader.name().toString()==NODENAME_PROJECT)
                {
                    if(isTotalProjectRange)
                    {
                        foreach (const QXmlStreamAttribute & attribute, reader.attributes())
                        {
                            logger->log(attribute.name().toString());
                            logger->log(attribute.value().toString());
                            if(attribute.name().toString()==ATTRIBUTE_NAME)
                            {
                                project->project_name= attribute.value().toString();
                            }
                            if(attribute.name().toString()==ATTRIBUTE_ICONINDEX)
                            {
                                project->iconIndex= attribute.value().toString();
                            }
                        }
                        vector_project.push_back(project);
                    }
                }
            }
            break;
        case QXmlStreamReader::EndElement:
            if(reader.name().toString()==NODENAME_TOTALPROJECT)
            {
                isTotalProjectRange=false;
            }
            if(reader.name().toString()=="root")
            {
                continue;
            }
            break;
        case QXmlStreamReader::Characters:
            break;
        }
    }

    //是否是正常结束
    if (reader.error())
    {
        QString str="Error: "+reader.errorString()+"in file test.xml at line "+QString::number(reader.lineNumber())+
                ",column "+QString::number(reader.columnNumber());
        logger->log(str);
        vector_project.clear();
        return vector_project;
    }
    //关闭文件
    inputfile.close();
    return vector_project;
}


void checkinWidget::updateXml(HabitOperationType type,project_info * project)
{
    QString filePath =STORAGE_PATH + CHECKIN_CONFIG_PATH;
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        logger->log(QString("updateXml:open local xml failed"));
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        logger->log(QString("updateXml:set doc content form file failed"));
        file.close();
        return;
    }
    file.close();
    if(type==AddHabit)
    {
        auto path= NODENAME_TOTALPROJECT;
        QDomNode parentDomElement=util::selectSingleNode(path,&doc);
        QDomElement newDomElement=doc.createElement(NODENAME_PROJECT);
        parentDomElement.appendChild(newDomElement);
        newDomElement.setAttribute(ATTRIBUTE_NAME,project->project_name);
        newDomElement.setAttribute(ATTRIBUTE_ICONINDEX,project->iconIndex);
        newDomElement.setAttribute(ATTRIBUTE_TYPE,project->type);
    }
    else if(type==DeleteNode)
    {
       // auto path=util::treeItemToNodePath(currentNode);
        //QDomNode domElement=selectSingleNode(path,&doc);
        ///domElement.parentNode().removeChild(domElement);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
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

void checkinWidget::addHabitItem(project_info *project)
{
    HabbitItem *habit =new HabbitItem;
    habit->setIconIndex(project->iconIndex.toInt());
    habit->setProjectName(project->project_name);
    int index = ui->leftNavigateWidget->layout()->count()-1; // Replace with the desired index where you want to insert the item
    QBoxLayout *boxLayout = dynamic_cast<QBoxLayout*>(ui->leftNavigateWidget->layout());
    if (boxLayout)
    {
        boxLayout->insertWidget(index, dynamic_cast<QWidget*>(habit));
    }
    connect(habit,&HabbitItem::triggerMousePressEvent,this,&checkinWidget::onReceiveHabitMousePressed);
}

void checkinWidget::onReceiveNewHabitFormData(QString name, int iconIndex)
{
    project_info *project=new project_info;
    project->iconIndex=QString::number(iconIndex);
    project->project_name=name;
    addHabitItem(project);
    updateXml(AddHabit, project);
}

void checkinWidget::onReceiveHabitMousePressed(HabbitItem *habit,QWidget *weekWidget)
{
    habit->setStyleSheet("QWidget#mainWidget{background-color:rgba(234,240,255,0.7)}");

    for(int i=0;i< ui->leftNavigateWidget->layout()->count();i++)
    {
        auto control=dynamic_cast<HabbitItem*>(ui->leftNavigateWidget->layout()->itemAt(i)->widget());
        if(control!=nullptr&&control!=habit)
        {
            control->setStyleSheet("QWidget#mainWidget{background-color:white}");

        }
    }

}
checkinWidget::~checkinWidget()
{
    delete ui;
}
