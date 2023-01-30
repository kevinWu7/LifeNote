#ifndef NEWNOTEGROUPFORM_H
#define NEWNOTEGROUPFORM_H

#include <QWidget>
#include <QMouseEvent>
#include <map>

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

    //记录鼠标，窗口位置
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;

    void cancleBtn_clicked();
    void okBtn_clicked();
    void onColorToolBtn_clicked();

signals:
    void sendParentWindowData(QString);

private:
    Ui::NewNoteGroupForm *ui;
    std::map<int,std::string> colorBtnMap=
    {
      {0,"rgb(252,77,6)"},
      {1,"rgb(212,171,128)"},
      {2,"rgb(168,119,199)"},
      {3,"rgb(110,207,101)"},
      {4,"rgb(241,92,228)"},
      {5,"rgb(162,171,178)"},
      {6,"rgb(252,100,110)"},
      {7,"rgb(125,180,255)"},
      {8,"rgb(156,184,120)"},
      {9,"rgb(181,169,70)"},
      {10,"rgb(57,179,173)"},
      {11,"rgb(107,105,141)"},
    };

};

#endif // NEWNOTEGROUPFORM_H
