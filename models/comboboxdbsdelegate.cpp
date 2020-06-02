#include "comboboxdbsdelegate.h"
#include <QComboBox>

ComboBoxDbsDelegate::ComboBoxDbsDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

ComboBoxDbsDelegate::ComboBoxDbsDelegate(const SComboBoxDbsSetup &setup, QObject *parent)
    : QStyledItemDelegate(parent),
        dbs(setup.dbs),
        m_table(setup.table),
        m_column(setup.column),
        m_filter(setup.filter),
        m_filterColumnName(setup.filterColumnName),
        m_filterColumnIndex(setup.filterColumnIndex)
{

}


ComboBoxDbsDelegate::~ComboBoxDbsDelegate()
{

}

QWidget* ComboBoxDbsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Create the combobox and populate it
    QComboBox *cb = new QComboBox(parent);
    const int row = index.row();
    //const int column = index.column();

    cb->addItem(tr("- select -"));

    QString squery("");
    if(m_filter)
    {
        QModelIndex modelIndex = index.model()->index(row, m_filterColumnIndex);
        QString filterColumnVal = modelIndex.data(Qt::DisplayRole).toString();

        if(!filterColumnVal.isEmpty())
            squery = QString("SELECT %1, id FROM %2 WHERE %3='%4'").arg(m_column, m_table, m_filterColumnName, filterColumnVal);        
    }
    else
        squery = QString("SELECT DISTINCT %1 FROM %2").arg(m_column, m_table);

    if(!squery.isEmpty())
    {
        bool ok = dbs->query(squery);
        if(ok)
        {
            if(m_filter)
            {
                QStringList listNames = dbs->queryResult(0);
                QStringList listIds = dbs->queryResult(1);

                int index = 0;
                foreach(QString s, listNames)
                {
                    if(listIds.size()>index)
                        cb->addItem(s, listIds[index++]);
                    else
                        cb->addItem(s);
                }
            }
            else
            {
                QStringList listNames = dbs->queryResult(0);

                foreach(QString s, listNames)
                {
                    cb->addItem(s);
                }
            }
        }
    }

    return cb;
}


void ComboBoxDbsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);

    // get the index of the text in the combobox that matches the current value of the item
    const QString currentText = index.data(Qt::EditRole).toString();   
    const int cbIndex = cb->findText(currentText);
    const int row = index.row();

    // if it is valid, adjust the combobox
    if (cbIndex >= 0)
        cb->setCurrentIndex(cbIndex);
    else
        cb->setCurrentIndex(0);
}


void ComboBoxDbsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);

    if(cb->currentIndex()>0)
    {
        model->setData(index, cb->currentText(), Qt::EditRole);
    }
    else //first 'select' item
        model->setData(index, "", Qt::EditRole);

    //UserRole - custom data
    QVariant userData = cb->itemData(cb->currentIndex());
    if(!userData.isNull())
    {
        int stock_id = userData.toInt();
        model->setData(index, userData, Qt::UserRole);
    }
}
