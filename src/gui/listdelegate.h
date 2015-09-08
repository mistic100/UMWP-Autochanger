#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include <QPainter>
#include <QtWidgets/QAbstractItemDelegate>

#include "../settings.h"


/**
 * @brief A custom display of QListWidgetItem with icon and description
 */
class ListDelegate : public QAbstractItemDelegate
{
private:
    Settings* m_settings;

public:
    ListDelegate(QObject* _parent, Settings* _settings);

    void    paint(QPainter* _painter, const QStyleOptionViewItem &_option, const QModelIndex &_index) const;
    QSize   sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
};

#endif // LISTDELEGATE_H
