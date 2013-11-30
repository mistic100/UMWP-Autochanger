#include "listdelegate.h"
#include "set.h"


/**
 * @brief ListDelegate::ListDelegate
 * @param QWidget* _parent
 * @param Controller* _poCtrl
 */
ListDelegate::ListDelegate(QObject* _parent, Controller* _poCtrl) : QAbstractItemDelegate(_parent)
{
    m_poCtrl = _poCtrl;
}

/**
 * @brief ListDelegate::sizeHint
 * @return QSize
 */
QSize ListDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(200, 35);
}

/**
 * @brief ListDelegate::paint
 */
void ListDelegate::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // get configuration
    Set* poSet = m_poCtrl->settings()->poGetSet(index.data(Qt::UserRole).toInt());
    bool selected = option.state & QStyle::State_Selected;
    QRect rect;

    painter->setOpacity(!poSet->isActive() ? 0.7 : 1.0);


    // BACKGROUND
    rect = option.rect.adjusted(0, 0, -1, 0);
    if (selected)
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


    // ICONS
    QIcon a_icon, w_icon, im_icon;
    rect = option.rect.adjusted(3, 3, -3, -1);

    switch (poSet->isActive())
    {
    case true:
        a_icon = QIcon(":/icon/bullet_green"); break;
    default:
        a_icon = QIcon(":/icon/bullet_red"); break;
    }
    a_icon.paint(painter, rect, Qt::AlignVCenter|Qt::AlignLeft);

    switch (poSet->type())
    {
    case 0:
        w_icon = QIcon(":/icon/w_desktop"); break;
    default:
        w_icon = QIcon(":/icon/w_monitor"); break;
    }
    w_icon.paint(painter, rect, Qt::AlignTop|Qt::AlignRight);

    switch (poSet->style())
    {
    case 0:
        im_icon = QIcon(":/icon/im_center"); break;
    case 1:
        im_icon = QIcon(":/icon/im_tile"); break;
    case 2:
        im_icon = QIcon(":/icon/im_stretch"); break;
    default:
        im_icon = QIcon(":/icon/im_stretch_prop"); break;
    }
    im_icon.paint(painter, rect, Qt::AlignBottom|Qt::AlignRight);


    // TITLE
    rect = option.rect.adjusted(20, 1, 0, 0);
    painter->setFont(QFont("Calibri", 11));
    if (selected)
    {
        painter->setPen(Qt::white);
    }
    else
    {
        painter->setPen(QColor(51,51,51));
    }
    painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, poSet->fullName(), &rect);

    // PATH
    rect = option.rect.adjusted(20, 19, 0, 0);
    painter->setFont(QFont("Calibri", 9, -1, true));
    if (selected)
    {
        painter->setPen(Qt::lightGray);
    }
    else
    {
        painter->setPen(Qt::darkGray);
    }
    painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, poSet->path(), &rect);
}
