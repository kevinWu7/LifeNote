#ifndef TEXTEDITCONTAINER_H
#define TEXTEDITCONTAINER_H

#include <QWidget>
#include "thememanager.h"

namespace Ui {
class TextEditContainer;
}

class TextEditContainer : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditContainer(QWidget *parent = nullptr);
    ~TextEditContainer();
public slots:
    void boldBtn_clicked();
    void italicBtn_clicked();
    void underlineBtn_clicked();
    void colorBtn_clicked();
    void onPictureBtn_clicked();
    void onUndoBtn_clicked();


    void onFontAddBtn_clicked();
    void onFontReduceBtn_clicked();

    void comboBoxCurrentIndexChanged();
    void textEditCursorPositionChanged();
public:
    Ui::TextEditContainer *ui;
private:
    void InsertImageDialog();
    void initfontCombobox();
    void themeChangedUpdateUiStatus();
    themeChangedCallback bindThemeChangetCallback;
};

#endif // TEXTEDITCONTAINER_H
