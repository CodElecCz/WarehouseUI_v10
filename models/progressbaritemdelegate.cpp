#include "progressbaritemdelegate.h"
#include <QProgressBar>
#include <QApplication>
#include <QItemDelegate>

ProgressBarItemDelegate::ProgressBarItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

ProgressBarItemDelegate::~ProgressBarItemDelegate()
{
}

void ProgressBarItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int progress = index.data(Qt::DisplayRole).toInt();

    QStyleOptionProgressBar progressBarOption;
    progressBarOption.rect = option.rect;

    progressBarOption.minimum = 0;
    progressBarOption.maximum = 100;
    progressBarOption.textAlignment = Qt::AlignCenter;
    progressBarOption.progress = (progress>100)? 100:progress;
    progressBarOption.text = QString("%1%").arg(progress);
    progressBarOption.textVisible = true;

    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}
