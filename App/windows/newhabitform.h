#ifndef NEWHABITFORM_H
#define NEWHABITFORM_H

#include <QWidget>
#include <QMouseEvent>
#include "baseinfo.h"

namespace Ui {
class NewHabitForm;
}

class NewHabitForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewHabitForm(QWidget *parent = nullptr);
    ~NewHabitForm();
    void InitRoundRadius();
    void setEditMode(QString name,int iconIndex,CheckinRule* rule);

signals:
    void sendSelectDataToParent(QString name ,int iconIndex,int formMode,CheckinRule* rule);

public slots:
    void okBtn_clicked();
    void cancleBtn_clicked();
    void iconBtn_clicked();

private:
    Ui::NewHabitForm *ui;
    void initIConBtn();
    void initRuleWidget();
    void periodComboCurIndexChanged();
    int iconIndex=0;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


    //记录鼠标，窗口位置
    QPointF windowPos;
    QPointF mousePos;
    QPointF dPos;

    bool formMode=0;//0代表是新建habit，1代表是修改habit，默认为0
};

#endif // NEWHABITFORM_H
