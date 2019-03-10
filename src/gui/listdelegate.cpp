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

const double backgroundOpacityActive = 1.0;
const double backgroundOpacityInactive = 0.8;
const double iconOpacityActive = 1.0;
const double iconOpacityInactive = 0.5;
const double textOpacityActive = 1.0;
const double textOpacityInactive = 0.9;

const QFont titleFont = QFont("Calibri", 11);
const QColor titleColorSelected = QColor(255, 255, 255);
const QColor titleColor = QColor(50, 50, 50);

const QFont pathFont = QFont("Calibri", 9, -1, true);
const QColor pathColorSelected = QColor(220, 220, 220);
const QColor pathColor = QColor(150, 150, 150);

const QFont infoFont = QFont("Calibri", 9, -1, true);
const QColor infoColorSelected = QColor(220, 220, 220);
const QColor infoColor = QColor(150, 150, 150);

QFont monitorsFont = QFont("Calibri", 8);
const QColor monitorsColor = Qt::black;

QT_BEGIN_NAMESPACE
    extern Q_GUI_EXPORT void qt_blurImage( QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
QT_END_NAMESPACE


/**
 * @brief ListDelegate::ListDelegate
 * @param QObject* _parent
 * @param Settings* _settings
 */
ListDelegate::ListDelegate(QObject* _parent, Controller *_ctrl) :
    QAbstractItemDelegate(_parent),
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings())
{
    monitorsFont.setLetterSpacing(QFont::AbsoluteSpacing, -1);
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
void ListDelegate::paint(QPainter* _painter, const QStyleOptionViewItem &_option, const QModelIndex &_index) const
{
    QImage buffer(_option.rect.width(), _option.rect.height(), QImage::Format_RGB32);
    QPainter painter(&buffer);

    // get configuration
    Set* set = m_settings->set(_index.data(Qt::UserRole).toInt());
    bool locked = m_ctrl->locked();
    bool selected = _option.state & QStyle::State_Selected && !locked;
    QRect baseRect(0, 0, _option.rect.width() - 1 ,_option.rect.height( )- 1);

    QRect rect;
    QIcon icon;

    int leftMargin = 2;
    int rightMargin1 = 3;
    int rightMargin2 = 2;

    painter.setBrush(Qt::white);
    painter.drawRect(baseRect);
    painter.setOpacity(set->isActive() ? backgroundOpacityActive : backgroundOpacityInactive);

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
            painter.setBrush(gradient);
            painter.setPen(borderInvalidSelected);
        }
        else
        {
            painter.setBrush(_index.row()%2 ? backgroundInvalidOdd : backgroundInvalidEven);
            painter.setPen(locked ? _index.row()%2 ? backgroundInvalidOdd : backgroundInvalidEven : borderInvalid);
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
            painter.setBrush(gradient);
            painter.setPen(borderSelected);
        }
        else
        {
            painter.setBrush(_index.row()%2 ? backgroundOdd : backgroundEven);
            painter.setPen(locked ? _index.row()%2 ? backgroundOdd : backgroundEven : border);
        }
    }

    painter.drawRect(rect);


    // STATE ICON
    if (m_ctrl->current().sets.contains(set))
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
    icon.paint(&painter, rect, Qt::AlignVCenter|Qt::AlignLeft);

    painter.setOpacity(set->isActive() ? iconOpacityActive : iconOpacityInactive);

    // LOCK ICON
    if (m_ctrl->lockEnabled() == UM::LOCK_SETS && set->lock() == TRUE_BOOL) {
        icon = QIcon(":/images/icons/bullet_lock.png");
        rect = baseRect.adjusted(leftMargin, 0, 0, 0);
        icon.paint(&painter, rect, Qt::AlignBottom|Qt::AlignLeft);
    }

    leftMargin+= 16;

    // TYPE ICON
    switch (set->type())
    {
    case UM::W_DESKTOP:
        icon = QIcon(":/images/icons/w_desktop.png");
        break;
    case UM::W_MONITOR:
        icon = QIcon(":/images/icons/w_monitor.png");
        break;
    case UM::W_NONE:
        break;
    }

    rect = baseRect.adjusted(0, 3, -rightMargin1, 0);
    icon.paint(&painter, rect, Qt::AlignTop|Qt::AlignRight);
    rightMargin1+= 16 + 4;


    // MONITORS
    // multiline is handled manually because QFont does not have a lineHeight option
    // the display does not allow more than 6 monitors
    if (set->type() == UM::W_MONITOR && !set->monitors().isEmpty())
    {
        QString monitors, monitors2;
        for (int i = 0; i < m_ctrl->enviro()->nbMonitors(); i++)
        {
            if (m_settings->monitor(i).enabled && set->isActiveOnMonitor(i))
            {
                if (monitors.size() < 5)
                {
                    if (!monitors.isEmpty())
                    {
                        monitors+= "-";
                    }
                    monitors+= QString::number(i+1);
                }
                else if (monitors2.size() < 5)
                {
                    if (!monitors2.isEmpty())
                    {
                        monitors2+= "-";
                    }
                    monitors2+= QString::number(i+1);
                }
            }
        }

        rect = QRect(baseRect.width()-rightMargin1+2, baseRect.top(), 20, 16);

        painter.setFont(monitorsFont);
        painter.setPen(monitorsColor);

        painter.drawText(rect, monitors);

        if (!monitors2.isEmpty())
        {
            rect.translate(0, 8);

            painter.drawText(rect, monitors2);
        }
    }


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
    case UM::IM_NONE:
        break;
    }

    rect = baseRect.adjusted(0, 0, -rightMargin2, 0);
    icon.paint(&painter, rect, Qt::AlignBottom|Qt::AlignRight);
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
    case UM::MODE_NONE:
        break;
    }

    rect = baseRect.adjusted(0, 0, -rightMargin2, 0);
    icon.paint(&painter, rect, Qt::AlignBottom|Qt::AlignRight);
    rightMargin2+= 16 + 4;


    painter.setOpacity(set->isActive() ? textOpacityActive : textOpacityInactive);

    // HOTKEY
    if (m_settings->param(UM::CONF::use_hotkeys).toBool() && set->hotkey() > 0)
    {
        rect = baseRect.adjusted(0, 3, -rightMargin1, 0);
        painter.setFont(infoFont);
        painter.setPen(selected ? infoColorSelected : infoColor);

        QString hotkey = set->hotkeyStr();
        painter.drawText(rect, Qt::AlignTop|Qt::AlignRight, hotkey);

        QFontMetrics metric(painter.font());
        rightMargin1+= metric.width(hotkey) + 4;
    }


    // FREQUENCY
    if (set->frequency() != 1)
    {
        rect = baseRect.adjusted(0, 0, -rightMargin2, -2);
        painter.setFont(infoFont);
        painter.setPen(selected ? infoColorSelected : infoColor);

        QString frequency = "x" + QString::number(set->frequency());
        painter.drawText(rect, Qt::AlignBottom|Qt::AlignRight, frequency);

        QFontMetrics metric(painter.font());
        rightMargin2+= metric.width(frequency) + 4;
    }


    leftMargin+= 1;
    rightMargin1+= 6;
    rightMargin2+= 6;


    // TITLE
    rect = baseRect.adjusted(leftMargin, 1, -rightMargin1, 0);
    painter.setFont(titleFont);
    painter.setPen(selected ? titleColorSelected : titleColor);

    QString title = QFontMetrics(painter.font()).elidedText(set->fullName(), Qt::ElideMiddle, rect.width());
    painter.drawText(rect, Qt::AlignTop|Qt::AlignLeft, title);


    // PATH
    rect = baseRect.adjusted(leftMargin, 0, -rightMargin2, -2);
    painter.setFont(pathFont);
    painter.setPen(selected ? pathColorSelected : pathColor);

    QString path = QFontMetrics(painter.font()).elidedText(set->path(), Qt::ElideMiddle, rect.width());
    painter.drawText(rect, Qt::AlignBottom|Qt::AlignLeft, path);


    painter.end();

    if (locked)
    {
        _painter->translate(-1, _option.rect.top() - 1);
        _painter->scale(1 + (double) 2 / _option.rect.width(), 1 + (double) 2 / _option.rect.height());

        qt_blurImage(_painter, buffer, 10, false, false);

        _painter->resetTransform();
    }
    else
    {
        _painter->drawImage(_option.rect, buffer);
    }
}
