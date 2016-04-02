#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_lookButton_clicked();
    //void QTableView::resizeColumnsToContents ();
    void on_deleteButton_clicked();
    void on_checkButton_clicked();
    void on_quitButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
