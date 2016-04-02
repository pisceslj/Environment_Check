#include "mytablemodel.h"
#include <QDate>

MyTableModel::MyTableModel(QObject *parent):QSqlTableModel(parent){}

QVariant MyTableModel::data(const QModelIndex &idx, int role) const
{
    if(!idx.isValid())
        return QVariant();

    QVariant vt = QSqlTableModel::data(idx,role);
    if(QVariant::DateTime == vt.type())
        return vt.toDate().toString("yyyy/MM/dd hh:mm:ss");

    return vt;
}

