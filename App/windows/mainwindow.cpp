#include "mainwindow.h"
#include "logger.h"
#include "ui_mainwindow.h"
#include "util.h"
#include <QMouseEvent>
#include "logger.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainPage->setStyleSheet("QWidget#mainPage{background-color:#FFFFFF}");
    this->setStyleSheet("QTreeWidget::item{height:25px;}");

    //设置左侧侧边栏样式
    ui->leftBar->setStyleSheet("QAbstractButton{min-height:17px;max-height:17px;margin:0px;border:none;} "
                               "QAbstractButton#addnewBtn{min-height:20px;max-height:20px;}"
                               "QWidget#leftBar{background-color:#FFFFFF} ");
    ui->titleBar->setStyleSheet("QToolButton{border:none;} "
                                "QToolButton:checked{background-color:rgb(218, 218, 218)}"
                                "QToolButton:hover{background-color:rgb(218, 218, 218)}"
                                "QWidget#titleBar{background-color:#FFFFFF}");

    ui->editWidget->setStyleSheet("QWidget#editWidget{background-color:#FFFFFF}");
    //set titleLineEdit stylesheet
    ui->titleLineEdit->setStyleSheet("border: 0px;");
    ui->textEdit->setStyleSheet("border:0px; background-color:#FFFFFF");

    ui->editWidget->layout()->setSpacing(0);
    //设置mainPage内部控件间距为5
    ui->mainPage->layout()->setSpacing(5);
    //设置editWidget内部左侧边距
    ui->editWidget->setContentsMargins(5,0,0,0);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setColumnWidth(0,40);

    ui->treeWidget->setHeaderLabels(QStringList()<<"first");
    ui->treeWidget->header()->setStretchLastSection(false);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //隐藏标题栏
    ui->treeWidget->header()->setVisible(false);


    //设置背景色为透明
    ui->treeWidget->setStyleSheet("background-color:transparent;");

    //下面这句代码 修改选中行的颜色，但是改的不是很完美，故先注释
    //setStyleSheet("QTreeWidget::item{height:25px;} QTreeView::branch::selected{background-color:#5087E5;} QTreeView::item::selected{background-color:#5087E5;}");

    //设置不同层次菜单的缩进
    ui->treeWidget->setIndentation(9);
    //设置左侧容器内部margin
    ui->leftBar->setContentsMargins(10,20,0,0);
    ui->leftBar->layout()->setSpacing(15);
    //设置边框不可见
    ui->treeWidget->setFrameStyle(QFrame::NoFrame);
    //通过配置文件，创建node
    config->loadConfigXML(ui->treeWidget);

    setAllItemIcon();

    initTopLevelNode();
    //设置左侧按钮icon
    ui->addnewBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->addnewBtn->setIcon(QIcon(":/res/icons/addnew.png"));
    ui->addnewBtn->setIconSize(QSize(22, 22));
    ui->addnewBtn->setCursor(Qt::PointingHandCursor);

    //设置标题栏按钮
    ui->boldBtn->setIcon(QIcon(":/res/icons/bold.png"));
    ui->boldBtn->setToolTip("加粗");
    ui->italicBtn->setIcon(QIcon(":/res/icons/italic.png"));
    ui->italicBtn->setToolTip("斜体");
    ui->colorBtn->setIcon(QIcon(":/res/icons/color.png"));
    ui->colorBtn->setToolTip("颜色");
    ui->underlineBtn->setIcon(QIcon(":/res/icons/underline.png"));
    ui->underlineBtn->setToolTip("下划线");
    ui->pictureBtn->setIcon(QIcon(":/res/icons/img.png"));
    ui->pictureBtn->setToolTip("添加图片");
    ui->saveBtn->setIcon(QIcon(":/res/icons/save.png"));
    ui->saveBtn->setToolTip("保存");
    ui->undoBtn->setIcon(QIcon(":/res/icons/undo.png"));
    ui->undoBtn->setToolTip("撤回");
    ui->fontAddBtn->setIcon(QIcon(":/res/icons/fontAdd.png"));
    ui->fontAddBtn->setToolTip("增大字号");
    ui->fontReduceBtn->setIcon(QIcon(":/res/icons/fontReduce.png"));
    ui->fontReduceBtn->setToolTip("减小字号");
    initfontCombobox();
    initRightMenu();


    //设置信号槽
    connect(ui->boldBtn,SIGNAL(clicked()),this,SLOT(boldBtn_clicked()));
    connect(ui->italicBtn,SIGNAL(clicked()),this,SLOT(italicBtn_clicked()));
    connect(ui->underlineBtn,SIGNAL(clicked()),this,SLOT(underlineBtn_clicked()));
    connect(ui->colorBtn,SIGNAL(clicked()),this,SLOT(colorBtn_clicked()));
    connect(ui->pictureBtn,&QToolButton::clicked,this,&MainWindow::onPictureBtn_clicked);
    connect(ui->undoBtn,&QToolButton::clicked,this,&MainWindow::onUndoBtn_clicked);
    connect(ui->saveBtn,&QToolButton::clicked,this,&MainWindow::onSaveBtn_clicked);
    connect(ui->addnewBtn,&QToolButton::clicked,this,&MainWindow::onAddnewBtn_clicked);
    connect(ui->fontAddBtn,&QToolButton::clicked,this,&MainWindow::onFontAddBtn_clicked);
    connect(ui->fontReduceBtn,&QToolButton::clicked,this,&MainWindow::onFontReduceBtn_clicked);
    connect(ui->fontComboBox, &QComboBox::currentIndexChanged,this, &MainWindow::comboBoxCurrentIndexChanged);
    connect(ui->treeWidget,&QTreeWidget::currentItemChanged,this,&MainWindow::currentTreeItemChanged);
    connect(ui->textEdit,&QTextEdit::cursorPositionChanged,this,&MainWindow::textEditCursorPositionChanged);
    connect(ui->treeWidget,&QTreeWidget::itemPressed,this,&MainWindow::right_item_pressed);
    connect(rightMenu,&QMenu::aboutToShow,this,&MainWindow::onMenuToShow);
    connect(newNoteAction, SIGNAL(triggered(bool)), this , SLOT(onNewNoteItemClick()));
    connect(newNoteGroupAction, SIGNAL(triggered(bool)), this , SLOT(onNewNoteGroupItemClick()));
    connect(collectNoteAction, SIGNAL(triggered(bool)), this , SLOT(onCollectNoteItemClick()));
    connect(moveNoteAction, SIGNAL(triggered(bool)), this , SLOT(onMoveNoteItemClick()));
    connect(lockAction, SIGNAL(triggered(bool)), this , SLOT(onLockItemClick()));
    connect(deleteNoteAction, SIGNAL(triggered(bool)), this , SLOT(onDeleteNoteItemClick()));
    connect(recoverNoteAction, SIGNAL(triggered(bool)), this , SLOT(onRecoverNoteItemClick()));
    connect(ui->titleLineEdit,&QLineEdit::editingFinished,this,&MainWindow::onTitleLineEditEditingFinished);
    connect(qApp, &QApplication::aboutToQuit,this ,&MainWindow::onApplicationQuit);
}


#pragma region TitleBar-Button-Funciton{
void MainWindow::boldBtn_clicked()
{
    QTextCharFormat fmt;
    if(ui->boldBtn->isChecked())
    {
        fmt.setFontWeight(QFont::Bold);
    }
    else
    {
        fmt.setFontWeight(QFont::Normal);
    }
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::italicBtn_clicked()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(ui->italicBtn->isChecked());
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::underlineBtn_clicked()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(ui->underlineBtn->isChecked());
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::colorBtn_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue,this);
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt=ui->textEdit->currentCharFormat();
        fmt.setForeground(color);
        ui->textEdit->mergeCurrentCharFormat(fmt);
    }
}

void MainWindow::onUndoBtn_clicked()
{
    ui->textEdit->undo();
}

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
        out<<ui->textEdit->toHtml()<<Qt::endl;
    }
}

void MainWindow::onPictureBtn_clicked()
{
    InsertImageDialog();
}

void MainWindow::InsertImageDialog()
{
    QString selectFilter="IMAGE (*.png *.jpg *jpeg *.bmp *.gif)\n";
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                "/", tr("IMAGE (*.png *.jpg *jpeg *.bmp *.gif)\n"),&selectFilter);

    QUrl Uri ( QString ( "file://%1" ).arg ( file ) );
    QImage image = QImageReader ( file ).read();

    QTextDocument * textDocument = ui->textEdit->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);
}

void MainWindow::onAddnewBtn_clicked()
{
    if(this->newGroupForm==NULL)
    {
        newGroupForm=new NewNoteGroupForm;
        connect(newGroupForm,&NewNoteGroupForm::sendParentWindowData,this, &MainWindow::onReceiveNewGroupFormData);
    }

    newGroupForm->move(this->frameGeometry().topLeft() +this->rect().center() -newGroupForm->rect().center());//使子窗体居中
    newGroupForm->show();

}

void MainWindow::onFontAddBtn_clicked()
{
    int realIndex=ui->fontComboBox->currentIndex();
    ui->fontComboBox->setCurrentIndex(realIndex==util::fontVector.size()-1?realIndex:realIndex+1);
}

void MainWindow::onFontReduceBtn_clicked()
{
    int realIndex=ui->fontComboBox->currentIndex();
    ui->fontComboBox->setCurrentIndex(realIndex==0?0:realIndex-1);
}

void MainWindow::initfontCombobox()
{
    ui->fontComboBox->clear();
    for(const QString &afont : util::fontVector)
    {
        ui->fontComboBox->addItem(afont);
    }
}

//change comboBox's selectIndex when move cursor
void MainWindow::textEditCursorPositionChanged()
{
    QTextCursor cursor=ui->textEdit->textCursor();
    if(cursor.hasSelection())
    {
        //prevent font be same when user  select All text
        //because select texts will change the cursor,then change the combobox's selectedIndex.
        //then trigger the comboBoxCurrentIndexChanged function,and all the text'font will be same
        return;
    }
    QFont font=cursor.charFormat().font();
    QString font_size =QString::number(font.pointSize());
    auto index=std::find(util::fontVector.begin(),util::fontVector.end(),font_size);
    if(index!= util::fontVector.end())
    {
         int realIndex=std::distance(util::fontVector.begin(),index);
         ui->fontComboBox->setCurrentIndex(realIndex);
    }
    logger->log(std::string("textEditCursorPositionChanged"));
}

//update current cursor font-size when change comboBox's selectIndex
void MainWindow::comboBoxCurrentIndexChanged()
{
    auto current_font= ui->fontComboBox->currentText();
    logger->log("current font:" +current_font);
    QFont font = ui->textEdit->currentFont();
    font.setPointSize(current_font.toInt());
    ui->textEdit->setCurrentFont(font);
}

#pragma endregion }



#pragma region Menu-Function{
//right click Menu, new NoteGroup
void MainWindow::onNewNoteGroupItemClick()
{
    if(ui->treeWidget->currentItem()==NULL)
    {
        return;
    }
    ExtraQTreeWidgetItem *newItem=new ExtraQTreeWidgetItem(BaseInfo::Parent);
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
    ui->titleLineEdit->setFocus();


    //update local xml and html when titleLineEdit finishEdit 。
    //in the function onTitleLineEditEditingFinished()

    setAllItemIcon();
}

//right click Menu ,new note
void MainWindow::onNewNoteItemClick()
{
    if(ui->treeWidget->currentItem()==NULL)
    {
        return;
    }
    QTreeWidgetItem *newItem=new ExtraQTreeWidgetItem(BaseInfo::Child);
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
    ui->titleLineEdit->setFocus();

    //update local xml and html when titleLineEdit finishEdit 。
    //in the function onTitleLineEditEditingFinished()

    setAllItemIcon();
}

//右键菜单，删除笔记本操作
void MainWindow::onDeleteNoteItemClick()
{
    ExtraQTreeWidgetItem* currentNode=dynamic_cast<ExtraQTreeWidgetItem*>(ui->treeWidget->currentItem());
    auto currentPath= QCoreApplication::applicationDirPath();
    auto fullPath= util::treeItemToFullFilePath(currentNode,currentNode->nodeType); //如d:/sotrage/xxx.html
    //if node is toplevelNode
    if(currentNode->parent()==NULL)
    {
        if(currentNode->childCount()==0)
        {
            int topIndex= ui->treeWidget->indexOfTopLevelItem(currentNode);
            logger->log( QString::number(topIndex));
            ui->treeWidget->takeTopLevelItem(topIndex);
            config->updateXml(BaseInfo::DeleteNode,currentNode);
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
        file.remove();
    }
    else
    {
        //若是非回收站的数据
        if(currentNode->nodeType==BaseInfo::Parent)//父节点
        {
            if(currentNode->childCount()>0)
            {
                QMessageBox::warning(this, tr("警告"),tr("\n无法批量删除,请选中单个笔记进行删除!"),QMessageBox::Ok);
                return;
            }
            else
            {
                QDir dir(fullPath);
                dir.removeRecursively();
            }
        }
        else
        {
            //移动本地存储文件到回收站
            QString fileName = util::treeItemToFileName(currentNode); //文件名称，如xxx.html
            auto recyclePath=QString("%1/storage/%2/%3").arg(currentPath,NODENAME_RECYLE,fileName);
            bool moveResult= QFile::rename(fullPath,recyclePath); //A路径移动到B路径
            std::string str="delete node and move file "+ std::string(moveResult ? "true": "false") ;
            logger->log(str);
        }
    }
    //delete doc(updateXml) must be ahead of the QTreeWidget'Node delete
    //because updateXml function is depend on the Node struct
    //delete directly if node is parentNode
    if(isRecycle||currentNode->nodeType==BaseInfo::Parent)
    {
        config->updateXml(BaseInfo::DeleteNode,currentNode);
    }
    else
    {
        config->updateXml(BaseInfo::MoveNode,currentNode,recycleNode);
    }
    currentNode->deleteType=1;
    currentNode->parent()->removeChild(currentNode);//this line will trigger currentTreeItemChanged immediately
    currentNode->deleteType=0;
    if(!isRecycle && currentNode->nodeType==BaseInfo::Child) //if parentNode is recycle,not need to add
    {
        recycleNode->addChild(currentNode);
    }
    setAllItemIcon();
}

//right-click Menu, recover Note
void MainWindow::onRecoverNoteItemClick()
{
    ExtraQTreeWidgetItem* currentNode=dynamic_cast<ExtraQTreeWidgetItem*>(ui->treeWidget->currentItem());
    auto path= util::treeItemToNodeDirPath(ui->treeWidget->currentItem());
    auto currentPath= QCoreApplication::applicationDirPath();

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

void MainWindow::onReceiveNewGroupFormData(QString nodeName,int color_index)
{
    //treewidget添加节点
    int count=ui->treeWidget->topLevelItemCount();
    auto newTopNode=new ExtraQTreeWidgetItem(BaseInfo::Parent);
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
    config->updateXmlAddTopLevelNode(newTopNode,collectNode);

    setAllItemIcon();
    //添加本地文件夹
    auto currentPath= QCoreApplication::applicationDirPath();
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
        ui->textEdit->setFocus();
        QTextCursor tmpCursor = ui->textEdit->textCursor();
        tmpCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 0);
        ui->textEdit->setTextCursor(tmpCursor);
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

    if(extraItem->nodeType==BaseInfo::Child)
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

    if(item->parent()==NULL)
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

void MainWindow::right_item_pressed(QTreeWidgetItem *item, int column)
{
    if(QGuiApplication::mouseButtons()!= Qt::RightButton)
    {
        return;
    }
    logger->log(QString("right clicked"));
    rightMenu->exec(QCursor::pos());   //菜单弹出位置为鼠标点击位置
}

//切换左侧节点时，保存上一个节点的内容，加载当前节点的内容
void MainWindow::currentTreeItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    logger->log(QString("currentTreeItemChanged tirggerd"));
    ExtraQTreeWidgetItem* extraPreviousNode= dynamic_cast<ExtraQTreeWidgetItem*>(previous);

    //save previous node content to local file
    //when delete node,the focus will be changed,and this function will be trigger.
    //so when the previous node is deleted, it's deletetype is 1 , don't save the previous node content.
    //otherwise, delete node, the local file will be create again.
    if(previous!=NULL && extraPreviousNode->deleteType==0 && ((ExtraQTreeWidgetItem*)previous)->nodeType==BaseInfo::Child)
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
            out<<ui->textEdit->toHtml()<<Qt::endl;
        }
        myfile.close();
    }
    if(current==NULL)
    {
        return;
    }
    //若是笔记本group，则将文本清空
    if(((ExtraQTreeWidgetItem*)current)->nodeType==BaseInfo::Parent)
    {
        ui->textEdit->setHtml("");
        ui->titleLineEdit->setText(current->text(0));
        //not allow to edit the Nodegroup
        ui->textEdit->setEnabled(false);
        for(auto child_toolBtn :ui->titleBar->children())
        {
            QToolButton* btn=dynamic_cast<QToolButton*>(child_toolBtn);
            if(btn!=NULL)
            {
                btn->setEnabled(false);
            }
        }
        return;
    }
    else
    {
        ui->textEdit->setEnabled(true);
        for(auto child_toolBtn :ui->titleBar->children())
        {
            QToolButton* btn=dynamic_cast<QToolButton*>(child_toolBtn);
            if(btn!=NULL)
            {
                btn->setEnabled(true);
            }
        }
    }
    //load current node‘s title to right-titleLineEdit title
    ui->titleLineEdit->setText(current->text(0));
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
    ui->textEdit->setHtml(allStr);
}

//给回收站&收藏node赋值
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
    if(childCount>0||child->nodeType==BaseInfo::Parent)
    {
        if(child->parent()==NULL&&child->text(0)==NODENAME_COLLECT) //顶级系统节点-收藏
        {
            child->setIcon(0,QIcon(":/res/icons/collect.png"));
        }
        else if(child->parent()==NULL&&child->text(0)==NODENAME_RECYLE)//顶级系统节点-废纸篓
        {
            child->setIcon(0,QIcon(":/res/icons/recycle.png"));
        }
        else
        {
           // QIcon icon= util::CreateColorSvgIcon(":/res/icons/noteparent.svg",QString::fromStdString(color),"0.8");
            auto icon=QIcon(":/res/icons/parentnote.png");
            child->setIcon(0,icon);
        }
        for (int j = 0; j < childCount; ++j)
        {
            ExtraQTreeWidgetItem * grandson = dynamic_cast<ExtraQTreeWidgetItem*>(child->child(j));
            if(grandson->childCount()>0||grandson->nodeType==BaseInfo::Parent)
            {
                setItemIcon(grandson);
            }
            else
            {
                QIcon icon= util::CreateColorSvgIcon(":/res/icons/notechild.svg", QString::fromStdString(color));
                grandson->setIcon(0,QIcon(icon));
            }
        }
    }
    else
    {
        child->setIcon(0,QIcon(":/res/icons/childnote.png"));
    }
}

void MainWindow::onTitleLineEditEditingFinished()
{
    logger->log(QString("onTitleLineEditEditingFinished"));
    if (ui->titleLineEdit->text().length() == 0)
    {
        return;
    }
    auto currentItem=ui->treeWidget->currentItem();
    if(currentItem->parent()==NULL)
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

    currentItem->setText(0, ui->titleLineEdit->text());


    // update the local file change
    auto extraItem =dynamic_cast<ExtraQTreeWidgetItem *>(currentItem);
    BaseInfo::OperationType type = extraItem->isNewNode==1?
                (extraItem->nodeType == BaseInfo::Child ?
                     BaseInfo::AddNode:
                     BaseInfo::AddNodeGroup)
              :BaseInfo::RenameNode;
    if(type==BaseInfo::RenameNode)
    {
        if(((ExtraQTreeWidgetItem*)currentItem)->nodeType==BaseInfo::Child)
        {
            QFile file(fileFullPath);
            file.rename(fileNewPath+"/"+ui->titleLineEdit->text()+".html");
        }
        else
        {
            QString parentFullPath = QString("%1/storage").arg(QCoreApplication::applicationDirPath());

            if(currentItem->parent()!=NULL)
            {
                parentFullPath=util::treeItemToFullFilePath(currentItem->parent(),BaseInfo::Parent);
            }
            QString newDir=QString("%1/%2").arg(parentFullPath,ui->titleLineEdit->text());
            QString oldDir=QString("%1/%2").arg(parentFullPath,oldName);
            QDir _dir(oldDir);
            logger->log(oldDir.toStdString());
            logger->log(newDir.toStdString());
            if (_dir.exists())
            {
                _dir.rename(oldDir, newDir);
            }
        }
        config->updateXmlRenameNode(oldPath,currentItem);
        return;
    }

    config->updateXml(type, currentItem->parent(),currentItem);

    if (type == BaseInfo::AddNodeGroup)
    {
        //新增本地文件夹
        QString dirpath = util::treeItemToFullFilePath(extraItem, BaseInfo::Parent);
        QDir *dir = new QDir();
        if (!dir->exists(dirpath))
        {
            dir->mkpath(dirpath);
        }
        extraItem->isNewNode=0;//reset isNewNode status
    }
    else if (type == BaseInfo::AddNode)
    {
        //创建本地空文档html
        QString filePath = util::treeItemToFullFilePath(currentItem, BaseInfo::Child);
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
    if(this->newGroupForm!=NULL)
    {
        this->newGroupForm->close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

