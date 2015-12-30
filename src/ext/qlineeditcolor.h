#ifndef QLINEEDITCOLOR
#define QLINEEDITCOLOR

#include <QLineEdit>
#include <QColorDialog>
#include <QEvent>

/**
 * @brief A QLineEdit combined with a QColorDialog
 * @signal colorChanged(QColor)
 */
class QLineEditColor : public QLineEdit
{
    Q_OBJECT

private:
    QColor m_color;

public:
    QLineEditColor(QWidget* _parent) : QLineEdit(_parent)
    {
        installEventFilter(this);
        setReadOnly(true);
        setColor(QColor(Qt::black), false);
    }

    /**
     * @brief Open the color picker on click
     * @param QEvent* _event
     * @return bool
     */
    bool eventFilter(QObject*, QEvent* _event)
    {
        if (_event->type() == QEvent::MouseButtonPress)
        {
            QColor color = QColorDialog::getColor(m_color, this, tr("Select color"));

            if (color.isValid())
            {
                setColor(color, true);
            }

            return false;
        }

        return false;
    }

    /**
     * @brief Set the color of the picker
     * @param QColor _color
     */
    void setColor(const QColor &_color)
    {
        setColor(_color, false);
    }

    /**
     * @brief Get the selected color
     * @return QColor
     */
    QColor getColor() const
    {
        return m_color;
    }

signals:
    /**
     * @brief Emitted when the user changes the color
     */
    void colorChanged(QColor);

private:
    /**
     * @brief Set the color of the picker and optionally emit a signal
     * @param QColor _color
     * @param bool _emitEvent
     */
    void setColor(const QColor &_color, bool _emitEvent)
    {
        m_color = _color;

        QPalette palette;
        palette.setColor(QPalette::Base, m_color);
        palette.setColor(QPalette::Text, getTextColor());

        setPalette(palette);
        setText(m_color.name());

        if (_emitEvent)
        {
            emit colorChanged(m_color);
        }
    }

    /**
     * @brief Compute the best text color
     * @return Qcolor
     */
    QColor getTextColor()
    {
        double luminance = 1 - (0.299 * m_color.red() + 0.587 * m_color.green() + 0.114 * m_color.blue()) / 255;

        if (luminance < 0.5)
        {
            return QColor(Qt::black);
        }
        else
        {
            return QColor(Qt::white);
        }
    }
};

#endif // QLINEEDITCOLOR
