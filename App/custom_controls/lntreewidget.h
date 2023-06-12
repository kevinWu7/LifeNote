#ifndef LNTREEWIDGET_H
#define LNTREEWIDGET_H
#include <QTreeWidget>
#include <QEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QTimer>
#include "extraqtreewidgetitem.h"
#include "thememanager.h"
#include <QStyledItemDelegate>
#include <QPainter>
#include "util.h"
#include "theme.h"

class CustomDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        // 绘制默认项
        QStyledItemDelegate::paint(painter, option, index);

        // 获取要绘制横线的项的数据
         QVariant data = index.data(Qt::UserRole); // 假设项的数据中包含一个标志用于指示是否绘制横线

         if (data.isValid() && data.toInt()>0)
         {
             // 绘制横线
             QRect rect = option.rect;
             QPen pen(util::generateRGBAColor(currentTheme["CONTROL_TEXT"], 1));  // 创建一个带有指定颜色的 pen
             pen.setWidth(2);  // 设置宽度为 2 像素
             painter->setPen(pen);  // 应用 pen
             if(data.toInt()==1)
             {
                 painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());
             }
             else if(data.toInt()==2)
             {
                 painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
             }
         }

    }
};

class LNTreeWidget:public QTreeWidget
{
public:
     LNTreeWidget(QWidget *parent = nullptr);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;
  //  bool eventFilter(QObject* obj, QEvent* event) override;
    ExtraQTreeWidgetItem* targetItem=nullptr;
    QRect draggingLineRect;
    void traverseTreeWidgetItems(QTreeWidgetItem *item);
    QTimer *timer=nullptr;
    void themeChanged();
    themeChangedCallback bFuncThemeChangeCallback;
    CustomDelegate delegate;
    ~LNTreeWidget();
};

#endif // LNTREEWIDGET_H
