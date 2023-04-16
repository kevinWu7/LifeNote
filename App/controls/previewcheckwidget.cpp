#include "previewcheckwidget.h"
#include "ui_previewcheckwidget.h"

PreviewCheckWidget::PreviewCheckWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewCheckWidget)
{
    ui->setupUi(this);
}

PreviewCheckWidget::~PreviewCheckWidget()
{
    delete ui;
}
