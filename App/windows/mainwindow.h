#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QMessageBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMenu>
#include <QTextDocumentFragment>
#include <QFileDialog>
#include <QImageReader>
#include <QTextBlockFormat>
#include "extraqtreewidgetitem.h"
#include "texteditcontainer.h"
#include "newnotegroupform.h"
#include "checkinwidget.h"





QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


//重写键盘函数
public:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    QAction * newNoteAction;
    QAction * newNoteGroupAction;
    QAction * deleteNoteAction;
    QAction * recoverNoteAction;
    QAction * collectNoteAction;
    QAction * moveNoteAction;
    QAction * lockAction;
    QTreeWidgetItem *recycleNode; //回收站顶级节点，构造函数给其赋值
    QTreeWidgetItem *collectNode; //收藏顶级节点，构造函数给其赋值

    QMenu *rightMenu;
    void setAllItemIcon();
    void setItemIcon(ExtraQTreeWidgetItem* child_item);
    void initRightMenu();

    void initTopLevelNode();

    void setLineVerticalInterval();

    TextEditContainer * textEditContainer=nullptr;
    checkinWidget *_checkinWidget=nullptr;
    NewNoteGroupForm * newGroupForm=nullptr;
    QTextBlockFormat *blockFormat=nullptr;//to set qtextedit vertical interval 3

public slots:
    void onSaveBtn_clicked();
    void currentTreeItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void right_item_pressed();//右键菜单点击
    void onAddnewBtn_clicked();
    void onNewNoteItemClick();
    void onNewNoteGroupItemClick();
    void onCollectNoteItemClick();
    void onMoveNoteItemClick();
    void onLockItemClick();
    void onDeleteNoteItemClick();
    void onRecoverNoteItemClick();
    void onMenuToShow();
    void logCheckStateChanged(int state);
    void onTitleLineEditEditingFinished();
    //接收新建笔记本子窗体的消息
    void onReceiveNewGroupFormData(QString nodeName,int color_index);
    void onApplicationQuit();
    void checkinBtn_clicked();

};
#endif // MAINWINDOW_H
