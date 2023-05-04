#ifndef HABBITITEM_H
#define HABBITITEM_H

#include <QWidget>
#include <QDate>
#include "checkinconfig.h"
namespace Ui {
class HabbitItem;
}

class HabbitItem : public QWidget
{
    Q_OBJECT

public:
    explicit HabbitItem(QString project_name,QWidget *parent = nullptr);
    ~HabbitItem();

    void setIconIndex(int iconIndex);
    void mousePressEvent(QMouseEvent *event) override;
    void InitCheckinBtn(const std::vector<checkin_dateitem*> &checkinItems);
    bool isSelected=false;//是否被光标选中
    QString projectName;
    int iconIndex=0;
    void InitWeekButtons();
public slots:
    //void OnReceiveWeekBtnClicked(QDate date,bool ischecked);
signals:
    void triggerMousePressEvent(HabbitItem *habit);
private:
    Ui::HabbitItem *ui;




};

#endif // HABBITITEM_H
