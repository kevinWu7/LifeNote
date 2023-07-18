#ifndef NEWNOTEGROUPFORM_H
#define NEWNOTEGROUPFORM_H

#include <map>
#include <QWidget>
#include <QMouseEvent>


namespace Ui {
class NewNoteGroupForm;
}

class NewNoteGroupForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewNoteGroupForm(QWidget *parent = nullptr);
    ~NewNoteGroupForm();

    void InitEvent();
    void InitColorPushBtn();
    //设置圆角
    void InitRoundRadius();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    //记录鼠标，窗口位置
    QPointF windowPos;
    QPointF mousePos;
    QPointF dPos;
    bool isDrag=false;

    void cancleBtn_clicked();
    void okBtn_clicked();
    void onColorToolBtn_clicked();



signals:
    void sendParentWindowData(QString,int);

private:
    Ui::NewNoteGroupForm *ui;
    int color_index=0;
};

#endif // NEWNOTEGROUPFORM_H
