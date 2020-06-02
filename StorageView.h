#ifndef STORAGEVIEW_H
#define STORAGEVIEW_H

#include <QWidget>
#include "pohoda/pohoda.h"

namespace Ui {
class StorageView;
}

class StorageView : public QWidget
{
    Q_OBJECT

public:
    explicit StorageView(QWidget *parent = nullptr);
    ~StorageView();

    void setStorageList(const QList<SStorage>& list);

protected:
    void changeEvent(QEvent* event);

signals:
    void importData();

private slots:
    void on_pushButtonPohoda_clicked(bool);

private:
    Ui::StorageView *ui;
};

#endif // STORAGEVIEW_H
