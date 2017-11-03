#ifndef WordProcessingForm_H
#define WordProcessingForm_H

#include <QWidget>
#include<QPushButton>
#include<QFrame>
#include<QGroupBox>
#include<QTextEdit>
#include <QFontDatabase>
#include <QComboBox>
#include <QScrollArea>
#include "WordDrawQWidget.h"
class WordProcessingForm : public QWidget
{
    Q_OBJECT

public:
    explicit WordProcessingForm(QWidget *parent = 0);
    ~WordProcessingForm();

public:
    int cell_Length;
    QColor backColor;
    QColor drawColor;
    QColor lineColor;
    QGroupBox *gbox;

    QComboBox *language_Combo;
    QComboBox *font_Combo;

    QScrollArea *scrollAreaView;
    WordDrawQWidget *wordDrawQWidget;
    //绘制
    QPushButton *drawButton;
    //横向
    QPushButton *transverseButton;

    //纵向
    QPushButton *verticalButton;

    QPushButton *clearButton;

    //输入
    QTextEdit *textEdit_Input;
    QTextEdit *textEdit;

    //需要绘制的点
    QList<QPoint> *list_Point;

    void setMyUi();

    //横向取模
    QString transverseData();

    //纵向取模
    QString longitudinalData();
public slots:
    void font_selected(const QString &text);
    void language_selected(const QString &text);

    //按钮事件
    void getWordModulo(bool);
    void traBtnClick(bool);
    void verBtnClick(bool);
    void clearClick(bool);
    void textChanged();
};

#endif // WordProcessingForm_H
