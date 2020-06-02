#ifndef RECEIPEFORM_H
#define RECEIPEFORM_H

#include <QWidget>
#include "dbs/database.h"
#include "SupplierView.h"
#include "models/receipetablemodel.h"
#include "models/regexpitemdelegate.h"
#include "models/comboboxitemdelegate.h"

namespace Ui {
class ReceipeForm;
}

class ReceipeForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReceipeForm(QWidget *parent = nullptr);
    explicit ReceipeForm(Database *dbs, const QMap<QString, SPositionConfig>& positionsMap, EReceipeType type, QWidget *parent = nullptr);
    ~ReceipeForm();

    void setSupplierList(const QList<SSupplier>& supplierList);
    void setStorageList(const QList<SStorage>& storageList);

private:
    void setLastEditableRow(bool enable);

protected:
    void changeEvent(QEvent*);

signals:
    void receipeCreate(const SReceipe& receipe);

private slots:
    void on_comboBoxSupplier_currentIndexChanged(int index);
    void on_comboBoxStorage_currentIndexChanged(int index);
    void on_pushButtonInsert_clicked(bool);
    void on_pushButtonPlus_clicked(bool);
    void on_pushButtonMinus_clicked(bool);

private:
    Ui::ReceipeForm         *ui;
    Database                *dbs;
    ReceipeTableModel       *receipeTableModel;
    ComboBoxItemDelegate    *index1;
    ComboBoxItemDelegate    *index2;
    ComboBoxItemDelegate    *index3;
    ComboBoxItemDelegate    *index4;

    QMap<QString, SPositionConfig>  m_positionsMap;
    QList<SSupplier>                m_suppliers;
    QList<SStorage>                 m_storages;
    EReceipeType                    m_type;
};

#endif // RECEIPEFORM_H
