#ifndef SUPPLIERVIEW_H
#define SUPPLIERVIEW_H

#include <QWidget>
#include "pohoda/pohoda.h"

namespace Ui {
class SupplierView;
}

class SupplierView : public QWidget
{
    Q_OBJECT

public:
    explicit SupplierView(QWidget *parent = nullptr);
    ~SupplierView();

    void setSupplierList(const QList<SSupplier>& list);

protected:
    void changeEvent(QEvent* event);

signals:
    void importData();

private slots:
    void on_pushButtonPohoda_clicked(bool);

private:
    Ui::SupplierView *ui;
};

#endif // SUPPLIERVIEW_H
