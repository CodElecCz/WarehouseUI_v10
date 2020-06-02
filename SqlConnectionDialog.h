#ifndef QSQLCONNECTIONDIALOG_H
#define QSQLCONNECTIONDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class QSqlConnectionDialog;
}

class QSqlConnectionDialog: public QDialog
{
    Q_OBJECT

public:
    QSqlConnectionDialog(QWidget *parent = nullptr);
    ~QSqlConnectionDialog();

    QString driverName() const;
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    int port() const;
    bool useInMemoryDatabase() const;

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_dbCheckBox_clicked();

private:
    Ui::QSqlConnectionDialog *ui;
};

#endif
