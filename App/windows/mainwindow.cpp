#include <QMouseEvent>
#include <QScrollBar>
#include <QLabel>
#include <QToolBar>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include "logger.h"
#include "ui_texteditcontainer.h"
#include "noteconfig.h"
#include "thememanager.h"
#include "theme.h"
#include "external/export_api_sniper_sdk.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground); // 设置窗口背景透明
#ifdef Q_OS_MAC
     ui->mainWindowTiitle->setVisible(false);
     Cocoa::setWindowTitleMerged(winId());
     ui->mainPage_layout->setContentsMargins(8,24,8,8);
#endif

#ifdef Q_OS_WIN
     //此句代码将会使边框消失，配合WA_TranslucentBackground可以达到纯透明效果，否则不管控件如何设置透明度，都无法达到效果
     setWindowFlags(Qt::FramelessWindowHint);

     QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
     effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
     effect->setColor(QColor(128,128,128,50));       //设置阴影颜色
     effect->setBlurRadius(25);        //设定阴影的模糊半径，数值越大越模糊
     ui->centralwidget->setGraphicsEffect(effect);
     ui->centralwidget->setContentsMargins(5,5,5,5); //设置间隔,实际就是阴影的宽度

     connect(ui->mainWindowTiitle->minButton, &QToolButton::clicked, this, &MainWindow::showMinimized);
     connect(ui->mainWindowTiitle->maxButton, &QToolButton::clicked, this, &MainWindow::toggleMaximized);
     connect( ui->mainWindowTiitle->closeButton, &QToolButton::clicked, this, &MainWindow::close);
     //追踪鼠标，默认情况是只有当按键按下才会被qt监控，要不发生按下事件时也接收鼠标信号，需手动设置部件追踪鼠标。
     this->setMouseTracking(true);
     ui->mainPage->setMouseTracking(true);
     ui->centralwidget->setMouseTracking(true);
     ui->mainWindowTiitle->setMouseTracking(true);

#endif

    ui->mainPage->setProperty("platform",util::getPlatFormName());
    textEditContainer=new TextEditContainer;
    _checkinWidget =new checkinWidget;
    _themeSwitchWidget =new ThemeSwitchWidget;
    logger->logEdit=ui->loggerTextEdit;
    logger->log(QDir::currentPath());

    ui->tabLayout->addWidget(textEditContainer);
    ui->tabLayout->addWidget(_checkinWidget);
    ui->tabLayout->addWidget(_themeSwitchWidget);

    ui->loggerTextEdit->setVisible(false);
    ui->loggerTextEdit->isAutoHide=false;

    //set the splitter default-ratio,total=9,leftbar=2,editwidget=7.
    ui->controlSplitter->setStretchFactor(0, 2); //代表第0个控件，即leftbar所占比例为2
    ui->controlSplitter->setStretchFactor(1, 7);//代表第1个控件，即textedit所占比例为7.一共是9
    //设置logger输出框，占比为3/8
    ui->mainSplitter->setStretchFactor(0,5);
    ui->mainSplitter->setStretchFactor(1,3);

    //通过配置文件，创建node
    noteconfig::loadConfigXML(ui->treeWidget);

    setAllItemIcon();

    initTopLevelNode();
    //设置左侧按钮icon
    ui->addnewBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->addnewBtn->setIcon(QIcon(":/icons/res/note/addnew.png"));
    ui->addnewBtn->setText("  新建笔记本");
    ui->addnewBtn->setIconSize(QSize(16, 16));
    ui->addnewBtn->setCursor(Qt::PointingHandCursor);

    ui->checkinBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->checkinBtn->setIcon(QIcon(":/icons/res/checkin/checkin.png"));
    ui->checkinBtn->setText("  打卡记录");
    ui->checkinBtn->setIconSize(QSize(16, 16));
    ui->checkinBtn->setCursor(Qt::PointingHandCursor);

    ui->systemSettingBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->systemSettingBtn->setIcon(QIcon(":/icons/res/system/setting.png"));
    ui->systemSettingBtn->setText("  设     置");
    ui->systemSettingBtn->setIconSize(QSize(16, 16));
    ui->systemSettingBtn->setCursor(Qt::PointingHandCursor);

    ui->themeSettingBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->themeSettingBtn->setIcon(QIcon(":/icons/res/system/theme.png"));
    ui->themeSettingBtn->setText("  换     肤");
    ui->themeSettingBtn->setIconSize(QSize(16, 16));
    ui->themeSettingBtn->setCursor(Qt::PointingHandCursor);

    initRightMenu();

    //设置信号槽
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::onTreeWidgetItemClicked);
    connect(ui->treeWidget,&QTreeWidget::currentItemChanged,this,&MainWindow::currentTreeItemChanged);
    connect(textEditContainer->ui->saveBtn,&QToolButton::clicked,this,&MainWindow::onSaveBtn_clicked);
    connect(ui->addnewBtn,&QToolButton::clicked,this,&MainWindow::onAddnewBtn_clicked);
    connect(ui->treeWidget,&QTreeWidget::itemPressed,this,&MainWindow::right_item_pressed);
    connect(rightMenu,&QMenu::aboutToShow,this,&MainWindow::onMenuToShow);
    connect(newNoteAction, SIGNAL(triggered(bool)), this , SLOT(onNewNoteItemClick()));
    connect(newNoteGroupAction, SIGNAL(triggered(bool)), this , SLOT(onNewNoteGroupItemClick()));
    connect(collectNoteAction, SIGNAL(triggered(bool)), this , SLOT(onCollectNoteItemClick()));
    connect(moveNoteAction, SIGNAL(triggered(bool)), this , SLOT(onMoveNoteItemClick()));
    connect(lockAction, SIGNAL(triggered(bool)), this , SLOT(onLockItemClick()));
    connect(deleteNoteAction, SIGNAL(triggered(bool)), this , SLOT(onDeleteNoteItemClick()));
    connect(recoverNoteAction, SIGNAL(triggered(bool)), this , SLOT(onRecoverNoteItemClick()));
    connect(textEditContainer->ui->titleLineEdit,&QLineEdit::editingFinished,this,&MainWindow::onTitleLineEditEditingFinished);
    connect(qApp, &QApplication::aboutToQuit,this ,&MainWindow::onApplicationQuit);
    connect(textEditContainer->ui->logCheck,&QCheckBox::stateChanged,this,&MainWindow::logCheckStateChanged);
    connect(ui->checkinBtn,&QToolButton::clicked,this,&MainWindow::checkinBtn_clicked);
    connect(ui->themeSettingBtn,&QToolButton::clicked,this,&MainWindow::themeSettingBtn_clicked);
    connect(ui->systemSettingBtn,&QToolButton::clicked,this,&MainWindow::systemSettingBtn_clicked);
    bindThemeChangetCallback=std::bind(&MainWindow::themeChangedUiStatus, this);
    ThemeManager::getInstance().registerThemeGlobalEvent(bindThemeChangetCallback);
    _checkinWidget->setVisible(false);
    _themeSwitchWidget->setVisible(false);
}




#pragma region TitleBar-Button-Funciton{

void MainWindow::onSaveBtn_clicked()
{
    if(ui->treeWidget->selectedItems().length()==0)
    {
        return;
    }
    QString fullPath=util::treeItemToFullFilePath(ui->treeWidget->currentItem());

    //创建一个输出文件的文档
    QFile  myfile(fullPath);
    //注意WriteOnly是往文本中写入的时候用，ReadOnly是在读文本中内容的时候用，Truncate表示将原来文件中的内容清空
    if (myfile.open(QFile::WriteOnly|QFile::Truncate))
    {
        QTextStream out(&myfile);
        out<<textEditContainer->ui->textEdit->toHtml()<<Qt::endl;
    }
}
void MainWindow::logCheckStateChanged(int state)
{
    if(state==Qt::Checked)
    {
        ui->loggerTextEdit->setVisible(true);

    }
    else
    {
        ui->loggerTextEdit->setVisible(false);
    }
}
#pragma endregion }



#pragma region Menu-Function{
//right click Menu, new NoteGroup
void MainWindow::onNewNoteGroupItemClick()
{
    if(ui->treeWidget->currentItem()==nullptr)
    {
        return;
    }
    ExtraQTreeWidgetItem *newItem=new ExtraQTreeWidgetItem( ParentNode);
    ((ExtraQTreeWidgetItem*)newItem)->isNewNode=1;
    //Q:为什么传入这个，执行完函数后nodes就被清空了？ A：因为takeChildren这个方法就是移除的作用，一般不要用
   // auto childrens=ui->treeWidget->currentItem()->takeChildren();
    int count=ui->treeWidget->currentItem()->childCount();
    QList<QTreeWidgetItem*> qlist;
    auto f=[&]()
    {
        for(int i=0;i<count;i++)
        {
            qlist.append(ui->treeWidget->currentItem()->child(i));
        }
    };
    f();
    QString newNodeGroupName=util::NoRepeatNodeName(qlist,"新建笔记本");
    newItem->setText(0,newNodeGroupName);
    auto currentNode=ui->treeWidget->currentItem();
    newItem->colorIndex= dynamic_cast<ExtraQTreeWidgetItem*>(currentNode)->colorIndex;
    currentNode->addChild(newItem);
    //set selectedItem to the newItem
    ui->treeWidget->setCurrentItem(newItem);
    //set focus to the right-titleLineEdit, Convenient for users to modify the title
    textEditContainer->ui->titleLineEdit->setFocus();


    //update local xml and html when titleLineEdit finishEdit 。
    //in the function onTitleLineEditEditingFinished()

    setAllItemIcon();
}

//right click Menu ,new note
void MainWindow::onNewNoteItemClick()
{
    if(ui->treeWidget->currentItem()==nullptr)
    {
        return;
    }
    QTreeWidgetItem *newItem=new ExtraQTreeWidgetItem(ChildNode);
    ((ExtraQTreeWidgetItem*)newItem)->isNewNode=1;
    int count=ui->treeWidget->currentItem()->childCount();
    QList<QTreeWidgetItem*> qlist;
    auto f=[&]()
    {
        for(int i=0;i<count;i++)
        {
            qlist.append(ui->treeWidget->currentItem()->child(i));
        }
    };
    f();

    QString newNodeName=util::NoRepeatNodeName(qlist,"无标题");
    newItem->setText(0,newNodeName);
    auto currentNode=ui->treeWidget->currentItem();

    currentNode->addChild(newItem);

    //set selectedItem to the newItem
    ui->treeWidget->setCurrentItem(newItem);
    //set focus to the right-titleLineEdit, Convenient for users to modify the title
    textEditContainer->ui->titleLineEdit->setFocus();

    //update local xml and html when titleLineEdit finishEdit 。
    //in the function onTitleLineEditEditingFinished()

    setAllItemIcon();
}

//右键菜单，删除笔记本操作
void MainWindow::onDeleteNoteItemClick()
{
    ExtraQTreeWidgetItem* currentNode=dynamic_cast<ExtraQTreeWidgetItem*>(ui->treeWidget->currentItem());
    auto currentPath=STORAGE_PATH;
    auto fullPath= util::treeItemToFullFilePath(currentNode); //如d:/sotrage/xxx.html
    //if node is toplevelNode
    if(currentNode->parent()==nullptr)
    {
        if(currentNode->childCount()==0)
        {
            int topIndex= ui->treeWidget->indexOfTopLevelItem(currentNode);
            logger->log( QString::number(topIndex));
            ui->treeWidget->takeTopLevelItem(topIndex);
            noteconfig::updateXml( DeleteNode,currentNode);
            //delete local floder
            QDir dir(fullPath);
            dir.removeRecursively();
            return;
        }
        else
        {
             QMessageBox::warning(this, tr("警告"),tr("\n顶级节点下内容不为空,请先删除子节点后重试!"),QMessageBox::Ok);
             return;
        }
    }

    bool isRecycle=currentNode->parent()->text(0)==NODENAME_RECYLE; //is recycle Node
    //if is recycleNode's child node ,delete directly
    if(isRecycle)
    {
        QFile file(fullPath);
        if(!file.remove())
        {
            QMessageBox::warning(this, tr("错误"),tr("\n文件移动失败,无法完成操作"));
            return;
        }
    }
    else  //若是非回收站的数据
    {
        if(currentNode->nodeType== ParentNode)//父节点
        {
            if(currentNode->childCount()>0)
            {
                QMessageBox::warning(this, tr("警告"),tr("\n无法批量删除,请选中单个笔记进行删除!"),QMessageBox::Ok);
                return;
            }
            else //父节点无子节点
            {
                QDir dir(fullPath);
                if(!dir.removeRecursively())
                {
                    QMessageBox::warning(this, tr("错误"),tr("\n文件移动失败,无法完成操作"));
                    return;
                }
            }
        }
        else
        {
            //移动本地存储文件到回收站
            QString fileName = util::treeItemToFileName(currentNode); //文件名称，如xxx.html
            auto recyclePath=QString("%1/storage/%2/%3").arg(currentPath,NODENAME_RECYLE,fileName);
            bool moveResult= QFile::rename(fullPath,recyclePath); //A路径移动到B路径
            std::string str="delete node and move file "+ std::string(moveResult ? "true": "false") ;
            logger->log(QString::fromStdString(str));
            if(!moveResult)
            {
                 QMessageBox::warning(this, tr("错误"),tr("\n文件移动失败,无法完成操作"));
                 return;
            }
        }
    }
    //delete doc(updateXml) must be ahead of the QTreeWidget'Node delete
    //because updateXml function is depend on the Node struct
    //delete directly if node is parentNode
    if(isRecycle||currentNode->nodeType==ParentNode)
    {
        noteconfig::updateXml(DeleteNode,currentNode);
    }
    else
    {
        noteconfig::updateXml(MoveNode,currentNode,recycleNode);
    }
    currentNode->deleteType=1;
    currentNode->parent()->removeChild(currentNode);//this line will trigger currentTreeItemChanged immediately
    currentNode->deleteType=0;
    if(!isRecycle && currentNode->nodeType==ChildNode) //if parentNode is recycle,not need to add
    {
        recycleNode->addChild(currentNode);
    }
    setAllItemIcon();
}

//right-click Menu, recover Note
void MainWindow::onRecoverNoteItemClick()
{
    //ExtraQTreeWidgetItem* currentNode=dynamic_cast<ExtraQTreeWidgetItem*>(ui->treeWidget->currentItem());
    auto path= util::treeItemToNodeDirPath(ui->treeWidget->currentItem());
    auto currentPath= STORAGE_PATH;

    //移动本地存储文件到回收站
    /*
    QString fileName = util::treeItemToFileName(ui->treeWidget->currentItem()); //文件名称，如xxx.html
    auto recyclePath=QString("%1/storage/回收站/%2").arg(currentPath,fileName);
    bool moveResult= QFile::rename(recyclePath,""); //A路径移动到B路径*/
}

void MainWindow::onCollectNoteItemClick()
{
    logger->log(QString("save menu"));
}

void MainWindow::onMoveNoteItemClick()
{
    logger->log(QString("move menu"));
}

void MainWindow::onLockItemClick()
{
    //std::cout<<"lock menu"<<std::endl;
}

#pragma endregion}

void MainWindow::onAddnewBtn_clicked()
{
    try
    {
        if(!this->newGroupForm)
        {
            newGroupForm=new NewNoteGroupForm;
            connect(newGroupForm,&NewNoteGroupForm::sendParentWindowData,this, &MainWindow::onReceiveNewGroupFormData);
        }
        newGroupForm->move(this->frameGeometry().topLeft() +this->rect().center() -newGroupForm->rect().center());//使子窗体居中
        newGroupForm->show();
    }
    catch(...)
    {
        logger->log(QString("error"));
    }
}

void MainWindow::checkinBtn_clicked()
{
    if(!ui->checkinBtn->isChecked())
    {
        ui->checkinBtn->setChecked(true);
        return;
    }
    _checkinWidget->setVisible(true);
    textEditContainer->setVisible(false);
    _themeSwitchWidget->setVisible(false);
    ui->systemSettingBtn->setChecked(false);
    ui->themeSettingBtn->setChecked(false);
    _checkinWidget->setFocus();
}

void MainWindow::themeSettingBtn_clicked()
{
    if(!ui->themeSettingBtn->isChecked())
    {
        ui->themeSettingBtn->setChecked(true);
        return;
    }
     _themeSwitchWidget->setVisible(true);
     textEditContainer->setVisible(false);
     _checkinWidget->setVisible(false);
     ui->systemSettingBtn->setChecked(false);
     ui->checkinBtn->setChecked(false);
     _themeSwitchWidget->setFocus();
}

void MainWindow::systemSettingBtn_clicked()
{

    ui->themeSettingBtn->setChecked(false);
    ui->checkinBtn->setChecked(false);
    //_themeSwitchWidget->setFocus();
}

void MainWindow::onReceiveNewGroupFormData(QString nodeName,int color_index)
{
    //treewidget添加节点
    int count=ui->treeWidget->topLevelItemCount();
    auto newTopNode=new ExtraQTreeWidgetItem(ParentNode);
    QList<QTreeWidgetItem*> qlist;
    auto f=[&]()
    {
        for(int i=0;i<count;i++)
        {
            qlist.append(ui->treeWidget->topLevelItem(i));
        }
    };
    f();
    QString realName=util::NoRepeatNodeName(qlist, nodeName);  //防止新建顶级节点重名，否则有bug
    newTopNode->setText(0,realName);
    newTopNode->colorIndex= QString::number(color_index);
    ui->treeWidget->insertTopLevelItem(count-2,newTopNode);

    //添加xml节点
    noteconfig::updateXmlAddTopLevelNode(newTopNode,collectNode);

    setAllItemIcon();
    //添加本地文件夹
    auto currentPath= STORAGE_PATH;
    QString dirpath =QString("%1/storage/%2").arg(currentPath,realName);
    QDir *dir = new QDir();
    if (!dir->exists(dirpath))
    {
        dir->mkpath(dirpath);
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter|| event->key() == Qt::Key_Return )
    {
        textEditContainer->ui->textEdit->setFocus();
        QTextCursor tmpCursor = textEditContainer->ui->textEdit->textCursor();
        tmpCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 0);
        textEditContainer->ui->textEdit->setTextCursor(tmpCursor);
    }
}

void MainWindow::initRightMenu()
{
    newNoteAction = new QAction("新建笔记", ui->treeWidget);
    newNoteGroupAction = new QAction("新建笔记本", ui->treeWidget);
    collectNoteAction = new QAction("收藏笔记", ui->treeWidget);
    moveNoteAction = new QAction("移动笔记", ui->treeWidget);
    lockAction = new QAction("添加密码锁", ui->treeWidget);
    deleteNoteAction = new QAction("删除笔记", ui->treeWidget);
    recoverNoteAction=new QAction("恢复笔记",ui->treeWidget);
    rightMenu=new QMenu(ui->treeWidget);
    rightMenu->addAction(newNoteAction);
    rightMenu->addAction(newNoteGroupAction);
    rightMenu->addAction(collectNoteAction);
    rightMenu->addAction(moveNoteAction);
    rightMenu->addAction(lockAction);
    rightMenu->addAction(deleteNoteAction);
    rightMenu->addAction(recoverNoteAction);
    util::ChangeQMenuStyle(*rightMenu);
}

void MainWindow::onMenuToShow()
{
    newNoteAction->setVisible(true);
    newNoteGroupAction->setVisible(true);
    recoverNoteAction->setVisible(true);
    moveNoteAction->setVisible(true);
    lockAction->setVisible(true);
    deleteNoteAction->setVisible(true);
    collectNoteAction->setVisible(true);

    auto item=ui->treeWidget->currentItem();
    auto extraItem=dynamic_cast<ExtraQTreeWidgetItem*>(item);

    if(extraItem->nodeType==ChildNode)
    {
        newNoteAction->setVisible(false);
        newNoteGroupAction->setVisible(false);
        collectNoteAction->setVisible(true);
    }
    else
    {
        newNoteAction->setVisible(true);
        newNoteGroupAction->setVisible(true);
        collectNoteAction->setVisible(false);
    }

    if(item->parent()==nullptr)
    {
        recoverNoteAction->setVisible(false);
    }
    else
    {
        bool isRecycle=item->parent()->text(0)==NODENAME_RECYLE; //is recycle Node
        if(isRecycle)
        {
            recoverNoteAction->setVisible(true);
            collectNoteAction->setVisible(false);
            moveNoteAction->setVisible(false);
            lockAction->setVisible(false);
        }
        else
        {
            recoverNoteAction->setVisible(false);
        }
    }
    if(extraItem==recycleNode)
    {
        newNoteAction->setVisible(false);
        newNoteGroupAction->setVisible(false);
        recoverNoteAction->setVisible(false);
        moveNoteAction->setVisible(false);
        lockAction->setVisible(false);
        deleteNoteAction->setVisible(false);
    }
    if(extraItem==collectNode)
    {
        deleteNoteAction->setVisible(false);
    }
}

void MainWindow::right_item_pressed()
{
    if(QGuiApplication::mouseButtons()!= Qt::RightButton)
    {
        return;
    }
    rightMenu->exec(QCursor::pos());   //菜单弹出位置为鼠标点击位置
}

//currentTreeItemChanged will call this function
//todo: set 3 interval is specific
//but if user modify the interval,can't call this function when currentTreeItemChanged
void MainWindow::setLineVerticalInterval()
{
    if(blockFormat==nullptr)
    {
        blockFormat=new QTextBlockFormat;
        blockFormat->setLineHeight(3,QTextBlockFormat::LineDistanceHeight);
    }
    //to set qtextedit vertical interval 3
    textEditContainer->ui->textEdit->selectAll();
    auto textCursor = textEditContainer->ui->textEdit->textCursor();
    textCursor.setBlockFormat(*blockFormat);
    textEditContainer->ui->textEdit->setTextCursor(textCursor);
    textCursor.clearSelection();//cacle the selection
    textEditContainer->ui->textEdit->setTextCursor(textCursor);
}

void MainWindow::themeChangedUiStatus()
{
    util::ChangeQMenuStyle(*rightMenu);
}


void MainWindow::onTreeWidgetItemClicked(QTreeWidgetItem *item, int column)
{
    //todo 暂时先隐藏
    if(!textEditContainer->isVisible())
    {
        _checkinWidget->setVisible(false);
        _themeSwitchWidget->setVisible(false);
        textEditContainer->setVisible(true);
    }
    ui->checkinBtn->setChecked(false);
    ui->themeSettingBtn->setChecked(false);
    ui->systemSettingBtn->setChecked(false);
}
//切换左侧节点时，保存上一个节点的内容，加载当前节点的内容
void MainWindow::currentTreeItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{

    logger->log(QString("currentTreeItemChanged tirggerd"));
    ExtraQTreeWidgetItem* extraPreviousNode= dynamic_cast<ExtraQTreeWidgetItem*>(previous);

    setLineVerticalInterval();

    //save previous node content to local file
    //when delete node,the focus will be changed,and this function will be trigger.
    //so when the previous node is deleted, it's deletetype is 1 , don't save the previous node content.
    //otherwise, delete node, the local file will be create again.
    if(previous!=nullptr && extraPreviousNode->deleteType==0 && ((ExtraQTreeWidgetItem*)previous)->nodeType== ChildNode)
    {
        auto previewFullPath=util::treeItemToFullFilePath(previous); //如d:/sotrage/xxx.html
        //解析出路径（不含文件名）和文件名
        int first = previewFullPath.lastIndexOf ("/");
        QString dirPath = previewFullPath.left (first); //文件夹路径

        //如果路径不存在，则创建
        QDir* dir = new QDir();
        if(!dir->exists(dirPath)){
            dir->mkpath(dirPath);
        }

        //创建一个输出文件的文档
        QFile  myfile(previewFullPath);
        //注意WriteOnly是往文本中写入的时候用，ReadOnly是在读文本中内容的时候用，Truncate表示将原来文件中的内容清空
        if (myfile.open(QFile::WriteOnly|QFile::Truncate))
        {
            QTextStream out(&myfile);
            out<<textEditContainer->ui->textEdit->toHtml()<<Qt::endl;
        }
        myfile.close();
    }
    if(current==nullptr)
    {
        return;
    }
    //若是笔记本group，则将文本清空
    if(((ExtraQTreeWidgetItem*)current)->nodeType==ParentNode)
    {
        textEditContainer->ui->textEdit->setHtml("");
        textEditContainer->ui->titleLineEdit->setText(current->text(0));
        //not allow to edit the Nodegroup
        textEditContainer->ui->textEdit->setEnabled(false);
        for(auto child_toolBtn :textEditContainer->ui->editTitleBar->children())
        {
            QToolButton* btn=dynamic_cast<QToolButton*>(child_toolBtn);
            if(btn!=nullptr)
            {
                btn->setEnabled(false);
            }
        }
        return;
    }
    else
    {
        textEditContainer->ui->textEdit->setEnabled(true);
        for(auto child_toolBtn :textEditContainer->ui->editTitleBar->children())
        {
            QToolButton* btn=dynamic_cast<QToolButton*>(child_toolBtn);
            if(btn!=nullptr)
            {
                btn->setEnabled(true);
            }
        }
    }
    //load current node‘s title to right-titleLineEdit title
    textEditContainer->ui->titleLineEdit->setText(current->text(0));
    //load current node's local file content to right-textEdit content
    auto fullPath=util::treeItemToFullFilePath(current);
    QFile file(fullPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QByteArray allArray = file.readAll();
    QString allStr = QString(allArray);
    file.close();
    textEditContainer->ui->textEdit->setHtml(allStr);
}

//给回收站&收藏node赋值
//创建stroge默认文件夹
void MainWindow::initTopLevelNode()
{
    int size = ui->treeWidget->topLevelItemCount();
    QTreeWidgetItem *child;
    for (int i = 0; i < size; i++)
    {
        child = ui->treeWidget->topLevelItem(i);
        if(child->text(0)==NODENAME_RECYLE)
        {
            recycleNode=child;
        }
        if(child->text(0)==NODENAME_COLLECT)
        {
            collectNode=child;
        }
        if(child->text(0)==NODENAME_RECYLE||child->text(0)==NODENAME_COLLECT)
        {
            auto path=util::treeItemToFullFilePath(child);
            //创建默认废纸篓路径
            QDir* dir = new QDir();
            if(!dir->exists(path)){
                dir->mkpath(path);
            }
        }
    }

}

//设置所有node的icon
void MainWindow::setAllItemIcon()
{
    int size = ui->treeWidget->topLevelItemCount();
    QTreeWidgetItem *child;
    for (int i = 0; i < size; i++)
    {
        child = ui->treeWidget->topLevelItem(i);
        setItemIcon(dynamic_cast<ExtraQTreeWidgetItem*>(child));
    }
}

void MainWindow::setItemIcon(ExtraQTreeWidgetItem* child)
{
    int childCount = child->childCount();
    auto color=util::colorBtnMap[child->colorIndex.toInt()];
    if(childCount>0||child->nodeType==ParentNode)
    {
        if(child->parent()==nullptr&&child->text(0)==NODENAME_COLLECT) //顶级系统节点-收藏
        {
            child->setIcon(0,QIcon(":/icons/res/note/collect.png"));
        }
        else if(child->parent()==nullptr&&child->text(0)==NODENAME_RECYLE)//顶级系统节点-废纸篓
        {
            child->setIcon(0,QIcon(":/icons/res/note/recycle.png"));
        }
        else
        {
           // QIcon icon= util::CreateColorSvgIcon(":/res/icons/noteparent.svg",QString::fromStdString(color),"0.8");
            auto icon=QIcon(":/icons/res/note/parentnote.png");
            child->setIcon(0,icon);
        }
        for (int j = 0; j < childCount; ++j)
        {
            ExtraQTreeWidgetItem * grandson = dynamic_cast<ExtraQTreeWidgetItem*>(child->child(j));
            if(grandson->childCount()>0||grandson->nodeType== ParentNode)
            {
                setItemIcon(grandson);
            }
            else
            {
                QIcon icon= util::CreateColorSvgIcon(":/icons/res/note/notechild.svg", QString::fromStdString(color));
                grandson->setIcon(0,QIcon(icon));
            }
        }
    }
    else
    {
        child->setIcon(0,QIcon(":/icons/res/note/childnote.png"));
    }
}

void MainWindow::onTitleLineEditEditingFinished()
{
    logger->log(QString("onTitleLineEditEditingFinished"));
    if (textEditContainer->ui->titleLineEdit->text().length() == 0)
    {
        return;
    }
    auto currentItem=ui->treeWidget->currentItem();
    if(currentItem->parent()==nullptr)
    {
        if(currentItem->text(0)==NODENAME_COLLECT||currentItem->text(0)==NODENAME_RECYLE)
        {
            QMessageBox::warning(this, tr("警告"),tr("\n系统节点无法重命名!"),QMessageBox::Ok);
            return;
        }
    }

    auto oldPath=util::treeItemToNodePath(currentItem);
    auto fileFullPath=util::treeItemToFullFilePath(currentItem);
    auto fileNewPath=util::treeItemToNodeDirPath(currentItem);
    auto oldName=currentItem->text(0);

    currentItem->setText(0, textEditContainer->ui->titleLineEdit->text());


    // update the local file change
    auto extraItem =dynamic_cast<ExtraQTreeWidgetItem *>(currentItem);
    OperationType type = extraItem->isNewNode==1?
                (extraItem->nodeType == ChildNode ?
                     AddNode:
                     AddNodeGroup)
              :RenameNode;
    if(type==RenameNode)
    {
        if(((ExtraQTreeWidgetItem*)currentItem)->nodeType==ChildNode)
        {
            QFile file(fileFullPath);
            file.rename(fileNewPath+"/"+textEditContainer->ui->titleLineEdit->text()+".html");
        }
        else
        {
            QString parentFullPath = QString("%1/storage").arg(STORAGE_PATH);

            if(currentItem->parent()!=nullptr)
            {
                parentFullPath=util::treeItemToFullFilePath(currentItem->parent());
            }
            QString newDir=QString("%1/%2").arg(parentFullPath,textEditContainer->ui->titleLineEdit->text());
            QString oldDir=QString("%1/%2").arg(parentFullPath,oldName);
            QDir _dir(oldDir);
            logger->log(oldDir);
            logger->log(newDir);
            if (_dir.exists())
            {
                _dir.rename(oldDir, newDir);
            }
        }
        noteconfig::updateXmlRenameNode(oldPath,currentItem);
        return;
    }

    noteconfig::updateXml(type, currentItem->parent(),currentItem);

    if (type == AddNodeGroup)
    {
        //新增本地文件夹
        QString dirpath = util::treeItemToFullFilePath(extraItem);
        QDir *dir = new QDir();
        if (!dir->exists(dirpath))
        {
            dir->mkpath(dirpath);
        }
        extraItem->isNewNode=0;//reset isNewNode status
    }
    else if (type ==  AddNode)
    {
        //创建本地空文档html
        QString filePath = util::treeItemToFullFilePath(currentItem);
        QFile myfile(filePath);
        //注意WriteOnly是往文本中写入的时候用，ReadOnly是在读文本中内容的时候用，Truncate表示将原来文件中的内容清空
        if (myfile.open(QFile::WriteOnly))
        {
            myfile.close();
        }
        extraItem->isNewNode=0;//reset isNewNode status
    }
}

void MainWindow::onApplicationQuit()
{
    if(this->newGroupForm!=nullptr)
    {
        this->newGroupForm->close();
    }
    //save qtextedit  modify data
    auto node =ui->treeWidget->currentItem();
    if(node==nullptr||dynamic_cast<ExtraQTreeWidgetItem*>(node)->nodeType== ParentNode)
    {
        return;
    }
    auto fullPath=util::treeItemToFullFilePath(node); //如d:/sotrage/xxx.html
    //解析出路径（不含文件名）和文件名
    int first = fullPath.lastIndexOf ("/");
    QString dirPath = fullPath.left(first); //文件夹路径

    //如果路径不存在，则创建
    QDir dir ;
    if(!dir.exists(dirPath)){
        dir.mkpath(dirPath);
    }

    //创建一个输出文件的文档
    QFile  myfile(fullPath);
    //注意WriteOnly是往文本中写入的时候用，ReadOnly是在读文本中内容的时候用，Truncate表示将原来文件中的内容清空
    if (myfile.open(QFile::WriteOnly|QFile::Truncate))
    {
        QTextStream out(&myfile);
        out<<textEditContainer->ui->textEdit->toHtml()<<Qt::endl;
    }
    myfile.close();
}


void MainWindow::toggleMaximized()
{
    QString color= currentTheme["CONTROL_TEXT"];
    if (isMaximized())
    {
        ui->centralwidget->setContentsMargins(5,5,5,5); //设置间隔,实际就是阴影的宽度
        ui->mainWindowTiitle->maxButton->setIcon(util::CreateColorSvgIcon(":/icons/res/system/win_max.svg",color));
        showNormal();
#ifdef Q_OS_WIN
        auto baseground=currentTheme["BACKGROUND_COLOR1"];
        auto baseground2=currentTheme["BACKGROUND_COLOR2"];
        setStyleSheet(QString("QWidget#mainWindowTiitle[platform=windows]{background-color:%1;border-top-left-radius:7px;border-top-right-radius:7px;}"
                              "QWidget#mainPage[platform=windows]{background-color:%2;border-bottom-left-radius:7px;border-bottom-right-radius:7px;}").arg(baseground,baseground2));
#endif
    }
    else
    {
        ui->centralwidget->setContentsMargins(0,0,0,0); //设置间隔,实际就是阴影的宽度
        ui->mainWindowTiitle->maxButton->setIcon(util::CreateColorSvgIcon(":/icons/res/system/win_restore.svg",color));
#ifdef Q_OS_WIN
        auto baseground=currentTheme["BACKGROUND_COLOR1"];
        auto baseground2=currentTheme["BACKGROUND_COLOR2"];
        setStyleSheet(QString("QWidget#mainWindowTiitle[platform=windows]{background-color:%1;border-top-left-radius:0px;border-top-right-radius:0px;}"
        "QWidget#mainPage[platform=windows]{background-color:%2;border-bottom-left-radius:0px;border-bottom-right-radius:0px;}").arg(baseground,baseground2));
#endif
        showMaximized();
    }
}



void MainWindow::connected_success()
{
    logger->log("连接成功!");
    send_message_export("GET_ALL_FILE\n");
}


#ifdef Q_OS_WIN
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        const QPoint p = event->pos();
        const QPoint globalPos = event->globalPosition().toPoint();
        const QPoint baseTopLeft = mapToGlobal(QPoint(0, 0));
        const QRect frameRect = frameGeometry();


        // Check if the mouse press is within the resize border region (e.g., 8 pixels)
        if (p.x()>MOUSE_MARGIN && p.y()< MOUSE_MARGIN && p.x()<this->width()-MOUSE_MARGIN)
        {
            isResizingTop = true;
            resizingOffset = frameRect.topLeft() - globalPos;
            event->accept();
        }
        else if (p.x()>MOUSE_MARGIN && p.x() < this->width()-MOUSE_MARGIN && p.y() > this->height()-MOUSE_MARGIN)
        {
            isResizingBottom = true;
            resizingOffset = frameRect.bottomLeft() - globalPos;
            event->accept();
        }
        else if (p.x() < MOUSE_MARGIN && p.y() < this->height()-MOUSE_MARGIN && p.y()>MOUSE_MARGIN)
        {
            isResizingLeft = true;
            resizingOffset = frameRect.topLeft() - globalPos;
            event->accept();
        }
        else if (p.x() > this->width()-MOUSE_MARGIN && p.y() > MOUSE_MARGIN && p.y()<this->height()-MOUSE_MARGIN)
        {
            isResizingRight = true;
            resizingOffset = frameRect.topRight() - globalPos;
            event->accept();
        }
        else if (p.x()<MOUSE_MARGIN && p.y() < MOUSE_MARGIN)
        {
            isResizingTopLeft = true;
            resizingOffset = frameRect.topLeft() - globalPos;
            event->accept();
        }
        else if (p.x() > this->width()-MOUSE_MARGIN && p.y() < MOUSE_MARGIN)
        {
            isResizingTopRight = true;
            resizingOffset = frameRect.topRight() - globalPos;
            event->accept();
        }
        else if (p.x() < MOUSE_MARGIN && p.y() > this->height()-MOUSE_MARGIN)
        {
            isResizingBottomLeft = true;
            resizingOffset = frameRect.bottomLeft() - globalPos;
            event->accept();
        }
        else if (p.x() > this->width()-MOUSE_MARGIN && p.y() > this->height()-MOUSE_MARGIN)
        {
            isResizingBottomRight = true;
            resizingOffset = frameRect.bottomRight() - globalPos;
            event->accept();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isResizingTop = false;
        isResizingBottom = false;
        isResizingLeft = false;
        isResizingRight = false;
        isResizingTopLeft = false;
        isResizingTopRight = false;
        isResizingBottomLeft = false;
        isResizingBottomRight = false;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint p = event->globalPosition().toPoint();
    const QPoint windowPos = mapToGlobal(QPoint(0, 0));
    const QRect frameRect = frameGeometry();


    if (isResizingTop)
    {
        const QPoint newTopLeft = p + resizingOffset;
        const int newHeight = height() + (frameRect.top() - newTopLeft.y());
        if(newHeight>this->minimumHeight())
        {
           setGeometry(frameRect.x(), newTopLeft.y(), frameRect.width(), newHeight);
        }
    }
    else if (isResizingBottom)
    {
        const QPoint newBottomLeft = p + resizingOffset;
        const int newHeight = newBottomLeft.y() - frameRect.top();
        setGeometry(frameRect.x(), frameRect.y(), frameRect.width(), newHeight);
    }
    else if (isResizingLeft)
    {
        // resizingOffset = frameRect.topLeft() - globalPos;
        const QPoint newTopLeft = p + resizingOffset;
        const int newWidth = width() + (frameRect.left() - newTopLeft.x());
        if(newWidth>this->minimumWidth())
        {
            setGeometry(newTopLeft.x(), frameRect.y(), newWidth, frameRect.height());
        }
    }
    else if (isResizingRight)
    {
        const QPoint newTopRight = p + resizingOffset;
        const int newWidth = newTopRight.x() - frameRect.left();
        setGeometry(frameRect.x(), frameRect.y(), newWidth, frameRect.height());
    }
    else if (isResizingTopLeft)
    {
        const QPoint newTopLeft = p + resizingOffset;
        const int newWidth = width() + (frameRect.left() - newTopLeft.x());
        const int newHeight = height() + (frameRect.top() - newTopLeft.y());
        setGeometry(newTopLeft.x(), newTopLeft.y(), newWidth, newHeight);
    }
    else if (isResizingTopRight)
    {
        const QPoint newTopRight = p + resizingOffset;
        const int newWidth = newTopRight.x() - frameRect.left();
        const int newHeight = height() + (frameRect.top() - newTopRight.y());
        setGeometry(frameRect.x(), newTopRight.y(), newWidth, newHeight);
    }
    else if (isResizingBottomLeft)
    {
        const QPoint newBottomLeft = p + resizingOffset;
        const int newWidth = width() + (frameRect.left() - newBottomLeft.x());
        const int newHeight = newBottomLeft.y() - frameRect.top();
        setGeometry(newBottomLeft.x(), frameRect.y(), newWidth, newHeight);
    }
    else if (isResizingBottomRight)
    {
        const QPoint newBottomRight = p + resizingOffset;
        const int newWidth = newBottomRight.x() - frameRect.left();
        const int newHeight = newBottomRight.y() - frameRect.top();
        setGeometry(frameRect.x(), frameRect.y(), newWidth, newHeight);
    }

    // Set cursor shape based on the mouse position
    const QPoint pos = event->pos();
    Qt::CursorShape cursorShape = Qt::ArrowCursor;
    logger->log("x:"+QString::number(pos.x())+"y:"+QString::number(pos.y()));
    if (pos.x()>MOUSE_MARGIN && pos.y()< MOUSE_MARGIN && pos.x()<this->width()-MOUSE_MARGIN)
    {
          cursorShape = Qt::SizeVerCursor; // Vertical resize
    }
    else if (pos.x()>MOUSE_MARGIN && pos.x() < this->width()-MOUSE_MARGIN && pos.y() > this->height()-MOUSE_MARGIN)
    {
           cursorShape = Qt::SizeVerCursor; // Vertical resize
    }
    else if (pos.x() < MOUSE_MARGIN && pos.y() < this->height()-MOUSE_MARGIN && pos.y()>MOUSE_MARGIN)
    {
        cursorShape = Qt::SizeHorCursor; // Horizontal resize
    }
    else if (pos.x() > this->width()-MOUSE_MARGIN && pos.y() > MOUSE_MARGIN && pos.y()<this->height()-MOUSE_MARGIN)
    {
         cursorShape = Qt::SizeHorCursor; // Horizontal resize
    }
    else if (pos.x()<MOUSE_MARGIN && pos.y() < MOUSE_MARGIN)
    {
             cursorShape = Qt::SizeFDiagCursor; // Diagonal resize (top-left to bottom-right)
    }
    else if (pos.x() > this->width()-MOUSE_MARGIN && pos.y() > this->height()-MOUSE_MARGIN)
    {
            cursorShape = Qt::SizeFDiagCursor; // Diagonal resize (top-left to bottom-right)
    }
    else if (pos.x() < MOUSE_MARGIN && pos.y() > this->height()-MOUSE_MARGIN)
    {
        cursorShape = Qt::SizeBDiagCursor; // Diagonal resize (top-right to bottom-left)
    }
    else if (pos.x() > this->width()-MOUSE_MARGIN && pos.y() < MOUSE_MARGIN)
    {
        cursorShape = Qt::SizeBDiagCursor; // Diagonal resize (top-right to bottom-left)
    }
    logger->log("cursor:"+QString::number(cursorShape));
    setCursor(cursorShape);
}
#endif


MainWindow::~MainWindow()
{
    delete ui;
}

