#include "progressbaritemdelegate.h"
#include <QProgressBar>
#include <QApplication>
#include <QItemDelegate>
#include <QStyleOptionViewItem>

ProgressBarItemDelegate::ProgressBarItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

ProgressBarItemDelegate::~ProgressBarItemDelegate()
{
}

void ProgressBarItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());
    const int progress = index.data(Qt::DisplayRole).toInt();

    if(progress>0 && progress<=100)
    {
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
    else if(progress==0)
    {
        Q_ASSERT(index.isValid());
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        opt.text = QString("%1%").arg(progress);
        opt.displayAlignment = Qt::AlignHCenter;
        const QWidget *widget = option.widget;
        QStyle *style = widget ? widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
    }
    else //over limit red
    {
        Q_ASSERT(index.isValid());
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        opt.text = QString("%1%").arg(progress);
        opt.displayAlignment = Qt::AlignHCenter;
        opt.backgroundBrush = QBrush(QColor(220, 0, 0));
        const QWidget *widget = option.widget;
        QStyle *style = widget ? widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
    }

}
