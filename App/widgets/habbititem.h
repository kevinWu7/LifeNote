#ifndef HABBITITEM_H
#define HABBITITEM_H

#include <QWidget>
#include <QDate>

namespace Ui {
class HabbitItem;
}

class HabbitItem : public QWidget
{
    Q_OBJECT

public:
    explicit HabbitItem(QWidget *parent = nullptr);
    ~HabbitItem();

    void setIconIndex(int iconIndex);
    void setProjectName(QString name);
    void mousePressEvent(QMouseEvent *event) override;

public slots:
    void OnReceiveWeekBtnClicked(QDate date,bool ischecked);
signals:
    void triggerMousePressEvent(HabbitItem *habit,QWidget* weekWidget);
private:
    Ui::HabbitItem *ui;
    void InitWeekButtons();
    QString projectName;
    int iconIndex=0;

};

#endif // HABBITITEM_H
