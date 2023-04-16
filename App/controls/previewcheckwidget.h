#ifndef PREVIEWCHECKWIDGET_H
#define PREVIEWCHECKWIDGET_H

#include <QWidget>

namespace Ui {
class PreviewCheckWidget;
}

class PreviewCheckWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewCheckWidget(QWidget *parent = nullptr);
    ~PreviewCheckWidget();

private:
    Ui::PreviewCheckWidget *ui;
};

#endif // PREVIEWCHECKWIDGET_H
