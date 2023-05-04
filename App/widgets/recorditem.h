#ifndef RECORDITEM_H
#define RECORDITEM_H

#include <QWidget>

namespace Ui {
class RecordItem;
}

class RecordItem : public QWidget
{
    Q_OBJECT

public:
    explicit RecordItem(QWidget *parent = nullptr);
    ~RecordItem();

private:
    Ui::RecordItem *ui;
};

#endif // RECORDITEM_H
