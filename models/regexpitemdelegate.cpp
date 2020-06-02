#include "regexpitemdelegate.h"
#include <QLineEdit>

RegExpItemDelegate::RegExpItemDelegate(const QString &regExp, QObject *parent)
    : QStyledItemDelegate(parent),
      m_regExp(regExp)
{

}

RegExpItemDelegate::~RegExpItemDelegate()
{

}


QWidget *RegExpItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setFrame(false);

    auto *validator = new QRegularExpressionValidator(m_regExp, parent);
    editor->setValidator(validator);

    connect(editor, SIGNAL(returnPressed()), this, SLOT(on_editor_returnPressed()));

    return editor;
}

void RegExpItemDelegate::on_editor_returnPressed()
{
    QLineEdit *le = qobject_cast<QLineEdit *>(sender());
    Q_ASSERT(le);
}

void RegExpItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *le = qobject_cast<QLineEdit *>(editor);
    Q_ASSERT(le);

    // get the index of the text in the combobox that matches the current value of the item
    const QString currentText = index.data(Qt::EditRole).toString();   

    // if it is valid, adjust the combobox
    le->setText(currentText);
}


void RegExpItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *le = qobject_cast<QLineEdit *>(editor);
    Q_ASSERT(le);
    model->setData(index, le->text(), Qt::EditRole);
}
