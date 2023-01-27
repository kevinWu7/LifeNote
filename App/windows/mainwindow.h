#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include<QSpinBox>
#include<QColorDialog>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QStandardItemModel>
#include<QStandardItem>
#include<iostream>
#include<QMenu>
#include<QTextDocumentFragment>
#include<QFileDialog>
#include<QImageReader>
#include "nodeconfig.h"
#include "extraqtreewidgetitem.h"
#include "newnotegroupform.h"




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
    nodeconfig *config ;
    QTreeWidgetItem *recycleNode; //回收站顶级节点，构造函数给其赋值

    QMenu *rightMenu;
    void setAllItemIcon();
    void setItemIcon(ExtraQTreeWidgetItem* child_item);
    void initRightMenu();
    void InsertImageDialog();
    void initRecycleNode();

    NewNoteGroupForm * newGroupForm;

public slots:
    void boldBtn_clicked();
    void italicBtn_clicked();
    void underlineBtn_clicked();
    void colorBtn_clicked();
    void onPictureBtn_clicked();
    void onUndoBtn_clicked();
    void onSaveBtn_clicked();
    void onAddnewBtn_clicked();
    void right_item_pressed(QTreeWidgetItem *item, int column);//右键菜单点击
    void currentTreeItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void onNewNoteItemClick();
    void onNewNoteGroupItemClick();
    void onCollectNoteItemClick();
    void onMoveNoteItemClick();
    void onLockItemClick();
    void onDeleteNoteItemClick();
    void onRecoverNoteItemClick();
    void onMenuToShow();

    void onTitleLineEditEditingFinished();
    //接收新建笔记本子窗体的消息
    void onReceiveNewGroupFormData(QString nodeName);
    void onApplicationQuit();

};
#endif // MAINWINDOW_H
