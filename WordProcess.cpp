#include "WordProcess.h"
#include <QPainter>

int processing(QString text, QString fontStr, QList<QPoint> *list_Point)
{
    int max_x=0;
    //清空
    list_Point->clear();

    if(text == "" || fontStr == "" ){
        return 0;
    }

    //设置默认500个字,取字模会自动过滤空白
    int textCount = 500;

    //位图
    QImage image(32 * textCount,32,QImage::Format_Mono);
    QPainter painter(&image);

    //设置白色
    QRgb backColor = qRgb(255,255,255);
    image.fill(backColor);
    QFont font;
    font.setFamily(fontStr);

    //设置文字大小为32像素(32像素对应纵向32)
    font.setPixelSize(32);

    //其他一些字体设置
    font.setWeight(1);
    font.setItalic(false);
    font.setUnderline(false);

    painter.setFont(font);

    //X轴左对齐，Y轴居中
    int flats = Qt::AlignLeft|Qt::AlignVCenter;

    //在位图上画图
    painter.drawText(image.rect(),flats,text);

    //竖向默认32像素
    for(int i=0;i<32 ;i++){
        //横向长度默认32像素，每次取得8位，顾设置每个字32像素需要4次循环
        for(int j=0;j<4*textCount;j++)
        {
            //偏移
            unsigned char t_byte = 0x80;
            //取图片1个字节
            unsigned char t_currentByte  = *(image.bits() + i*4*textCount +j);
            //设置跳出循环标记,比较完一个字节跳出
            int count = 0;
            while(count < 8){
                //每次都复位
                t_byte = 0x80;
                //设置偏移
                t_byte >>= count % 8;
                //比较当前位
                if(!(t_currentByte & t_byte)){
                    //0为黑色，算出坐标
                    int x = j*8 + count;
                    int y = i;
                    //保存黑色点，其余为白色
                    QPoint m(x,y);
                    list_Point->append(m);
                    if(x>max_x)
                    {
                        max_x = x;
                    }
                }
                count++;
            }
        }
    }
    return max_x;
}
