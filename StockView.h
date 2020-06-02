#ifndef STOCKVIEW_H
#define STOCKVIEW_H

#include <QWidget>
#include "pohoda/pohoda.h"

namespace Ui {
class StockView;
}

class StockView : public QWidget
{
    Q_OBJECT

public:
    explicit StockView(QWidget *parent = nullptr);
    ~StockView();

    void setStockList(QList<SStock> list);

protected:
    void changeEvent(QEvent* event);

signals:
    void importData();

private slots:
    void on_pushButtonPohoda_clicked(bool);

private:
    Ui::StockView *ui;
};

#endif // STOCKVIEW_H
