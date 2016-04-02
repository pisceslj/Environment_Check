#include <QtGui/QApplication>
#include <qtextcodec.h>
#include <QTextCodec>
#include <QProcess>
#include "widget.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");//情况2
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}

