#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include <QPainter>
#include <QAbstractItemDelegate>

#include "../settings.h"
#include "../controller.h"


/**
 * @brief A custom display of QListWidgetItem with icon and description
 */
class ListDelegate : public QAbstractItemDelegate
{
private:
    Controller* m_ctrl;
    Settings*   m_settings;

public:
    ListDelegate(QObject* _parent, Controller* _ctrl);

    void paint(QPainter* _painter, const QStyleOptionViewItem &_option, const QModelIndex &_index) const;
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
};

#endif // LISTDELEGATE_H
