#ifndef WORDDRAWQWIDGET_H
#define WORDDRAWQWIDGET_H
#include <QScrollArea>
#include <QWheelEvent>
#include "WordProcess.h"
class WordDrawQWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordDrawQWidget(QList<QPoint > *list_Point,int x,QWidget *parent = 0);
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) ;
    ~WordDrawQWidget();
public:
    //初始化
    void init();

    //单元格长宽
    int cell_Length;

    //需要绘制的点
    QList<QPoint> *list_Point;
    //横坐标
    int x;

    //设置需要绘制的点和最大横坐标
    void setPoints_MaxX(QList<QPoint> *list_Point,int x);

};

#endif // WORDDRAWQWIDGET_H
