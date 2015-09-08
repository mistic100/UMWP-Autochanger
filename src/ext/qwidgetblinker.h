#ifndef QWIDGETBLINKER_H
#define QWIDGETBLINKER_H

#include <QtWidgets/QWidget>
#include <QTimeLine>
#include <QEvent>


/**
 * @brief Utility allowing to make any QWidget blink
 */
class QWidgetBlinker : public QWidget
{
    Q_OBJECT

private:
    QTimeLine* m_timeLine;

public:
    QWidgetBlinker(QWidget* _parent, int _duration=1000) : QWidget(_parent)
    {
        m_timeLine = new QTimeLine(_duration, this);
        m_timeLine->setFrameRange(0, 2);
        m_timeLine->setLoopCount(0);

        connect(m_timeLine, SIGNAL(frameChanged(int)), this, SLOT(frameChanged(int)));
    }

    void start()
    {
        hide();
        m_timeLine->start();
    }

    void stop()
    {
        show();
        m_timeLine->stop();
    }

private:
    void hide()
    {
        parentWidget()->installEventFilter(this);
        parentWidget()->update();
    }

    void show()
    {
        parentWidget()->removeEventFilter(this);
        parentWidget()->update();
    }

protected:
    bool eventFilter(QObject*, QEvent* _event)
    {
        return _event->type() == QEvent::Paint;
    }

private slots:
    void frameChanged(int _frame)
    {
        _frame == 0 ?  hide() : show();
    }
};

#endif // QWIDGETBLINKER_H
