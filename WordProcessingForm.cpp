#include "WordProcessingForm.h"
#include <QPainter>
#include <QWheelEvent>
#include <QList>
#include <QHBoxLayout>
#include <QtCore/qmath.h>

WordProcessingForm::WordProcessingForm(QWidget *parent) :
    QWidget(parent)
{
    list_Point = new QList<QPoint>();


    setMyUi();
}

WordProcessingForm::~WordProcessingForm()
{
    delete list_Point;
}
void WordProcessingForm::setMyUi(){
    cell_Length = 15;
    list_Point = new QList<QPoint>();
    //滚动区域
    scrollAreaView = new QScrollArea(this);
    //垂直滚动条不可见
    scrollAreaView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreaView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollAreaView->setFocusPolicy(Qt::NoFocus);
    scrollAreaView->setFrameStyle(QFrame::NoFrame);
    scrollAreaView->setFixedSize(15*32,15*34);
    scrollAreaView->move(0,0);

    //滚动绘画区域
    //默认绘制32*32区域
    wordDrawQWidget = new WordDrawQWidget(list_Point,32);
    wordDrawQWidget->setFixedSize(15*32,15*32 + 1);
    wordDrawQWidget->cell_Length = cell_Length;
    QPalette w_Background(wordDrawQWidget->palette());
    w_Background.setColor(QPalette::Background,Qt::black);
    wordDrawQWidget->setPalette(w_Background);
    //设置滚动区域的窗体
    scrollAreaView->setWidget(wordDrawQWidget);
    scrollAreaView->setWidgetResizable(false);

    QPalette pal_Background(this->palette());
    pal_Background.setColor(QPalette::Background,Qt::black);
    this->setPalette(pal_Background);
    drawColor = QColor(255,193,37);
    lineColor = Qt::white;
    QWidget *widget = new QWidget(this);
    widget->setFixedSize(15*18,15*34);
    widget->move(15*32,0);
    QHBoxLayout *hBoxLayoutS= new QHBoxLayout(widget);
    hBoxLayoutS->setSpacing(0);
    hBoxLayoutS->setMargin(0);
    gbox = new QGroupBox();
    gbox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    hBoxLayoutS->addWidget(gbox,1);



    //语言字体选择
    QFontDatabase database;
    language_Combo = new QComboBox(this);
    language_Combo->setFont(QFont("SimSun",9));
    foreach (const QFontDatabase::WritingSystem m, database.writingSystems()) {
        QString lan = database.writingSystemName(m);
        language_Combo->addItem(lan);
    }
    language_Combo->setCurrentIndex(QFontDatabase::SimplifiedChinese - 2);
    connect(language_Combo,SIGNAL(currentIndexChanged(QString)),this,SLOT(language_selected(QString)));
    font_Combo = new QComboBox(this);
    font_Combo->setFont(QFont("SimSun",9));

    foreach (const QString &family, database.families(QFontDatabase::SimplifiedChinese)) {
        font_Combo->addItem(family);
    }
    font_Combo->setCurrentIndex(16);

    connect(font_Combo,SIGNAL(currentIndexChanged(QString)),this,SLOT(font_selected(QString)));


    drawButton = new QPushButton();
    drawButton->setText("取模");
    drawButton->setFont(QFont("SimSun",9));
    connect(drawButton,SIGNAL(clicked(bool)),this,SLOT(getWordModulo(bool)));
    drawButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    transverseButton = new QPushButton();
    transverseButton->setText("横向取模");
    transverseButton->setFont(QFont("SimSun",9));
    connect(transverseButton,SIGNAL(clicked(bool)),this,SLOT(traBtnClick(bool)));
    transverseButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    verticalButton = new QPushButton();
    verticalButton->setText("竖向取模");
    verticalButton->setFont(QFont("SimSun",9));
    connect(verticalButton,SIGNAL(clicked(bool)),this,SLOT(verBtnClick(bool)));
    verticalButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    clearButton = new QPushButton();
    clearButton->setText("清空");
    clearButton->setFont(QFont("SimSun",9));
    connect(clearButton,SIGNAL(clicked(bool)),this,SLOT(clearClick(bool)));
    clearButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QGridLayout *gridLayout = new QGridLayout();

    textEdit_Input = new QTextEdit();
    connect(textEdit_Input,SIGNAL(textChanged()),this,SLOT(textChanged()));
    textEdit = new QTextEdit();


    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 1);
    gridLayout->setColumnStretch(3, 1);

    for(int i =0;i<12;i++){
        gridLayout->setRowStretch(i,1);
    }
    gridLayout->addWidget(textEdit_Input,0,0,1,4);

    gridLayout->addWidget(language_Combo,1,0,1,4);
    gridLayout->addWidget(font_Combo,2,0,1,4);
    gridLayout->addWidget(transverseButton,3,0,1,4);
    gridLayout->addWidget(verticalButton,4,0,1,4);
    gridLayout->addWidget(clearButton,5,0,1,4);
    gridLayout->addWidget(textEdit,6,0,6,4);
    gbox->setLayout(gridLayout);
    this->update();
}
void WordProcessingForm::language_selected(const QString &text)
{
    font_Combo->clear();
    QFontDatabase database;
    QFontDatabase::WritingSystem writeSys;
    foreach (const QFontDatabase::WritingSystem m, database.writingSystems()) {
        QString lan = database.writingSystemName(m);
        if(language_Combo->currentText() == lan)
        {
            writeSys = m;
            break;
        }
    }
    foreach (const QString &family, database.families(writeSys)) {
        font_Combo->addItem(family);
    }
}

void WordProcessingForm::font_selected(const QString &text)
{
    QString fontStr = font_Combo->currentText();
    QString now_text = textEdit_Input->toPlainText();

    int x = processing(now_text,fontStr,list_Point);
    if(x > 31)
    {
        if((x+1)%32!= 0)
        {
            x = ((x+1)/32 + 1)*32;
        }
        wordDrawQWidget->setFixedSize(x*cell_Length, 15*32 + 1);
    }
    else
        wordDrawQWidget->setFixedSize(32*cell_Length, 15*32 + 1);

    wordDrawQWidget->setPoints_MaxX(list_Point,x);

    this->update();
}

void WordProcessingForm::getWordModulo(bool)
{
    QString text = textEdit_Input->toPlainText();
    //WordProcessing.processing(text,"宋体",1,);
}

void WordProcessingForm::traBtnClick(bool)
{
    QString s =transverseData();
    textEdit->setText(s);
}
void WordProcessingForm::verBtnClick(bool)
{
    QString s =longitudinalData();
    textEdit->setText(s);
}
void WordProcessingForm::clearClick(bool)
{
    this->textEdit->setText("");
    this->textEdit_Input->setText("");
    list_Point->clear();
    this->update();
}

//横向从左向右取得8位，下一一行继续。
QString WordProcessingForm::transverseData()
{
    int x = 0;
    QList<QPoint>::iterator iter = list_Point->begin();
    for(;iter != list_Point->end();iter++)
    {
        QPoint m = (*iter);
        if(m.x() > 0){
            x = m.x();
        }
    }
    //计算
    QList<QString> stringList;
    for(int i = 0;i<32;i++)
    {
        //x轴,去除不必要的全0
        for(int j = 0;j<x/8 + 1;j++)
        {
            int x = j*8;
            int data = 0;
            QList<QPoint>::iterator iter = list_Point->begin();
            for(;iter != list_Point->end();iter++)
            {
                QPoint m = (*iter);
                if(((m.x() - x <8 )&&(m.x() -x >=0))&&(m.y() == i))
                {
                    int num = m.x() - x;
                    data += qPow(2,7-num);
                }

            }
            QString str = QString("0x%1").arg(data,2,16,QLatin1Char('0'));
            stringList.append(str);
        }
    }
    QString qstr="";
    QList<QString>::iterator strIter = stringList.begin();
    QList<QString>::iterator strIter_endP = stringList.end();
    for(;strIter != stringList.end();strIter++)
    {
        if(strIter != strIter_endP){
            qstr += *strIter + ",";
        }
        else
        {
            qstr += *strIter;
        }
    }
    if(qstr.count() != 0)
    {
        qstr = qstr.left(qstr.count() - 1);
    }
    return qstr;
}
//纵向从上向下取得8位。
QString WordProcessingForm::longitudinalData()
{
    int x = 0;
    QList<QPoint>::iterator iter = list_Point->begin();
    for(;iter != list_Point->end();iter++)
    {
        QPoint m = (*iter);
        if(m.x() > 0){
            x = m.x();
        }
    }
    //计算
    QList<QString> stringList;
    for(int i = 0;i<=x;i++)
    {

        for(int j = 0;j<4;j++)
        {
            int y = j*8;
            int data = 0;
            QList<QPoint>::iterator iter = list_Point->begin();
            for(;iter != list_Point->end();iter++)
            {
                QPoint m = (*iter);
                if(((m.y() - y <8 )&&(m.y() -y >=0))&&(m.x() == i))
                {
                    int num = m.y() - y;
                    data += qPow(2,7-num);
                }

            }
            QString str = QString("0x%1").arg(data,2,16,QLatin1Char('0'));
            stringList.append(str);
        }
    }
    QString qstr="";
    QList<QString>::iterator strIter = stringList.begin();
    QList<QString>::iterator strIter_endP = stringList.end();
    for(;strIter != stringList.end();strIter++)
    {
        if(strIter != strIter_endP){
            qstr += *strIter + ",";
        }
        else
        {
            qstr += *strIter;
        }
    }
    if(qstr.count() != 0)
    {
        qstr = qstr.left(qstr.count() - 1);
    }
    return qstr;
}
void WordProcessingForm::textChanged()
{
    list_Point->clear();
    QString fontStr = font_Combo->currentText();
    QString text = textEdit_Input->toPlainText();
    int x = processing(text,fontStr,list_Point);
    if(x > 31)
    {
        if((x+1)%32!= 0)
        {
            x = ((x+1)/32 + 1)*32;
        }
        wordDrawQWidget->setFixedSize(x*cell_Length, 15*32 + 1);
    }
    else
    {
        x = 32;
        wordDrawQWidget->setFixedSize(32*cell_Length, 15*32 + 1);
    }
    wordDrawQWidget->setPoints_MaxX(list_Point,x);

    this->update();
}


