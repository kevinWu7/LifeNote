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

private:
    Ui::HabbitItem *ui;
};

#endif // HABBITITEM_H
