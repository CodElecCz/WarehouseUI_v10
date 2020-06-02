#include "comboboxitemdelegate.h"
#include <QComboBox>
#include <QApplication>
#include <QMouseEvent>

ComboBoxItemDelegate::ComboBoxItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}


ComboBoxItemDelegate::~ComboBoxItemDelegate()
{

}


QWidget *ComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Create the combobox and populate it
    QComboBox *cb = new QComboBox(parent);
    //const int row = index.row();
    //const int column = index.column();

    cb->addItem(tr("-"));
    cb->addItems(m_list);

    return cb;
}


void ComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    // get the index of the text in the combobox that matches the current value of the item
    const int currentIndex = index.data(Qt::EditRole).toInt();

    // if it is valid, adjust the combobox
    if (currentIndex >= -1)
    {
       cb->setCurrentIndex(currentIndex+1);
    }
}


void ComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    model->setData(index, cb->currentIndex()-1, Qt::EditRole);
}
