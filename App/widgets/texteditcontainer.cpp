#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QImageReader>
#include "logger.h"
#include "util.h"
#include "texteditcontainer.h"
#include "ui_texteditcontainer.h"
#include "roundedtooltiphelper.h"
#include "theme.h"


TextEditContainer::TextEditContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditContainer)
{
    ui->setupUi(this);

    initfontCombobox();

    //设置标题栏按钮
    themeChangedUpdateUiStatus();

    ui->boldBtn->setToolTip("加粗");
    RoundedToolTipHelper::installHelper(ui->boldBtn);
    ui->italicBtn->setToolTip("斜体");
    RoundedToolTipHelper::installHelper(ui->italicBtn);
    ui->colorBtn->setToolTip("颜色");
    RoundedToolTipHelper::installHelper(ui->colorBtn);
    ui->underlineBtn->setToolTip("下划线");
    RoundedToolTipHelper::installHelper(ui->underlineBtn);
    ui->pictureBtn->setToolTip("添加图片");
    RoundedToolTipHelper::installHelper(ui->pictureBtn);
    ui->saveBtn->setToolTip("保存");
    RoundedToolTipHelper::installHelper(ui->saveBtn);
    ui->undoBtn->setToolTip("撤回");
    RoundedToolTipHelper::installHelper(ui->undoBtn);
    ui->fontAddBtn->setToolTip("增大字号");
    RoundedToolTipHelper::installHelper(ui->fontAddBtn);
    ui->fontReduceBtn->setToolTip("减小字号");
    RoundedToolTipHelper::installHelper(ui->fontReduceBtn);

    connect(ui->boldBtn,SIGNAL(clicked()),this,SLOT(boldBtn_clicked()));
    connect(ui->italicBtn,SIGNAL(clicked()),this,SLOT(italicBtn_clicked()));
    connect(ui->underlineBtn,SIGNAL(clicked()),this,SLOT(underlineBtn_clicked()));
    connect(ui->colorBtn,SIGNAL(clicked()),this,SLOT(colorBtn_clicked()));
    connect(ui->pictureBtn,&QToolButton::clicked,this,&TextEditContainer::onPictureBtn_clicked);
    connect(ui->undoBtn,&QToolButton::clicked,this,&TextEditContainer::onUndoBtn_clicked);


    connect(ui->fontAddBtn,&QToolButton::clicked,this,&TextEditContainer::onFontAddBtn_clicked);
    connect(ui->fontReduceBtn,&QToolButton::clicked,this,&TextEditContainer::onFontReduceBtn_clicked);
    connect(ui->fontComboBox, &QComboBox::currentIndexChanged,this, &TextEditContainer::comboBoxCurrentIndexChanged);

    connect(ui->textEdit,&QTextEdit::cursorPositionChanged,this,&TextEditContainer::textEditCursorPositionChanged);

    bindThemeChangetCallback=std::bind(&TextEditContainer::themeChangedUpdateUiStatus,this);
    ThemeManager::getInstance().registerThemeGlobalEvent(bindThemeChangetCallback);
}

TextEditContainer::~TextEditContainer()
{
    delete ui;
}


void TextEditContainer::boldBtn_clicked()
{
    QTextCharFormat fmt;
    if(ui->boldBtn->isChecked())
    {
        fmt.setFontWeight(QFont::Bold);
    }
    else
    {
        fmt.setFontWeight(QFont::Normal);
    }
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void TextEditContainer::italicBtn_clicked()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(ui->italicBtn->isChecked());
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void TextEditContainer::underlineBtn_clicked()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(ui->underlineBtn->isChecked());
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void TextEditContainer::colorBtn_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue,this);
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt=ui->textEdit->currentCharFormat();
        fmt.setForeground(color);
        ui->textEdit->mergeCurrentCharFormat(fmt);
    }
}

void TextEditContainer::onUndoBtn_clicked()
{
    ui->textEdit->undo();
}



void TextEditContainer::onPictureBtn_clicked()
{
    InsertImageDialog();
}

void TextEditContainer::InsertImageDialog()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                QDir::homePath(), tr("IMAGE (*.png *.jpg *jpeg *.bmp *.gif)"));

    QUrl Uri = QUrl::fromLocalFile(file);
    QImage image = QImageReader(file).read();

    if (!image.isNull())
    {
        QTextDocument *textDocument = ui->textEdit->document();

        // Calculate a reasonable display size based on the image's aspect ratio
        QSizeF imageSize(image.width(), image.height());
        QSizeF viewportSize(ui->textEdit->viewport()->size());
        QSizeF displaySize ;
        //大于显示窗口，则缩小。若小于显示窗口的尺寸，则保持不变
        if( imageSize.width()> viewportSize.width()*displayImgRatio||imageSize.height()>viewportSize.height()*displayImgRatio)
        {
            // Calculate the scaling factor for both width and height
            qreal widthScaleFactor = viewportSize.width()*displayImgRatio / imageSize.width();
            qreal heightScaleFactor = viewportSize.height()*displayImgRatio/ imageSize.height();

            // Use the minimum scaling factor to maintain aspect ratio
            qreal scaleFactor = qMin(widthScaleFactor, heightScaleFactor);

            // Calculate the display size
            displaySize = QSizeF(imageSize.width() * scaleFactor, imageSize.height() * scaleFactor);
        }
        else
        {
            displaySize= QSizeF(imageSize.width() , imageSize.height() );
        }
        // Add the image as a resource
        textDocument->addResource(QTextDocument::ImageResource, Uri, QVariant(image));

        // Insert the image with the calculated size
        QTextCursor cursor = ui->textEdit->textCursor();
        QTextImageFormat imageFormat;
        imageFormat.setWidth(displaySize.width());
        imageFormat.setHeight(displaySize.height());
        imageFormat.setName(Uri.toString());
        cursor.insertImage(imageFormat);
    } else {
        // 处理图像插入失败的情况
    }
}



void TextEditContainer::onFontAddBtn_clicked()
{
    size_t realIndex=ui->fontComboBox->currentIndex();
    ui->fontComboBox->setCurrentIndex(realIndex==util::fontVector.size()-1?realIndex:realIndex+1);
}

void TextEditContainer::onFontReduceBtn_clicked()
{
    size_t realIndex=ui->fontComboBox->currentIndex();
    ui->fontComboBox->setCurrentIndex(realIndex==0?0:realIndex-1);
}

void TextEditContainer::initfontCombobox()
{
    ui->fontComboBox->clear();

    for(const QString &afont : util::fontVector)
    {
        ui->fontComboBox->addItem(afont);
    }
    ui->fontComboBox->setCurrentIndex(3); //default font is 14
}

void TextEditContainer::themeChangedUpdateUiStatus()
{
    QString color= currentTheme["CONTROL_TEXT"];
    ui->boldBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/note/bold.svg",color));
    ui->italicBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/note/italic.svg",color));
    ui->colorBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/note/color.svg",color));
    ui->underlineBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/note/underline.svg",color));
    ui->pictureBtn->setIcon(QIcon(":/icons/res/note/img.png"));
    ui->saveBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/note/save.svg",color));
    ui->undoBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/note/undo.svg",color));
    ui->fontAddBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/note/fontAdd.svg",color));
    ui->fontReduceBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/note/fontReduce.svg",color));
}

//change comboBox's selectIndex when move cursor
void TextEditContainer::textEditCursorPositionChanged()
{
    QTextCursor cursor=ui->textEdit->textCursor();

    if(cursor.hasSelection())
    {
        //prevent font be same when user  select All text
        //because select texts will change the cursor,then change the combobox's selectedIndex.
        //then trigger the comboBoxCurrentIndexChanged function,and all the text'font will be same
        return;
    }
    QFont font=cursor.charFormat().font();
    QString font_size =QString::number(font.pointSize());
    auto index=std::find(util::fontVector.begin(),util::fontVector.end(),font_size);
    if(index!= util::fontVector.end())
    {
        int realIndex=std::distance(util::fontVector.begin(),index);
        ui->fontComboBox->setCurrentIndex(realIndex);
    }
    //reset the bold... btn's state
    ui->boldBtn->setChecked(font.bold());
    ui->italicBtn->setChecked(font.italic());
    ui->underlineBtn->setChecked(font.underline());

    logger->log("textEditCursorPositionChanged");

}

//update current cursor font-size when change comboBox's selectIndex
void TextEditContainer::comboBoxCurrentIndexChanged()
{
    auto current_font= ui->fontComboBox->currentText();
    logger->log("current font:" +current_font);
    QFont font = ui->textEdit->currentFont();
    font.setPointSize(current_font.toInt());
    ui->textEdit->setCurrentFont(font);
}
