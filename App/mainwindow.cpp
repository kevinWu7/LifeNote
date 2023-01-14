#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //this->setWindowIcon(QIcon(":/res/icons/logo.ico"));
   // setWindowIcon(QIcon(":/res/icons/logo.ico"));
    //设置左侧侧边栏样式
    ui->leftBar->setStyleSheet(QString("QAbstractButton{min-height:%1px;max-height:%1px;margin:0px;border:none;}").arg(17));
    ui->titleBar->setStyleSheet(QString("QAbstractButton{min-height:%1px;max-height:%1px;margin:0px;}").arg(25));

    //set titleLineEdit stylesheet
    ui->titleLineEdit->setStyleSheet("border: 0px;");
    ui->textEdit->setStyleSheet("border:0px;");
    ui->editWidget->layout()->setSpacing(0);

    ui->treeWidget->setColumnCount(1);

    ui->treeWidget->setHeaderLabels(QStringList()<<"first");
    //ui->treeWidget->setColumnWidth(0, 40);  //设置列宽
    ui->treeWidget->header()->setStretchLastSection(false);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //隐藏标题栏
    ui->treeWidget->header()->setVisible(false);

    //设置背景色为透明
    ui->treeWidget->setStyleSheet("background-color:transparent;");
    setStyleSheet("QTreeWidget::item{""height:25px;""}");
    //设置不同层次菜单的缩进
    ui->treeWidget->setIndentation(15);
    //设置边框不可见
    ui->treeWidget->setFrameStyle(QFrame::NoFrame);

    //通过配置文件，创建node
    config->loadConfigXML(ui->treeWidget);

    setAllItemIcon();

    initRecycleNode();
    //设置左侧按钮icon
    ui->searchBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->searchBtn->setIcon(QIcon(":/res/icons/search.png"));

    //设置标题栏按钮
    ui->boldBtn->setIcon(QIcon(":/res/icons/bold.png"));
    ui->italicBtn->setIcon(QIcon(":/res/icons/italic.png"));
    ui->colorBtn->setIcon(QIcon(":/res/icons/color.png"));
    ui->underlineBtn->setIcon(QIcon(":/res/icons/underline.png"));

    initRightMenu();

    //设置信号槽
    connect(ui->boldBtn,SIGNAL(clicked()),this,SLOT(boldBtn_clicked()));
    connect(ui->italicBtn,SIGNAL(clicked()),this,SLOT(italicBtn_clicked()));
    connect(ui->underlineBtn,SIGNAL(clicked()),this,SLOT(underlineBtn_clicked()));
    connect(ui->colorBtn,SIGNAL(clicked()),this,SLOT(colorBtn_clicked()));
    connect(ui->pictureBtn,&QToolButton::clicked,this,&MainWindow::onPictureBtn_clicked);
    connect(ui->undoBtn,&QToolButton::clicked,this,&MainWindow::onUndoBtn_clicked);
    connect(ui->saveBtn,&QToolButton::clicked,this,&MainWindow::onSaveBtn_clicked);
    connect(ui->treeWidget,&QTreeWidget::currentItemChanged,this,&MainWindow::currentTreeItemChanged);
    connect(ui->treeWidget,&QTreeWidget::itemPressed,this,&MainWindow::right_item_pressed);
    connect(rightMenu,&QMenu::aboutToShow,this,&MainWindow::onMenuToShow);
    connect(newNoteAction, SIGNAL(triggered(bool)), this , SLOT(onNewNoteItemClick()));
    connect(newNoteGroupAction, SIGNAL(triggered(bool)), this , SLOT(onNewNoteGroupItemClick()));
    connect(saveNoteAction, SIGNAL(triggered(bool)), this , SLOT(onSaveNoteItemClick()));
    connect(moveNoteAction, SIGNAL(triggered(bool)), this , SLOT(onMoveNoteItemClick()));
    connect(lockAction, SIGNAL(triggered(bool)), this , SLOT(onLockItemClick()));
    connect(deleteNoteAction, SIGNAL(triggered(bool)), this , SLOT(onDeleteNoteItemClick()));
    connect(recoverNoteAction, SIGNAL(triggered(bool)), this , SLOT(onRecoverNoteItemClick()));
    connect(ui->titleLineEdit,&QLineEdit::editingFinished,this,&MainWindow::onTitleLineEditEditingFinished);
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
    if(color.isValid()){
        QTextCharFormat fmt;
        fmt=ui->textEdit->currentCharFormat();
        fmt.setForeground(color);
        ui->textEdit->mergeCurrentCharFormat(fmt);
    }
}

void MainWindow::onUndoBtn_clicked()
{

}

void MainWindow::onSaveBtn_clicked()
{
    if(ui->treeWidget->selectedItems().length()==0)
    {
        return;
    }
    if(ui->treeWidget->currentItem()->childCount()>0)
    {
        return;
    }
    QString fullPath=util::treeItemToFullFilePath(ui->treeWidget->currentItem());
    //解析出路径（不含文件名）和文件名
    int first = fullPath.lastIndexOf ("/");
    QString fileName = fullPath.right(fullPath.length ()-first-1); //xxxx.html
    QString dirPath = fullPath.left (first); //文件夹路径

    //如果路径不存在，则创建
    QDir* dir = new QDir();
    if(!dir->exists(dirPath)){
        dir->mkpath(dirPath);
    }

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
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                ".", tr("Bitmap Files (*.bmp)\n"
                                                        "JPEG (*.jpg *jpeg)\n"
                                                        "GIF (*.gif)\n"
                                                        "PNG (*.png)\n"));
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
#pragma endregion }

#pragma region Menu-Function{
//right click Menu, new NoteGroup
void MainWindow::onNewNoteGroupItemClick()
{
    if(ui->treeWidget->currentItem()==NULL)
    {
        return;
    }
    QTreeWidgetItem *newItem=new ExtraQTreeWidgetItem(BaseInfo::Parent);
    ((ExtraQTreeWidgetItem*)newItem)->isNewNode=1;
    QString newNodeGroupName=util::NoRepeatNodeName(ui->treeWidget->currentItem(),"新建笔记本");
    newItem->setText(0,newNodeGroupName);
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

//right click Menu ,new note
void MainWindow::onNewNoteItemClick()
{
    if(ui->treeWidget->currentItem()==NULL)
    {
        return;
    }
    QTreeWidgetItem *newItem=new ExtraQTreeWidgetItem(BaseInfo::Child);
    ((ExtraQTreeWidgetItem*)newItem)->isNewNode=1;
    QString newNodeName=util::NoRepeatNodeName(ui->treeWidget->currentItem(),"无标题");
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
    auto fullPath= util::treeItemToFullFilePath(currentNode); //如d:/sotrage/xxx.html
    bool isRecycle=currentNode->parent()->text(0)==RECYLE; //is recycle Node
    //if is recycleNode's child node ,delete directly
    if(isRecycle)
    {
        QFile file(fullPath);
        file.remove();
    }
    else
    {
        //若是非回收站的数据
        if(currentNode->nodeType==BaseInfo::Parent&&currentNode->childCount()>0)
        {
            QMessageBox::warning(this, tr("警告"),tr("\n无法批量删除,请选中单个笔记进行删除!"),QMessageBox::Ok);
            return;
        }
        //移动本地存储文件到回收站
        QString fileName = util::treeItemToFileName(currentNode); //文件名称，如xxx.html
        auto recyclePath=QString("%1/storage/%2/%3").arg(currentPath,RECYLE,fileName);
        bool moveResult= QFile::rename(fullPath,recyclePath); //A路径移动到B路径
        std::cout<<"delete node and move file "<<(moveResult ? "true": "false")  <<std::endl;
    }
    //delete doc(updateXml) must be ahead of the QTreeWidget'Node delete
    //because updateXml function is depend on the Node struct
    if(isRecycle)
    {
        config->updateXml(BaseInfo::DeleteNode,currentNode);
    }
    else
    {
        config->updateXml(BaseInfo::MoveNode,currentNode,recycleNode);
    }
    ExtraQTreeWidgetItem* extraCurrentNode= dynamic_cast<ExtraQTreeWidgetItem*>(currentNode);
    extraCurrentNode->deleteType=1;
    currentNode->parent()->removeChild(extraCurrentNode);//this line will trigger currentTreeItemChanged immediately
    extraCurrentNode->deleteType=0;
    if(!isRecycle) //if parentNode is recycle,not need to add
    {
       recycleNode->addChild(extraCurrentNode);
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
    bool moveResult= QFile::rename(recyclePath,""); //A路径移动到B路径
    std::cout<<"recycle Recover:  "<<( moveResult ? "true": "false")  <<std::endl;*/
}

void MainWindow::onSaveNoteItemClick()
{
    std::cout<<"save menu"<<std::endl;
}

void MainWindow::onMoveNoteItemClick()
{
    std::cout<<"move menu"<<std::endl;
}

void MainWindow::onLockItemClick()
{
    std::cout<<"lock menu"<<std::endl;
}

#pragma endregion}

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
    saveNoteAction = new QAction("收藏笔记", ui->treeWidget);
    moveNoteAction = new QAction("移动笔记", ui->treeWidget);
    lockAction = new QAction("添加密码锁", ui->treeWidget);
    deleteNoteAction = new QAction("删除笔记", ui->treeWidget);
    recoverNoteAction=new QAction("恢复笔记",ui->treeWidget);
    rightMenu=new QMenu(ui->treeWidget);
    rightMenu->addAction(newNoteAction);
    rightMenu->addAction(newNoteGroupAction);
    rightMenu->addAction(saveNoteAction);
    rightMenu->addAction(moveNoteAction);
    rightMenu->addAction(lockAction);
    rightMenu->addAction(deleteNoteAction);
    rightMenu->addAction(recoverNoteAction);
}

void MainWindow::onMenuToShow()
{
    auto item=ui->treeWidget->currentItem();
    auto extraItem=dynamic_cast<ExtraQTreeWidgetItem*>(item);
    if(extraItem->nodeType==BaseInfo::Child)
    {
        newNoteAction->setVisible(false);
        newNoteGroupAction->setVisible(false);
        saveNoteAction->setVisible(true);
    }
    else
    {
        newNoteAction->setVisible(true);
        newNoteGroupAction->setVisible(true);
        saveNoteAction->setVisible(false);
    }
    if(item->parent()==NULL)
    {
        deleteNoteAction->setVisible(false);
    }
    else
    {
        deleteNoteAction->setVisible(true);
        bool isRecycle=item->parent()->text(0)==RECYLE; //is recycle Node
        if(isRecycle)
        {
             recoverNoteAction->setVisible(true);
        }
        else
        {
             recoverNoteAction->setVisible(false);
        }
    }
}

void MainWindow::right_item_pressed(QTreeWidgetItem *item, int column)
{
    if(QGuiApplication::mouseButtons()!= Qt::RightButton)
    {
        return;
    }
    std::cout<<"right clicked"<<std::endl;
    rightMenu->exec(QCursor::pos());   //菜单弹出位置为鼠标点击位置
}

//切换左侧节点时，保存上一个节点的内容，加载当前节点的内容
void MainWindow::currentTreeItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    std::cout<<"currentTreeItemChanged tirggerd"<<std::endl;
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
         return;
    }
    else
    {
         ui->textEdit->setEnabled(true);
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

//给回收站node赋值
void MainWindow::initRecycleNode()
{
    int size = ui->treeWidget->topLevelItemCount();
    QTreeWidgetItem *child;
    for (int i = 0; i < size; i++)
    {
        child = ui->treeWidget->topLevelItem(i);
        if(child->text(0)==RECYLE)
        {
            recycleNode=child;
            break;
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
    if(childCount>0||child->nodeType==BaseInfo::Parent)
    {
         child->setIcon(0,QIcon(":/res/icons/parentnote.png"));
         for (int j = 0; j < childCount; ++j)
         {
             ExtraQTreeWidgetItem * grandson = dynamic_cast<ExtraQTreeWidgetItem*>(child->child(j));
             if(grandson->childCount()>0||grandson->nodeType==BaseInfo::Parent)
             {
                 grandson->setIcon(0,QIcon(":/res/icons/parentnote.png"));
                 setItemIcon(grandson);
             }
             else
             {
                 grandson->setIcon(0,QIcon(":/res/icons/childnote.png"));
             }
         }
    }
    else
    {
         child->setIcon(0,QIcon(":/res/icons/childnote.png"));
    }
}

void MainWindow::onTitleLineEditEditingFinished() {
  qDebug("onTitleLineEditEditingFinished");
  if (ui->titleLineEdit->text().length() == 0)
  {
     return;
  }

  auto oldPath=util::treeItemToNodePath(ui->treeWidget->currentItem());
  auto fileFullPath=util::treeItemToFullFilePath(ui->treeWidget->currentItem());
  auto fileNewPath=util::treeItemToNodeDirPath(ui->treeWidget->currentItem());
  auto oldName=ui->treeWidget->currentItem()->text(0);

  ui->treeWidget->currentItem()->setText(0, ui->titleLineEdit->text());


  // update the local file change
  auto extraItem =dynamic_cast<ExtraQTreeWidgetItem *>(ui->treeWidget->currentItem());
  BaseInfo::OperationType type = extraItem->isNewNode==1?
              (extraItem->nodeType == BaseInfo::Child ?
                   BaseInfo::AddNode:
                   BaseInfo::AddNodeGroup)
              :BaseInfo::RenameNode;
  if(type==BaseInfo::RenameNode)
  {
      if(((ExtraQTreeWidgetItem*)ui->treeWidget->currentItem())->nodeType==BaseInfo::Child)
      {
          QFile file(fileFullPath);
          file.rename(fileNewPath+"/"+ui->titleLineEdit->text()+".html");
      }
      else
      {
          QString parentFullPath=util::treeItemToFullFilePath(ui->treeWidget->currentItem()->parent(),BaseInfo::Parent);
          QString newDir=QString("%1/%2").arg(parentFullPath,ui->titleLineEdit->text());
          QString oldDir=QString("%1/%2").arg(parentFullPath,oldName);
          QDir _dir(oldDir);
          qDebug()<<oldDir;
          qDebug()<<newDir;
          if (_dir.exists())
          {
             _dir.rename(oldDir, newDir);
          }
      }
      config->updateXmlRenameNode(oldPath,ui->treeWidget->currentItem());
      return;
  }

  config->updateXml(type, ui->treeWidget->currentItem()->parent(),ui->treeWidget->currentItem());

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
    QString filePath = util::treeItemToFullFilePath(ui->treeWidget->currentItem(), BaseInfo::Child);
    QFile myfile(filePath);
    //注意WriteOnly是往文本中写入的时候用，ReadOnly是在读文本中内容的时候用，Truncate表示将原来文件中的内容清空
    if (myfile.open(QFile::WriteOnly))
    {
      myfile.close();
    }
    extraItem->isNewNode=0;//reset isNewNode status
  }
}

MainWindow::~MainWindow()
{
    delete ui;
}

