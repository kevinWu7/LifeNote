#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QImageReader>
#include "logger.h"
#include "util.h"
#include "texteditcontainer.h"
#include "ui_texteditcontainer.h"
#include "baseinfo.h"



TextEditContainer::TextEditContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditContainer)
{
    ui->setupUi(this);
    ui->titleBar->setStyleSheet("QToolButton{border:none;} "
                                "QToolButton:checked{background-color:rgb(218, 218, 218)}"
                                "QToolButton:hover{background-color:rgb(218, 218, 218)}"
                                "QWidget#titleBar{background-color:#FFFFFF}");
    //set titleLineEdit stylesheet
    ui->titleLineEdit->setStyleSheet("border: 0px;");

    ui->editWidget->setStyleSheet("QWidget#editWidget"
                                  "{background-color:#FFFFFF;"
                                  "border-radius:7px}");
    ui->titleBarBottomLine->setStyleSheet(QString("QFrame{border-top: 1px solid %1; border-bottom: none;}").arg(LINE_COLOR));

    ui->editWidget->layout()->setSpacing(0);
    //设置mainPage内部控件间距为5
    //ui->mainPage->layout()->setSpacing(5);

    //设置editWidget内部左侧边距
    ui->editWidget->setContentsMargins(3,3,3,3);
    initfontCombobox();

    //设置标题栏按钮
    ui->boldBtn->setIcon(QIcon(":/icons/res/note/bold.png"));
    ui->boldBtn->setToolTip("加粗");
    ui->italicBtn->setIcon(QIcon(":/icons/res/note/italic.png"));
    ui->italicBtn->setToolTip("斜体");
    ui->colorBtn->setIcon(QIcon(":/icons/res/note/color.png"));
    ui->colorBtn->setToolTip("颜色");
    ui->underlineBtn->setIcon(QIcon(":/icons/res/note/underline.png"));
    ui->underlineBtn->setToolTip("下划线");
    ui->pictureBtn->setIcon(QIcon(":/icons/res/note/img.png"));
    ui->pictureBtn->setToolTip("添加图片");
    ui->saveBtn->setIcon(QIcon(":/icons/res/note/save.png"));
    ui->saveBtn->setToolTip("保存");
    ui->undoBtn->setIcon(QIcon(":/icons/res/note/undo.png"));
    ui->undoBtn->setToolTip("撤回");
    ui->fontAddBtn->setIcon(QIcon(":/icons/res/note/fontAdd.png"));
    ui->fontAddBtn->setToolTip("增大字号");
    ui->fontReduceBtn->setIcon(QIcon(":/icons/res/note/fontReduce.png"));
    ui->fontReduceBtn->setToolTip("减小字号");

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
    QString selectFilter="IMAGE (*.png *.jpg *jpeg *.bmp *.gif)\n";
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                "/", tr("IMAGE (*.png *.jpg *jpeg *.bmp *.gif)\n"),&selectFilter);

    QUrl Uri ( QString ( "file://%1" ).arg ( file ) );
    QImage image = QImageReader ( file ).read();

    QTextDocument * textDocument = ui->textEdit->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);
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

    logger->log(std::string("textEditCursorPositionChanged"));

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
