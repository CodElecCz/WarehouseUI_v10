#ifndef COMBOBOXDBSDELEGATE_H
#define COMBOBOXDBSDELEGATE_H

#include <QStyledItemDelegate>
#include "dbs/database.h"

typedef struct _SComboBoxDbsSetup
{
    Database* dbs;
    QString table;
    QString column;

    bool    filter;
    QString filterColumnName;
    int     filterColumnIndex;
} SComboBoxDbsSetup;

class ComboBoxDbsDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:              
    ComboBoxDbsDelegate(QObject *parent = nullptr);
    ComboBoxDbsDelegate(const SComboBoxDbsSetup& setup, QObject *parent = nullptr);
    ~ComboBoxDbsDelegate();

    // editing
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
    Database*   dbs;
    QString     m_table;
    QString     m_column;

    bool        m_filter;
    QString     m_filterColumnName;
    int         m_filterColumnIndex;   
};

#endif // COMBOBOXDBSDELEGATE_H
