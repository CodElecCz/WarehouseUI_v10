#ifndef PROGRESSBARITEMDELEGATE_H
#define PROGRESSBARITEMDELEGATE_H

#include <QStyledItemDelegate>

class ProgressBarItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ProgressBarItemDelegate(QObject *parent = nullptr);

    ~ProgressBarItemDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:

};

#endif // COMBOBOXITEMDELEGATE_H
