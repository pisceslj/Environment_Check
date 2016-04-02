#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QPainter>
#include "WaveScene.h"
#include <QMouseEvent>
#include <QtSql>
#include <QKeyEvent>
#include <QTextEdit>
#include <QTimer>//计时
#include <QDateTime>
#include <QString>
#include <QtCore>
#include <QSqlQueryModel>
#include <QTime>//显示时间
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QtSql/QsqlDatabase>
#include <QtSql/QsqlQuery>
#include <QPainterPath>
#include <QPainter>
#include "dialog.h"

namespace Ui
{
    class Widget;
}
class Widget : public QWidget
{
    Q_OBJECT
public slots:


public:
    Widget(QWidget *parent = 0);
    ~Widget();
    Win_QextSerialPort *myCom;

    WaveScene *myscene1;
    WaveScene *myscene2;
    WaveScene *myscene3;
    WaveScene *myscene4;

protected:
    void paintOnWidget(QWidget *w,int totalCount,QString munit);
    void mouseDoubleClickEvent(QMouseEvent *);
    virtual bool eventFilter(QObject *watched, QEvent *e);
private:
    Ui::Widget *ui;
    int time_status;
    QTimer *timer1;         //定时刷新
    QTimer *timer2;         //计时
    int hourTemp;           //Hour
    int minuteTemp;         //Minute
    int secondTemp;         //Second
    int countTemp;

private slots:
    void drawClicked();
    void clearClicked();
    void timeoutslot();
    void on_closeMyComBtn_clicked();
    void on_openMyComBtn_clicked();
    void on_quitBtn_clicked();
    void on_stopButton_clicked();
    void readMyCom();    //声明读串口槽函数
    void on_checkSqlBtn_clicked();
    void timerUpdate();
    void clearline();
};

#endif // WIDGET_H
