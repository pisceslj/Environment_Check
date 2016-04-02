#include "WaveScene.h"
#include <QGraphicsItem>
#include <stdlib.h>
#include <qstring.h>
#include <string.h>
#include <QTextCodec>
#include <QMutexLocker>
#include <QtGui/qpainter.h>
#include <QDebug>
#include "widget.h"

extern float Temp;
extern int Hum,Conct,Light;

/*
 *********构造函数************
 */
WaveScene::WaveScene(QObject *parent):QGraphicsScene(parent)
{
        this->setBackgroundBrush(Qt::black);
        setDefaultScaleLinePen();
        setDefaultWaveDataPen();
        setDefaultScope();
}

/*
 *********构造函数重载**********
 */
WaveScene::WaveScene(qreal width, qreal height, QObject * parent):QGraphicsScene(0, 0, width, height, parent)
{
        this->setBackgroundBrush(Qt::black);
        setDefaultScaleLinePen();
        setDefaultWaveDataPen();
        setDefaultScope();
}

/*
 ***********设置默认的刻度线画笔********
 */
void WaveScene::setDefaultScaleLinePen()
{
        m_scaleLinePen.setColor(Qt::green);
        m_scaleLinePen.setStyle(Qt::DashLine);
        m_scaleLinePen.setWidthF(1);
}

/*
 ***********设置默认波形画笔***********
 */
void WaveScene::setDefaultWaveDataPen()
{
        m_waveDataPen.setColor(Qt::red);
        m_waveDataPen.setStyle(Qt::SolidLine);
        m_waveDataPen.setWidthF(1);
}

/* 设置默认显示范围和比例*/
void WaveScene::setDefaultScope()
{
        QRectF rectf = this->sceneRect();
        setScope(rectf.width(), rectf.height());
}

/* 设置刻度线画笔 */
void WaveScene::setScaleLinePen(QPen pen)
{
        m_scaleLinePen = pen;
}
/* 设置波形画笔 */
void WaveScene::setWaveDataPen(QPen pen)
{
        m_waveDataPen = pen;
}

/* 设置显示范围 */
void WaveScene::setScope(qreal scopeX, qreal scopeY)
{
        QRectF rectf = this->sceneRect();
        m_scopeX = scopeX;//横向范围
        m_scopeY = scopeY;

        m_scaleX = rectf.width() / m_scopeX;
        m_scaleY = rectf.height() / m_scopeY;
//        qDebug("m_scopeX = %f\n",m_scopeX);
//        qDebug("m_scopeY = %f\n",m_scopeY);

}

/*
 ***********初始化起点及启动绘图*******
 */
void WaveScene::timer_1(qreal step)
{
     QRectF rectf = this->sceneRect();
     m_step = step; //波形数据之间的间隔
     count = 0;

     p11.setX(rectf.x() * m_scaleX);
     p11.setY(rectf.height() - Temp * m_scaleY*12);
     timer21=new QTimer(this);
     connect(timer21,SIGNAL(timeout()),this,SLOT(drawWave1()));//timeoutslot()为自定义槽
     timer21->start(1);
}

void WaveScene::timer_2(qreal step)
{
     QRectF rectf = this->sceneRect();
     m_step = step; //波形数据之间的间隔
     count = 0;
     p12.setX(rectf.x() * m_scaleX);
     p12.setY(rectf.height() - Hum * m_scaleY*5);
     timer22=new QTimer(this);
     connect(timer22,SIGNAL(timeout()),this,SLOT(drawWave2()));//timeoutslot()为自定义槽
     timer22->start(1);
}

void WaveScene::timer_3(qreal step)
{
     QRectF rectf = this->sceneRect();
     m_step = step; //波形数据之间的间隔
     count = 0;
     p13.setX(rectf.x() * m_scaleX);
     p13.setY(rectf.height() - (Conct/20) * m_scaleY);
     timer23=new QTimer(this);
     connect(timer23,SIGNAL(timeout()),this,SLOT(drawWave3()));//timeoutslot()为自定义槽
     timer23->start(1);
}

void WaveScene::timer_4(qreal step)
{
     QRectF rectf = this->sceneRect();
     m_step = step; //波形数据之间的间隔
     count = 0;
     p14.setX((rectf.x() * m_scaleX));//rectf.x()=0
     p14.setY(rectf.height() - Light * m_scaleY*5);
     timer24=new QTimer(this);
     connect(timer24,SIGNAL(timeout()),this,SLOT(drawWave4()));//timeoutslot()为自定义槽*/
     timer24->start(1);
}

/*
 ********暂停/继续************
 */
void WaveScene::qtimer_stop1()
{
      timer21->stop();
}
void WaveScene::qtimer_stop2()
{
      timer22->stop();
}
void WaveScene::qtimer_stop3()
{
      timer23->stop();
}
void WaveScene::qtimer_stop4()
{
      timer24->stop();
}
void WaveScene::qtimer_start1()
{
    timer21->start(1);
}
void WaveScene::qtimer_start2()
{
    timer22->start(1);
}
void WaveScene::qtimer_start3()
{
    timer23->start(1);
}
void WaveScene::qtimer_start4()
{
    timer24->start(1);
}
/*
 *绘制波形
 */
void WaveScene::drawWave1()
{
        QRectF rectf = this->sceneRect();
        m_sceneSize = rectf.size();
//        float r1 = rectf.x();
//        float r2 = rectf.height();
//        float r3 = rectf.width();
        //qDebug("\n %f \n",Temp);
//        qDebug("\nrectf.x() = %f\n",r1);
//        qDebug("\nrectf.height() = %f\n",r2);
//        qDebug("\nrectf.width() = %f\n",r3);
//        qDebug("\nm_scanleX = %f\n",m_scaleX);//0.544554
//        qDebug("\n%f\n",r1*m_scaleX);//0
//        qDebug("\n%f\n",r2*m_scaleX);//120.891089
//        qDebug("\n%f\n",r3*m_scaleX);//149.752475
        //qDebug("\nm_scanleY = %f\n",m_scaleY);        //m_scanleY = 0.439604
        p21.setX((count * m_step) * m_scaleX);      //m_scanleX = 0.5
        p21.setY(rectf.height() - Temp * m_scaleY*12);//rectf.height = 222

        m_waveList1.push_back((QGraphicsItem *) this->addLine(QLineF(p11, p21),m_waveDataPen));
        p11 = p21;

        if((count * m_step) * m_scaleX > rectf.width())
        {
            removeWave1();
            p11.setX(rectf.x() * m_scaleX);
            count=0;
        }
        else
        {
            count++;
        }
}

void WaveScene::drawWave2()
{
        QRectF rectf = this->sceneRect();
        m_sceneSize = rectf.size();

       //qDebug("\n%d\n",Hum);
        p22.setX(((count * m_step) * m_scaleX));
        p22.setY(rectf.height() - Hum * m_scaleY*5);
        m_waveList2.push_back((QGraphicsItem *) this->addLine(QLineF(p12, p22),m_waveDataPen));
        p12 = p22;

        if((count * m_step) * m_scaleX > rectf.width())
        {
            removeWave2();
            p12.setX((rectf.x() * m_scaleX));
            count=0;
        }
        else
        {
            count++;
        }
}

void WaveScene::drawWave3()
{
        QRectF rectf = this->sceneRect();
        m_sceneSize = rectf.size();

        //qDebug("\n%d\n",Conct);
        p23.setX(((count * m_step) * m_scaleX));
        p23.setY(rectf.height() - (Conct/20) * m_scaleY);
        m_waveList3.push_back((QGraphicsItem *) this->addLine(QLineF(p13, p23),m_waveDataPen));
        p13 = p23;

        if((count * m_step) * m_scaleX > rectf.width())
        {
            removeWave3();
            p13.setX((rectf.x() * m_scaleX));
            count=0;
        }
        else
        {
            count++;
        }
}
void WaveScene::drawWave4()
{
        QRectF rectf = this->sceneRect();
        m_sceneSize = rectf.size();

        //qDebug("\n%d\n",Light);
        p24.setX(((count * m_step) * m_scaleX));
        p24.setY(rectf.height() - Light * m_scaleY*5);
        m_waveList4.push_back((QGraphicsItem *) this->addLine(QLineF(p14, p24),m_waveDataPen));
        p14 = p24;

        if((count * m_step) * m_scaleX > (rectf.width()))
        {
            removeWave4();
            p14.setX((rectf.x() * m_scaleX));
            count=0;
        }
        else
        {
            count++;
        }
}

/*
 *********画刻度线，并指定横向和纵向的栅格数***********
 */
void WaveScene::createScale(int num_x, int num_y)
{
        //横向的刻度数
        m_numScaleX = num_x;
        m_numScaleY = num_y;

        QRectF rectf = this->sceneRect();

        QPointF p1;
        QPointF p2;

        //绘制横向刻度线
        for (int i = 1; i < num_x; i++)
        {
                p1.setX(rectf.x());
                p1.setY((rectf.height() / num_x * i));
                p2.setX(rectf.width());
                p2.setY((rectf.height() / num_x * i));
                m_scaleLinesList.push_back((QGraphicsItem*) this->addLine(
                                QLineF(p1, p2), m_scaleLinePen));
        }
        //绘制纵向刻度线
        for (int i = 1; i < num_y; i++)
        {
                p1.setX((rectf.width() / num_y * i));
                p1.setY(rectf.y());
                p2.setX((rectf.width() / num_y * i));
                p2.setY(rectf.height());
                m_scaleLinesList.push_back((QGraphicsItem*) this->addLine(
                                QLineF(p1, p2), m_scaleLinePen));
        }
}

/* 移除全部的刻度线 */
void WaveScene::removeScale()
{
        if (m_scaleLinesList.isEmpty())
                return;
        foreach(QGraphicsItem *item, m_scaleLinesList)
                {
                        this->removeItem(item);
                }
        m_scaleLinesList.clear();
}

/* 移除全部的波形 */
void WaveScene::removeWave1()
{
        if (m_waveList1.isEmpty())
                return;
        foreach(QGraphicsItem *item1, m_waveList1)
                {
                  this->removeItem(item1);
                }
        m_waveList1.clear();
}

void WaveScene::removeWave2()
{
       if (m_waveList2.isEmpty())
                return;
        foreach(QGraphicsItem *item2, m_waveList2)
                {
                  this->removeItem(item2);
                }
        m_waveList2.clear();
}

void WaveScene::removeWave3()
{
        if (m_waveList3.isEmpty())
                return;
        foreach(QGraphicsItem *item3, m_waveList3)
                {
                  this->removeItem(item3);
                }
        m_waveList3.clear();
}
void WaveScene::removeWave4()
{
        if (m_waveList4.isEmpty())
                return;
        foreach(QGraphicsItem *item4, m_waveList4)
                {
                  this->removeItem(item4);
                }
        m_waveList4.clear();
}




