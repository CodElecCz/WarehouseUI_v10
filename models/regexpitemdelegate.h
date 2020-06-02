#ifndef REGEXPITEMDELEGATE_H
#define REGEXPITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "dbs/database.h"

class RegExpItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    RegExpItemDelegate(const QString& regExp, QObject *parent = nullptr);
    ~RegExpItemDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void setRegExp(const QString& regExp) { m_regExp.setPattern(regExp); };

private slots:
    void on_editor_returnPressed();

private:
    QRegularExpression  m_regExp;
};

#endif // REGEXPITEMDELEGATE_H
