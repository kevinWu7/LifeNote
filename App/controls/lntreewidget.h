#ifndef LNTREEWIDGET_H
#define LNTREEWIDGET_H
#include<QTreeWidget>
#include<QEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>


class LNTreeWidget:public QTreeWidget
{
public:
     LNTreeWidget(QWidget *parent = nullptr);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void startDrag1(Qt::DropActions supportedActions);//暂时先不实现这个stargDrag
    void mousePressEvent(QMouseEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
};

#endif // LNTREEWIDGET_H
