#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include <QPainter>
#include <QAbstractItemDelegate>
#include "main.h"

/*
 * a custom display of QListWidgetItem
 * with icon and description
 */
class ListDelegate : public QAbstractItemDelegate
{
public:
    ListDelegate(QObject* _parent=0);

    void    paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize   sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // LISTDELEGATE_H
