#include "listdelegate.h"
#include "../set.h"

const QColor gradientInvalidSelected[3] = {QColor(254, 187, 187), QColor(254, 144, 144), QColor(255, 92, 92)};
const QColor borderInvalidSelected = QColor(255, 15, 15);
const QColor backgroundInvalidOdd = QColor(254, 214, 214);
const QColor backgroundInvalidEven = QColor(254, 214, 214);
const QColor borderInvalid = QColor(255, 15, 15);

const QColor gradientSelected[3] = {QColor(109, 191, 224), QColor(27, 134, 183), QColor(0, 120, 174)};
const QColor borderSelected = QColor(0, 90, 131);
const QColor backgroundOdd = Qt::white;
const QColor backgroundEven = QColor(255, 254, 239);
const QColor border = QColor(211, 211, 211);

const float backgroundOpacityActive = 1.0;
const float backgroundOpacityInactive = 0.8;
const float iconOpacityActive = 1.0;
const float iconOpacityInactive = 0.5;
const float textOpacityActive = 1.0;
const float textOpacityInactive = 0.9;

const QFont titleFont = QFont("Calibri", 11);
const QColor titleColorSelected = QColor(255, 255, 255);
const QColor titleColor = QColor(50, 50, 50);

const QFont pathFont = QFont("Calibri", 9, -1, true);
const QColor pathColorSelected = QColor(220, 220, 220);
const QColor pathColor = QColor(150, 150, 150);

const QFont infoFont = QFont("Calibri", 9, -1, true);
const QColor infoColorSelected = QColor(220, 220, 220);
const QColor infoColor = QColor(150, 150, 150);


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
    QIcon icon;

    int leftMargin = 2;
    int rightMargin1 = 3;
    int rightMargin2 = 2;


    _painter->setOpacity(set->isActive() ? backgroundOpacityActive : backgroundOpacityInactive);

    // BACKGROUND
    rect = baseRect;
    if (!set->isValid())
    {
        if (selected)
        {
            QLinearGradient gradient(rect.left(), rect.top(), rect.left(), rect.height()+rect.top());
            gradient.setColorAt(0.0, gradientInvalidSelected[0]);
            gradient.setColorAt(0.5, gradientInvalidSelected[1]);
            gradient.setColorAt(1.0, gradientInvalidSelected[2]);
            _painter->setBrush(gradient);
            _painter->setPen(borderInvalidSelected);
        }
        else
        {
            _painter->setBrush(_index.row()%2 ? backgroundInvalidOdd : backgroundInvalidEven);
            _painter->setPen(borderInvalid);
        }
    }
    else
    {
        if (selected)
        {
            QLinearGradient gradient(rect.left(), rect.top(), rect.left(), rect.height()+rect.top());
            gradient.setColorAt(0.0, gradientSelected[0]);
            gradient.setColorAt(0.9, gradientSelected[1]);
            gradient.setColorAt(1.0, gradientSelected[2]);
            _painter->setBrush(gradient);
            _painter->setPen(borderSelected);
        }
        else
        {
            _painter->setBrush(_index.row()%2 ? backgroundOdd : backgroundEven);
            _painter->setPen(border);
        }
    }

    _painter->drawRect(rect);


    // STATE ICON
    if (set == m_ctrl->currentSet())
    {
        icon = QIcon(":/images/icons/bullet_yellow.png");
    }
    else if (set->isActive())
    {
        icon = QIcon(":/images/icons/bullet_green.png");
    }
    else
    {
        icon = QIcon(":/images/icons/bullet_red.png");
    }

    rect = baseRect.adjusted(leftMargin, 0, 0, 0);
    icon.paint(_painter, rect, Qt::AlignVCenter|Qt::AlignLeft);
    leftMargin+= 16;


    _painter->setOpacity(set->isActive() ? iconOpacityActive : iconOpacityInactive);

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

    rect = baseRect.adjusted(0, 3, -rightMargin1, 0);
    icon.paint(_painter, rect, Qt::AlignTop|Qt::AlignRight);
    rightMargin1+= 16 + 4;


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

    rect = baseRect.adjusted(0, 0, -rightMargin2, 0);
    icon.paint(_painter, rect, Qt::AlignBottom|Qt::AlignRight);
    rightMargin2+= 20 + 2;


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

    rect = baseRect.adjusted(0, 0, -rightMargin2, 0);
    icon.paint(_painter, rect, Qt::AlignBottom|Qt::AlignRight);
    rightMargin2+= 16 + 4;


    _painter->setOpacity(set->isActive() ? textOpacityActive : textOpacityInactive);

    // HOTKEY
    if (m_settings->param(UM::CONF::use_hotkeys).toBool() && set->hotkey() > 0)
    {
        rect = baseRect.adjusted(0, 3, -rightMargin1, 0);
        _painter->setFont(infoFont);
        _painter->setPen(selected ? infoColorSelected : infoColor);

        QString hotkey = set->hotkeyStr();
        _painter->drawText(rect, Qt::AlignTop|Qt::AlignRight, hotkey);

        QFontMetrics metric(_painter->font());
        rightMargin1+= metric.width(hotkey) + 4;
    }


    // FREQUENCY
    if (set->frequency() != 1)
    {
        rect = baseRect.adjusted(0, 0, -rightMargin2, -2);
        _painter->setFont(infoFont);
        _painter->setPen(selected ? infoColorSelected : infoColor);

        QString frequency = "x" + QString::number(set->frequency());
        _painter->drawText(rect, Qt::AlignBottom|Qt::AlignRight, frequency);

        QFontMetrics metric(_painter->font());
        rightMargin2+= metric.width(frequency) + 4;
    }


    leftMargin+= 1;
    rightMargin1+= 6;
    rightMargin2+= 6;

    // TITLE
    rect = baseRect.adjusted(leftMargin, 1, -rightMargin1, 0);
    _painter->setFont(titleFont);
    _painter->setPen(selected ? titleColorSelected : titleColor);

    QString title = QFontMetrics(_painter->font()).elidedText(set->fullName(), Qt::ElideMiddle, rect.width());
    _painter->drawText(rect, Qt::AlignTop|Qt::AlignLeft, title);


    // PATH
    rect = baseRect.adjusted(leftMargin, 0, -rightMargin2, -2);
    _painter->setFont(pathFont);
    _painter->setPen(selected ? pathColorSelected : pathColor);

    QString path = QFontMetrics(_painter->font()).elidedText(set->path(), Qt::ElideMiddle, rect.width());
    _painter->drawText(rect, Qt::AlignBottom|Qt::AlignLeft, path);
}
