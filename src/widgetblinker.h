#ifndef WIDGETBLINKER_H
#define WIDGETBLINKER_H

#include <QWidget>
#include <QTimeLine>
#include <QEvent>


/**
 * @brief Util allowing to make any QWidget blink
 */
class WidgetBlinker : public QWidget
{
    Q_OBJECT

private:
    QWidget* m_target;

    QTimeLine* m_timeLine;

public:
    WidgetBlinker(QWidget* _target) : QWidget()
    {
        m_target = _target;

        m_timeLine = new QTimeLine(1000);
        m_timeLine->setFrameRange(0, 2);
        m_timeLine->setLoopCount(0);

        connect(m_timeLine, SIGNAL(frameChanged(int)), this, SLOT(frameChanged(int)));
    }

    bool eventFilter(QObject*, QEvent* _event)
    {
        return _event->type() == QEvent::Paint;
    }

    void start()
    {
        m_timeLine->start();
    }

    void stop()
    {
        show();
        m_timeLine->stop();
    }

    void hide()
    {
        m_target->installEventFilter(this);
        m_target->update();
    }

    void show()
    {
        m_target->removeEventFilter(this);
        m_target->update();
    }

public slots:
    void frameChanged(int _frame)
    {
        if (_frame == 0) hide();
        else if (_frame == 1) show();
    }
};

#endif // WIDGETBLINKER_H
