#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H
#include <QSqlTableModel>

class MyTableModel : public QSqlTableModel
{
//Q_OBJECT
public:
    explicit MyTableModel(QObject *parent = 0);
    QVariant data(const QModelIndex &idx,int role = Qt::DisplayRole)const;
};

#endif // MYTABLEMODEL_H
