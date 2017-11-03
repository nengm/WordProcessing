#include "WordProcessingForm.h"
#include <QApplication>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDesktopWidget *pDesk = QApplication::desktop();
    WordProcessingForm w;
    w.setFixedSize(15*50,15*34+1);
    w.move((pDesk->width() - w.width())/2,(pDesk->height() - w.height())/2);
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.setWindowTitle("Handwriting");
    w.show();
    return a.exec();
}
