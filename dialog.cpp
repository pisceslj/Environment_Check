#include "dialog.h"
#include "ui_dialog.h"
#include "mytablemodel.h"
#include <QtSql>
#include <QMessageBox>
#include <QSqlError>

QSqlTableModel *model;
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("SqlData Check"));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_lookButton_clicked()
{
    QString select_all_sql = "select * from envi_data1"; //查询所有数据
    QSqlQuery sql_query;
    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {

        QSqlTableModel *model = new QSqlTableModel(ui->tableView);
        model->setTable("envi_data1");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Temp"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Hum"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Light"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Conct"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("DateTime"));
        model->select();
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);//要执行提交修改才能写入数据库

        ui->tableView->setModel(model);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不能对表格编辑
        ui->tableView->setSortingEnabled(true);     //可排序
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);     //选中行
        ui->tableView->resizeColumnsToContents();   //根据列内容调整宽度
        ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    }
}

void Dialog::on_deleteButton_clicked()
{
        //得到id 并删除数据库数据
        int row = ui->tableView->currentIndex().row();
        model->removeRow(row);//删除改行
        int ok = QMessageBox::warning(this,tr("Delete this row!"),tr("Are your sure delete this row?"),QMessageBox::Yes,QMessageBox::No);
        if(ok == QMessageBox::No)
        {
            model->revertAll();//如果不删除，则撤销
        }
        else{
            model->submitAll();//否则提交，在数据库删除该行
        }

        QModelIndex index = ui->tableView->currentIndex();
        int id=index.sibling(row,0).data().toInt();//取第一列内容

        qDebug("\nhangshu is %d\n",row);
        qDebug("\nlujie is %d\n",id);
        //删除数据库中数据

        /*QSqlQuery query;
            query.prepare("delete from envi_data1 where id =:id ");
            query.bindValue(":id",id);
            query.exec();
            if(!query.isActive())
            {
                QMessageBox::critical(this,QString::fromLocal8Bit("Warning!"),QString::fromLocal8Bit("Delete failed!"));
                return;
            }*/
}

void Dialog::on_checkButton_clicked()
{
    /*QSqlDatabase db;
    QSqlQuery query(db);//
    query.exec("select * from envi_data1");
    query.seek(0);
    int j=0;
    while (query.next())
    {
         j++;
    }
    qDebug()<< "j::"<<j;
    qDebug()<< query.lastError();
    //query.exec("select *from envi_data1 where time between  format('"ui->lineEdit->text()"', 'yyyy-mm-dd hh:mm:ss') and format('"ui->lineEdit_2->text()"', 'yyyy-mm-dd hh:mm:ss')");
        qDebug()<< query.lastError();
        query.seek(0);
        int i=0;
        while (query.next())
        {
            i++;
        }
        qDebug()<< "i::"<<i;
        qDebug()<< query.lastError();

        model->setTable("envi_data1");
        model->select();
        model->submitAll();
        ui->tableView->setModel(model);
        ui->tableView->show();*/
}

void Dialog::on_quitButton_clicked()
{
    this->close();
}
