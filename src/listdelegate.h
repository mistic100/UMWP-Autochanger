#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include <QPainter>
#include <QAbstractItemDelegate>

#include "main.h"
#include "settings.h"


/**
 * @brief A custom display of QListWidgetItem with icon and description
 */
class ListDelegate : public QAbstractItemDelegate
{
private:
    Settings* m_settings;

public:
    ListDelegate(QObject* _parent, Settings* _settings);

    void    paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize   sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
};

#endif // LISTDELEGATE_H
