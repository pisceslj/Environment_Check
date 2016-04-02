#include <QMessageBox>
#include <qtextstream.h>
#include <QDebug>
#include <QDialog>
#include "widget.h"
#include "ui_widget.h"
#include "win_qextserialport.h"
#include "dialog.h"

float Temp;
int Hum,Conct,Light;

/**
 * @brief Widget::Widget构造函数
 * @param parent
 */
Widget::Widget(QWidget *parent): QWidget(parent),ui(new Ui::Widget)
{
    timer1 = new QTimer(this);//定时刷新
    timer2 = new QTimer(this);//统计运行时间
    time_status = 1;          //停止状态，1代表启动，0代表停止
    setFixedSize(952,700);    //固定窗口大小

    //获取四个绘图显示区的高和宽
    int view_h1 = ui->graphicsView->height();
    int view_w1 = ui->graphicsView->width();
    int view_h2 = ui->graphicsView_2->height();
    int view_w2 = ui->graphicsView_2->width();
    int view_h3 = ui->graphicsView_3->height();
    int view_w3 = ui->graphicsView_3->width();
    int view_h4 = ui->graphicsView_4->height();
    int view_w4 = ui->graphicsView_4->width();

    //实例化四个绘图区对象
    myscene1 = new WaveScene(view_w1-2,view_h1-4,this);
    myscene2 = new WaveScene(view_w2-2,view_h2-4,this);
    myscene3 = new WaveScene(view_w3-2,view_h3-4,this);
    myscene4 = new WaveScene(view_w4-2,view_h4-4,this);

    //在显示区里设置绘图区域
    ui->graphicsView->setScene(myscene1);
    ui->graphicsView_2->setScene(myscene2);
    ui->graphicsView_3->setScene(myscene3);
    ui->graphicsView_4->setScene(myscene4);

    //为四个绘图区安装事件
    ui->widget->installEventFilter(this);
    ui->widget_2->installEventFilter(this);
    ui->widget_3->installEventFilter(this);
    ui->widget_4->installEventFilter(this);

    //设置绘图和清除的槽函数（点击触发）
    connect(ui->drawButton, SIGNAL(clicked()), this, SLOT(drawClicked()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearClicked()));

    setWindowTitle(tr("Environment Check"));          //设置窗体名称
    ui->lcdNumber->setDigitCount(19);                 //显示格式yyyy-MM-dd hh:mm:ss
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat); //设置时间显示字体
    ui->lcdNumber->display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    //定义计数的时分秒
    countTemp=0;secondTemp=0;
    minuteTemp=0;hourTemp=0;
    ui->lcdNumberH->setDigitCount(2);
    ui->lcdNumberH->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumberM->setDigitCount(2);
    ui->lcdNumberM->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumberS->setDigitCount(2);
    ui->lcdNumberS->setSegmentStyle(QLCDNumber::Flat);

    //设置计数器与计时函数的事件关联
    connect(timer1,SIGNAL(timeout()),this,SLOT(clearline()));  //定时刷新
    connect(timer2,SIGNAL(timeout()),this,SLOT(timerUpdate()));//统计计时
    //设置整个UI
    ui->setupUi(this);
}

/**
 * @brief Widget::~Widget析构函数
 */
Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Widget::mouseDoubleClickEvent鼠标双击事件
 * @param click
 */
void Widget::mouseDoubleClickEvent(QMouseEvent *click)
{
    if(click->button()==Qt::LeftButton)          //鼠标左键点击
    {
        if(windowState()!=Qt::WindowFullScreen)  //当前屏幕不为全屏
            setWindowState(Qt::WindowFullScreen);//设置为全屏
        else
            setWindowState(Qt::WindowNoState);   //否则保持不变
    }
}

/**
 * @brief Widget::drawClicked描绘波形
 */
void Widget::drawClicked()
{
    myscene1->removeWave1();
    myscene1->removeScale();
    myscene1->createScale(20,12);
    myscene1->setScope(500, 500);
    myscene1->timer_1(0.1);

    myscene2->removeWave2();
    myscene2->removeScale();
    myscene2->createScale(20,12);
    myscene2->setScope(500, 500);
    myscene2->timer_2(0.1);

    myscene3->removeWave3();
    myscene3->removeScale();
    myscene3->createScale(20,12);
    myscene3->setScope(500, 500);
    myscene3->timer_3(0.1);

    myscene4->removeWave4();
    myscene4->removeScale();
    myscene4->createScale(20,12);
    myscene4->setScope(500, 500);
    myscene4->timer_4(0.1);

    ui->drawButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->clearButton->setEnabled(true);
}

/**
 * @brief Widget::clearClicked
 * 按钮点击时触发清除所绘制图线
 */
void Widget::clearClicked()
{
     myscene1->removeWave1();
     myscene2->removeWave2();
     myscene3->removeWave3();
     myscene4->removeWave4();

     ui->drawButton->setEnabled(true);
     ui->stopButton->setEnabled(true);
     ui->clearButton->setEnabled(false);

}

/**
 * @brief Widget::timeoutslot
 * 计时结束槽
 */
void Widget::timeoutslot()
{
     QMessageBox::information(this, tr("Warning"), tr("The return time_status is "));
}

/**
 * @brief Widget::on_openMyComBtn_clicked
 * 按键触发打开串口和链接数据库操作
 */
void Widget::on_openMyComBtn_clicked()
{
    timer1->start(2600); //刷新显示
    timer2->start(10);   //统计运行时间

    //定义一个结构体，用来存放串口各个参数
    struct PortSettings myComSetting = {BAUD4800,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};

    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    myCom = new Win_QextSerialPort("COM4",myComSetting,QextSerialBase::EventDriven);

    if(myCom->open(QIODevice::ReadWrite))   //打开串口
        {
            myCom->setBaudRate(BAUD4800);   //设置波特率
            myCom->setDataBits(DATA_8);     //设置数据位
            myCom->setStopBits(STOP_1);     //设置停止位
            myCom->setParity(PAR_NONE);     //设置校验位
            myCom->setFlowControl(FLOW_OFF);//设置流控制
            myCom->setTimeout(500);         //设置超时时间
        }
    //以可读写方式打开串口
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    //信号和槽函数关联，当串口缓冲区有数据时，进行读串口操作

    //连接数据库驱动类型
    qDebug() << QSqlDatabase::drivers();
    //链接函数实现数据库的链接功能
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); //添加QSQLite数据库驱动
    db.setHostName("127.0.0.1");                           //数据库主机地址
    db.setUserName("root");                                //登陆的用户是超级用户root
    db.setPassword("");                                    //登陆密码无
    db.setDatabaseName("environment");                     //打开的数据库表，表预先创建。

    //布尔类型，打开数据。
    if(!db.open()) //if判断
    {
       QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
       //否则输出数据库的消息框错误信息
     }
     else
     {
        qDebug()<<"open Database!"<<endl;
     }
    ui->openMyComBtn->setEnabled(false);  //打开串口后“打开串口”按钮不可用
    ui->closeMyComBtn->setEnabled(true);  //打开串口后“关闭串口”按钮可用
    ui->drawButton->setEnabled(true);     //打开串口后“开始绘图”按钮可用
    ui->stopButton->setEnabled(true);     //打开串口后“暂停绘图”按钮可用
}

/**
 * @brief Widget::on_closeMyComBtn_clicked
 * 按键触发关闭串口操作
 */
void Widget::on_closeMyComBtn_clicked()
{
    timer1->stop();                        //停止刷新显示
    timer2->stop();                        //停止统计运行时间

    if(time_status == 1)                   //time_status=1说明在正常运行
    {
        myCom->close();                    //关闭串口
        myscene1->qtimer_stop1();          //
        myscene2->qtimer_stop2();          //
        myscene3->qtimer_stop3();          //停止
        myscene4->qtimer_stop4();          //
        time_status = 0;                   //修改运行标志
    }

    ui->openMyComBtn->setEnabled(true);    //关闭串口后“打开串口”按钮可用
    ui->closeMyComBtn->setEnabled(false);  //关闭串口后“关闭串口”按钮不可用
    ui->drawButton->setEnabled(false);     //关闭串口后“开始绘图”按钮不可用
    ui->stopButton->setEnabled(false);     //关闭串口后“暂停绘图”按钮不可用
    ui->clearButton->setEnabled(false);    //关闭串口后“清除绘图”按钮不可用
}

/**
 * @brief Widget::on_stopButton_clicked
 * 暂停按钮事件
 */
void Widget::on_stopButton_clicked()
{
    if(time_status == 0)               //time_status说明处于停止状态
     {
         timer1->start(2600);          //停止刷新
         timer2->start(10);            //停止统计
         myscene1->qtimer_start1();
         myscene2->qtimer_start2();
         myscene3->qtimer_start3();
         myscene4->qtimer_start4();
         time_status = 1;              //修改运行标志
     }
     else if(time_status == 1)
     {
          timer1->stop();              //停止刷新
          timer2->stop();              //停止统计
          myscene1->qtimer_stop1();
          myscene2->qtimer_stop2();
          myscene3->qtimer_stop3();
          myscene4->qtimer_stop4();
          time_status = 0;
     }
    ui->stopButton->setEnabled(true);
    ui->drawButton->setEnabled(false);
    ui->clearButton->setEnabled(true);
}

/**
 * @brief Widget::on_checkSqlBtn_clicked
 * 显示数据
 */
void Widget::on_checkSqlBtn_clicked()
{
    Dialog dialog;
    dialog.show();
    dialog.exec();
}

/**
 * @brief Widget::on_quitBtn_clicked
 * 退出按钮事件
 */
void Widget::on_quitBtn_clicked()
{
    this->close();
}

/**
 * @brief Widget::readMyCom
 * 读串口函数
 */
void Widget::readMyCom()
{
    QByteArray data = myCom->readAll();//读取串口缓冲区的所有数据给临时变量data

    if(!data.isEmpty())
    {
       QByteArray t,h,c,l;
       QByteArray tem,hu,cont,lit;
       QString Time;
       t = data.mid(0,8).prepend(" ");
       h = data.mid(8,5).prepend(" ");
       l = data.mid(13,8).prepend(" ");
       c = data.mid(22,8).prepend(" ");
       QString temp = QString::fromLocal8Bit(t);
       QString light = QString::fromLocal8Bit(l);
       tem = data.mid(1,4);
       hu = data.mid(8,2);
       cont = data.mid(22,4);
       lit = data.mid(14,2);

       Temp = atof(tem);
       Hum = atoi(hu);
       Conct = atoi(cont);
       Light = atoi(lit);
       Time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
       /*QSqlQuery query;
       query.prepare("insert into envi_data1(temp,hum,light,conct,time)"
                                         "values(:temp,:hum,:light,:conct,:time)");
       query.bindValue(":temp",Temp);
       query.bindValue(":hum",Hum);
       query.bindValue(":light",Light);
       query.bindValue(":conct",Conct);
       query.bindValue(":time",Time);
       query.exec();*/

       ui->textBrowser->insertPlainText(temp);
       ui->textBrowser_2->insertPlainText(c);
       ui->textBrowser_3->insertPlainText(light);
       ui->textBrowser_4->insertPlainText(h);


 }
}

/**
 * @brief Widget::clearline
 * 清除显示
 */
void Widget::clearline()
{
    ui->textBrowser->clear();
    ui->textBrowser_2->clear();
    ui->textBrowser_3->clear();
    ui->textBrowser_4->clear();
}

/**
 * @brief Widget::eventFilter
 * @param watched
 * @param e
 * @return
 * 事件过滤函数
 */
bool Widget::eventFilter(QObject *watched, QEvent *e)
{  
    if(watched == ui->widget){
       if(e->type() == QEvent::Paint)
          {
            paintOnWidget(ui->widget,40,tr("Y/℃"));
            return true;
        }
    }else if(watched == ui->widget_2){
        if(e->type() == QEvent::Paint)
        {
            paintOnWidget(ui->widget_2,100,tr("Y/%RH"));
            return true;
        }
    }else if(watched == ui->widget_3){
        if(e->type() == QEvent::Paint)
        {
            paintOnWidget(ui->widget_3,10000,tr("Y/ppm"));
            return true;
        }
    }else{
        if(e->type() == QEvent::Paint)
        {
            paintOnWidget(ui->widget_4,100,tr("Y/%"));
            return true;
        }
    }
    return QWidget::eventFilter(watched, e);
}

/**
 * @brief Widget::paintOnWidget   绘制坐标系
 * @param w                       绘制对象
 * @param totalCount              Y轴的值域
 * @param munit                   Y轴所代表单位
 */
void Widget::paintOnWidget(QWidget *w,int totalCount,QString munit)
{
    QPainter painter(w);
    painter.setPen(Qt::yellow);
    QFontMetrics metrics = painter.fontMetrics();

    int textHeight = metrics.ascent() + metrics.descent(); //textHeight = 12
    int leftWidth = metrics.width(tr("9000")) +5;          //leftWidth  = 29
    int rightWidth = metrics.width(tr("(日)"));            //rightWidth = 24
    int width = w->size().width() - leftWidth - rightWidth;//w->size.width = 331  width = 278
    int height = w->size().height() - 4 * textHeight;      //w->size.height = 271 height = 223

    // 绘制外框
    painter.drawRect(0, 0, w->size().width() -1, w->size().height() - 1);
    //　移动坐标系
    painter.translate(leftWidth, 1.75 * textHeight + height);

    int count = 20;      //分成20成
    float deltaX = width / 12.0f;         // x坐标上每分的宽度 deltaX = 23.166667
    float deltaY = (float)height / count; // y坐标上每分的宽度 deltaY = 11.150000

    // 画横坐标
    painter.drawLine(0, 0, width, 0);
    painter.drawText((deltaX * 12),0,tr(" X/s"));
    for (int i = 0; i <= 12; ++i)
    {
        QString second = QString("%1").arg(i);
        int stringWidth = metrics.width(second);

        // 绘制坐标刻度
        if(i != 0)
        {
            painter.drawLine(deltaX * i, 0, deltaX * i, 4);
        }
        // 绘制坐标处的秒
        int secondX = deltaX * (i - 1) + ((deltaX - stringWidth) / 2);
        painter.drawText(secondX+20, textHeight+6, second);
    }

    // 画纵坐标
    painter.drawLine(0, 0, 0, -height);
    for (int i = 1; i <= count; ++i)
    {
        QString value = QString("%1").arg(i * totalCount / count);
        int stringWidth = metrics.width(value);

        painter.drawText(-metrics.width(munit),
                         -(deltaY * count + textHeight / 2 + metrics.descent()),munit);
        // 绘制坐标值
        painter.drawText(-stringWidth - 4, -(deltaY * i + textHeight / 2 - metrics.ascent()), value);
        // 绘制坐标刻度
        painter.drawLine(-4, -i * deltaY, 0, -i * deltaY);
    }
}

/**
 * @brief Widget::timerUpdate
 * 统计运行多少秒
 */
void Widget::timerUpdate()
{
    countTemp += 1;
    if(countTemp==100)
    {
        countTemp = 0;
        secondTemp += 1;
        if(secondTemp==60)
        {
            secondTemp=0;
            minuteTemp+=1;
            if(minuteTemp==60)
            {
                minuteTemp=0;
                hourTemp+=1;
                if(hourTemp==24)
                {
                    hourTemp=0;
                }
            }
        }
    }
    //把整数转换成字符串
    QString hour = QString::number(hourTemp);
    QString minute = QString::number(minuteTemp);
    QString second = QString::number(secondTemp);

    ui->lcdNumberH->display(hour);
    ui->lcdNumberM->display(minute);
    ui->lcdNumberS->display(second);
}

