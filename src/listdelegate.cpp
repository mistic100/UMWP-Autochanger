#include "listdelegate.h"


ListDelegate::ListDelegate(QObject* _parent) : QAbstractItemDelegate(_parent) {}

QSize ListDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(200, 35);
}

void ListDelegate::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // get configuration
    bool active = option.state & QStyle::State_Selected;
    QRect rect = option.rect.adjusted(0, 0, -1, 0);
    QIcon icon = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole+1).toString();
    bool transparent = !index.data(Qt::UserRole+2).toBool();

    painter->setOpacity(transparent ? 0.7 : 1.0);

    // BACKGROUND
    if (active)
    {
        QLinearGradient gradientSelected(rect.left(), rect.top(), rect.left(), rect.height()+rect.top());
        gradientSelected.setColorAt(0.0, QColor(119,213,247));
        gradientSelected.setColorAt(0.9, QColor(27,134,183));
        gradientSelected.setColorAt(1.0, QColor(0,120,174));

        painter->setPen(QColor(0,90,131));
        painter->setBrush(gradientSelected);
        painter->drawRoundedRect(rect, 3, 3);
    }
    else
    {
        painter->setPen(QColor(211,211,211));
        painter->setBrush( index.row()%2 ? Qt::white : QColor(255,254,239) ); // alternating colors
        painter->drawRoundedRect(rect, 2, 2);
    }

    int imageSpace = 5;
    // ICON
    if (!icon.isNull())
    {
        rect = option.rect.adjusted(3, 0, 0, 0);
        icon.paint(painter, rect, Qt::AlignVCenter|Qt::AlignLeft);
        imageSpace = 20;
    }

    // TITLE
    rect = option.rect.adjusted(imageSpace, 1, 0, 0);
    painter->setFont(QFont("Calibri", 11));
    if (active) painter->setPen(Qt::white);
           else painter->setPen(QColor(51,51,51));
    painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, title, &rect);

    // DESCRIPTION
    rect = option.rect.adjusted(imageSpace, 19, 0, 0);
    painter->setFont(QFont("Calibri", 9, -1, true));
    if (active) painter->setPen(Qt::lightGray);
           else painter->setPen(Qt::darkGray);
    painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, description, &rect);
}
