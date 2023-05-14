#ifndef HABITITEM_H
#define HABITITEM_H

#include <QWidget>
#include <QDate>
#include "checkinconfig.h"
namespace Ui {
class HabitItem;
}

class HabitItem : public QWidget
{
    Q_OBJECT

public:
    explicit HabitItem(QString project_name,QWidget *parent = nullptr);
    ~HabitItem();

    void setIconIndex(int iconIndex);
    void setProjectName(QString name);
    void setHabitSelected(bool isSelect);
    void mousePressEvent(QMouseEvent *event) override;
    void InitCheckinBtn(const std::vector<checkin_dateitem*> &checkinItems);
    bool isSelected=false;//是否被光标选中
    QString projectName;
    int iconIndex=0;
    void InitWeekButtons();
public slots:
    //void OnReceiveWeekBtnClicked(QDate date,bool ischecked);
signals:
    void triggerMousePressEvent(HabitItem *habit);
private:
    Ui::HabitItem *ui;
    std::function<void(checkin_dateitem *)> bindFunctionOfreceiveBtnChecked;
    void receiveBtnChecked(checkin_dateitem* dateItem);
    int checkedCount=0;//右侧统计x/7中指代x
    QString baseStyleSheet;




};

#endif // HABITITEM_H
