#include "WordDrawQWidget.h"
#include <QPainter>
WordDrawQWidget::WordDrawQWidget(QList<QPoint > *list_Point,int x,QWidget *parent) :
    QWidget(parent)
{
    //设置点
    this->list_Point = list_Point;

    //设置横坐标
    this->x = x;
}
WordDrawQWidget::~WordDrawQWidget()
{

}

void WordDrawQWidget::paintEvent(QPaintEvent *event)
{
    if(list_Point != NULL){
        QPainter painter_rect(this);
        painter_rect.setRenderHint(QPainter::Antialiasing, true);
        painter_rect.setPen(QPen(QColor(255,193,37),1,Qt::SolidLine));//设置画笔形式
        painter_rect.setBrush(QBrush(QColor(255,193,37),Qt::SolidPattern));//设置画刷形式
        for ( int i=0; i!=list_Point->size(); ++i )
        {
            QPoint p = list_Point->at(i);
            painter_rect.drawRect(p.x()*cell_Length,p.y()*cell_Length,cell_Length,cell_Length);
        }
    }

    QPainter painter_Black(this);
    painter_Black.setPen(QPen(Qt::white,1,Qt::SolidLine));//设置画笔形式
    painter_Black.setBrush(QBrush(Qt::white,Qt::SolidPattern));//设置画刷形式
    int needDraw = 32;

    if(x > 32){
        needDraw = x;
    }

    //绘制网格
    for(int i=0;i<33;i++)
    {
        painter_Black.drawLine(0,i*cell_Length,cell_Length*needDraw,i*cell_Length);
    }
    for(int i=0;i<needDraw+1;i++){
        painter_Black.drawLine(i*cell_Length,0,i*cell_Length,32*cell_Length);
    }
}
void WordDrawQWidget::setPoints_MaxX(QList<QPoint> *list_Point,int x)
{
    this->list_Point = list_Point;
    this->x = x;
    this->update();
}

void WordDrawQWidget::mousePressEvent(QMouseEvent *event)
{
    int nowx = event->pos().x();
    int nowy = event->pos().y();

    //屏蔽落在y轴24格以后的坐标
    if(nowy >= x*cell_Length || nowx>= x*cell_Length)
    {
        return;
    }
    int index_x = nowx/cell_Length;
    int index_y = nowy/cell_Length;

    //标志，flag为true证明点击的坐标不在QPointList中，需要加入
    bool flag = true;
    //STL循环风格
    QMutableListIterator<QPoint> iter(*list_Point);
    for(iter.toBack();iter.hasPrevious();)
    {
        QPoint p = iter.previous();
        if((p.x() == index_x)&&(p.y() == index_y))
        {
            flag = false;
            iter.remove();
            break;
        }
        else{
            iter.peekNext();
        }
    }
    if(flag){
        list_Point->append(QPoint(index_x,index_y));
    }
    this->update();
}
