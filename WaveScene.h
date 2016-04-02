#ifndef WaveScene_H
#define WaveScene_H
#include <QtGui/qpainter.h>
#include <QGraphicsScene>
#include <QList>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QByteArray>
#include "win_qextserialport.h"

class WaveScene : public QGraphicsScene{
        Q_OBJECT

private:
        QPen m_scaleLinePen;
        QPen m_waveDataPen;
        QList<QGraphicsItem*> m_scaleLinesList;
        QList<QGraphicsItem*> m_waveList1,m_waveList2,m_waveList3,m_waveList4;
        QSizeF m_sceneSize;//绘制时，scene的大小
        qreal m_scopeX;//横向范围
        qreal m_scopeY;//纵向范围
        qreal m_scaleX;//横向的每个单位的像素数（比例）
        qreal m_scaleY;//纵向比例
        qreal m_step;//波形数据之间的间隔
        int m_numScaleX;//横向的刻度数
        int m_numScaleY;//纵向的刻度数
private:
        void setDefaultScaleLinePen();
        void setDefaultWaveDataPen();
        void setDefaultScope();
        void paintEvent( QPaintEvent * );

public:
        int count;
        int TempID;
        QPointF p11,p21;
        QPointF p12,p22;
        QPointF p13,p23;
        QPointF p14,p24;
        QTimer *timer21,*timer22,*timer23,*timer24;
        QByteArray temp_status;
        Win_QextSerialPort *myCom;  //声明对象
        WaveScene(QObject *parent = 0);
        WaveScene(qreal width, qreal height, QObject * parent = 0);

        void setScaleLinePen(QPen pen);
        void setWaveDataPen(QPen pen);
        void setScope(qreal scopeX, qreal scopeY);
        void createScale(int num_x,int num_y);
        void removeScale();

        void timer_1(qreal step);
        void timer_2(qreal step);
        void timer_3(qreal step);
        void timer_4(qreal step);
        void removeWave1();
        void removeWave2();
        void removeWave3();
        void removeWave4();
        void qtimer_stop1();
        void qtimer_stop2();
        void qtimer_stop3();
        void qtimer_stop4();
        void qtimer_start1();
        void qtimer_start2();
        void qtimer_start3();
        void qtimer_start4();
public slots:
        void drawWave1();
        void drawWave2();
        void drawWave3();
        void drawWave4();
};


#endif // WaveScene_H
