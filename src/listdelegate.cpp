#include "listdelegate.h"
#include "set.h"


/**
 * @brief ListDelegate::ListDelegate
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
ListDelegate::ListDelegate(QObject* _parent, Settings* _settings) : QAbstractItemDelegate(_parent)
{
    m_settings = _settings;
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
    Set* set = m_settings->set(index.data(Qt::UserRole).toInt());
    bool selected = option.state & QStyle::State_Selected;
    QRect baseRect = option.rect.adjusted(0, 0, -1, 0);
    QRect rect;


    painter->setOpacity(!set->isActive() ? 0.8 : 1.0);

    // BACKGROUND
    rect = baseRect;
    if (!set->isValid())
    {
        QLinearGradient gradientSelected(rect.left(), rect.top(), rect.left(), rect.height()+rect.top());
        gradientSelected.setColorAt(0.0, QColor(254, 187, 187));
        gradientSelected.setColorAt(0.45, QColor(254, 144, 144));
        gradientSelected.setColorAt(1.0, QColor(255, 92, 92));

        painter->setPen(QColor(255, 15, 15));
        painter->setBrush(gradientSelected);
        painter->drawRect(rect);
    }
    else if (selected)
    {
        QLinearGradient gradientSelected(rect.left(), rect.top(), rect.left(), rect.height()+rect.top());
        gradientSelected.setColorAt(0.0, QColor(109, 191, 224));
        gradientSelected.setColorAt(0.9, QColor(27, 134, 183));
        gradientSelected.setColorAt(1.0, QColor(0, 120, 174));

        painter->setPen(QColor(0, 90, 131));
        painter->setBrush(gradientSelected);
        painter->drawRect(rect);
    }
    else
    {
        painter->setPen(QColor(211, 211, 211));
        painter->setBrush(index.row()%2 ? Qt::white : QColor(255, 254, 239)); // alternating colors
        painter->drawRect(rect);
    }


    // STATE ICON
    QIcon a_icon, w_icon, im_icon;

    switch (set->isActive())
    {
    case true:
        a_icon = QIcon(":/icon/bullet_green"); break;
    case false:
        a_icon = QIcon(":/icon/bullet_red"); break;
    }
    rect = baseRect.adjusted(2, 0, 0, 0);
    a_icon.paint(painter, rect, Qt::AlignVCenter|Qt::AlignLeft);


    painter->setOpacity(!set->isActive() ? 0.5 : 1.0);

    // MODE ICON
    switch (set->type())
    {
    case 0:
        w_icon = QIcon(":/icon/w_desktop"); break;
    case 1:
        w_icon = QIcon(":/icon/w_monitor"); break;
    }
    rect = baseRect.adjusted(0, 3, -3, 0);
    w_icon.paint(painter, rect, Qt::AlignTop|Qt::AlignRight);


    // IMAGE ICON
    switch (set->style())
    {
    case 0:
        im_icon = QIcon(":/icon/im_center"); break;
    case 1:
        im_icon = QIcon(":/icon/im_tile"); break;
    case 2:
        im_icon = QIcon(":/icon/im_stretch"); break;
    case 3:
        im_icon = QIcon(":/icon/im_stretch_prop"); break;
    case 4:
        im_icon = QIcon(":/icon/im_fill"); break;
    }
    rect = baseRect.adjusted(0, 0, -2, 0);
    im_icon.paint(painter, rect, Qt::AlignBottom|Qt::AlignRight);


    painter->setOpacity(!set->isActive() ? 0.9 : 1.0);

    // HOTKEY
    rect = baseRect.adjusted(0, 3, -23, 0);
    painter->setFont(QFont("Calibri", 9, -1, true));
    if (selected)
    {
        painter->setPen(QColor(220, 220, 220));
    }
    else
    {
        painter->setPen(QColor(150, 150, 150));
    }

    QString hotkey = QKeySequence(set->hotkey()).toString(QKeySequence::NativeText);
    painter->drawText(rect, Qt::AlignTop|Qt::AlignRight, hotkey);

    QFontMetrics metric(painter->font());
    int hkWidth = metric.width(hotkey);


    // TITLE
    rect = baseRect.adjusted(19, 1, -hkWidth-26, 0);
    painter->setFont(QFont("Calibri", 11));
    if (selected)
    {
        painter->setPen(QColor(255, 255, 255));
    }
    else
    {
        painter->setPen(QColor(50, 50, 50));
    }

    QString title = QFontMetrics(painter->font()).elidedText(set->fullName(), Qt::ElideMiddle, rect.width());
    painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, title);


    // PATH
    rect = baseRect.adjusted(19, 19, -25, 0);
    painter->setFont(QFont("Calibri", 9, -1, true));
    if (selected)
    {
        painter->setPen(QColor(220, 220, 220));
    }
    else
    {
        painter->setPen(QColor(150, 150, 150));
    }

    QString path = QFontMetrics(painter->font()).elidedText(set->path(), Qt::ElideMiddle, rect.width());
    painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, path);
}
