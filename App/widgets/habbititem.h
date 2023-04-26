#ifndef HABBITITEM_H
#define HABBITITEM_H

#include <QWidget>

namespace Ui {
class HabbitItem;
}

class HabbitItem : public QWidget
{
    Q_OBJECT

public:
    explicit HabbitItem(QWidget *parent = nullptr);
    ~HabbitItem();
    int iconIndex=0;
    void setIconIndex(int iconIndex);
    void setProjectName(QString name);
     void mousePressEvent(QMouseEvent *event) override;
public slots:

signals:
    void triggerMousePressEvent(HabbitItem *habit,QWidget* weekWidget);
private:
    Ui::HabbitItem *ui;
};

#endif // HABBITITEM_H
