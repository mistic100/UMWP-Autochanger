#include "listdelegate.h"
#include "../set.h"


/**
 * @brief ListDelegate::ListDelegate
 * @param QObject* _parent
 * @param Settings* _settings
 */
ListDelegate::ListDelegate(QObject* _parent, Controller *_ctrl) :
    QAbstractItemDelegate(_parent),
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings())
{}

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
void ListDelegate::paint(QPainter* _painter, const QStyleOptionViewItem &_option, const QModelIndex &_index) const
{
    // get configuration
    Set* set = m_settings->set(_index.data(Qt::UserRole).toInt());
    bool selected = _option.state & QStyle::State_Selected;
    QRect baseRect = _option.rect.adjusted(0, 0, -1, 0);
    QRect rect;


    _painter->setOpacity(!set->isActive() ? 0.8 : 1.0);

    // BACKGROUND
    rect = baseRect;
    if (!set->isValid())
    {
        if (selected)
        {
            QLinearGradient gradient(rect.left(), rect.top(), rect.left(), rect.height()+rect.top());
            gradient.setColorAt(0.0,  QColor(254, 187, 187));
            gradient.setColorAt(0.45, QColor(254, 144, 144));
            gradient.setColorAt(1.0,  QColor(255, 92, 92));
            _painter->setBrush(gradient);
        }
        else
        {
            _painter->setBrush(QColor(254, 214, 214));
        }

        _painter->setPen(QColor(255, 15, 15));
    }
    else if (selected)
    {
        QLinearGradient gradient(rect.left(), rect.top(), rect.left(), rect.height()+rect.top());
        gradient.setColorAt(0.0, QColor(109, 191, 224));
        gradient.setColorAt(0.9, QColor(27, 134, 183));
        gradient.setColorAt(1.0, QColor(0, 120, 174));
        _painter->setBrush(gradient);

        _painter->setPen(QColor(0, 90, 131));
    }
    else
    {
        _painter->setPen(QColor(211, 211, 211));
        _painter->setBrush(_index.row()%2 ? Qt::white : QColor(255, 254, 239));
    }

    _painter->drawRect(rect);


    // STATE ICON
    QIcon icon;

    if (set == m_ctrl->currentSet())
    {
        icon = QIcon(":/images/icons/bullet_yellow.png");
    }
    else
    {
        switch (set->isActive())
        {
        case true:
            icon = QIcon(":/images/icons/bullet_green.png");
            break;
        case false:
            icon = QIcon(":/images/icons/bullet_red.png");
            break;
        }
    }

    rect = baseRect.adjusted(2, 0, 0, 0);
    icon.paint(_painter, rect, Qt::AlignVCenter|Qt::AlignLeft);


    _painter->setOpacity(!set->isActive() ? 0.5 : 1.0);

    // TYPE ICON
    switch (set->type())
    {
    case UM::W_DESKTOP:
        icon = QIcon(":/images/icons/w_desktop.png");
        break;
    case UM::W_MONITOR:
        icon = QIcon(":/images/icons/w_monitor.png");
        break;
    }
    rect = baseRect.adjusted(0, 3, -3, 0);
    icon.paint(_painter, rect, Qt::AlignTop|Qt::AlignRight);


    // STYLE ICON
    switch (set->style())
    {
    case UM::IM_CENTER:
        icon = QIcon(":/images/icons/im_center.png");
        break;
    case UM::IM_TILE:
        icon = QIcon(":/images/icons/im_tile.png");
        break;
    case UM::IM_STRETCH:
        icon = QIcon(":/images/icons/im_stretch.png");
        break;
    case UM::IM_STRETCH_PROP:
        icon = QIcon(":/images/icons/im_stretch_prop.png");
        break;
    case UM::IM_FILL:
        icon = QIcon(":/images/icons/im_fill.png");
        break;
    case UM::IM_CUSTOM:
        icon = QIcon(":/images/icons/im_custom.png");
        break;
    }
    rect = baseRect.adjusted(0, 0, -2, 0);
    icon.paint(_painter, rect, Qt::AlignBottom|Qt::AlignRight);


    // MODE ICON
    switch (set->mode())
    {
    case UM::MODE_RANDOM:
        icon = QIcon(":/images/icons/mode_random.png");
        break;
    case UM::MODE_SEQUENTIAL:
        icon = QIcon(":/images/icons/mode_sequential.png");
        break;
    }
    rect = baseRect.adjusted(0, 0, -24, 0);
    icon.paint(_painter, rect, Qt::AlignBottom|Qt::AlignRight);


    _painter->setOpacity(!set->isActive() ? 0.9 : 1.0);

    // HOTKEY
    int hkWidth = 0;

    if (m_settings->param(UM::CONF::use_hotkeys).toBool() && set->hotkey() > 0)
    {
        rect = baseRect.adjusted(0, 3, -23, 0);
        _painter->setFont(QFont("Calibri", 9, -1, true));
        if (selected)
        {
            _painter->setPen(QColor(220, 220, 220));
        }
        else
        {
            _painter->setPen(QColor(150, 150, 150));
        }

        QString hotkey = set->hotkeyStr();
        _painter->drawText(rect, Qt::AlignTop|Qt::AlignRight, hotkey);

        QFontMetrics metric(_painter->font());
        hkWidth = metric.width(hotkey);
    }


    // TITLE
    rect = baseRect.adjusted(19, 1, -hkWidth-26, 0);
    _painter->setFont(QFont("Calibri", 11));
    if (selected)
    {
        _painter->setPen(QColor(255, 255, 255));
    }
    else
    {
        _painter->setPen(QColor(50, 50, 50));
    }

    QString title = QFontMetrics(_painter->font()).elidedText(set->fullName(), Qt::ElideMiddle, rect.width());
    _painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, title);


    // PATH
    rect = baseRect.adjusted(19, 19, -25, 0);
    _painter->setFont(QFont("Calibri", 9, -1, true));
    if (selected)
    {
        _painter->setPen(QColor(220, 220, 220));
    }
    else
    {
        _painter->setPen(QColor(150, 150, 150));
    }

    QString path = QFontMetrics(_painter->font()).elidedText(set->path(), Qt::ElideMiddle, rect.width());
    _painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, path);
}
