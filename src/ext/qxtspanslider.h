#ifndef QXTSPANSLIDER_H
/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/

#define QXTSPANSLIDER_H

#include <QSlider>
#include <QStyle>
#include <QObject>

QT_FORWARD_DECLARE_CLASS(QStylePainter)
QT_FORWARD_DECLARE_CLASS(QStyleOptionSlider)

class QxtSpanSlider : public QSlider
{
    Q_OBJECT

public:
    enum HandleMovementMode
    {
        FreeMovement,
        NoCrossing,
        NoOverlapping
    };

    enum SpanHandle
    {
        NoHandle,
        LowerHandle,
        UpperHandle
    };
    
    Q_PROPERTY(int lowerValue READ lowerValue WRITE setLowerValue)
    Q_PROPERTY(int upperValue READ upperValue WRITE setUpperValue)
    Q_PROPERTY(int lowerPosition READ lowerPosition WRITE setLowerPosition)
    Q_PROPERTY(int upperPosition READ upperPosition WRITE setUpperPosition)
    Q_PROPERTY(HandleMovementMode handleMovementMode READ handleMovementMode WRITE setHandleMovementMode)
    Q_ENUMS(HandleMovementMode)
    
private:
    int lower = 0;
    int upper = 0;
    int lowerPos = 0;
    int upperPos = 0;
    int offset = 0;
    int position = 0;
    QxtSpanSlider::SpanHandle lastPressed = QxtSpanSlider::NoHandle;
    QxtSpanSlider::SpanHandle mainControl = QxtSpanSlider::LowerHandle;
    QStyle::SubControl lowerPressed = QStyle::SC_None;
    QStyle::SubControl upperPressed = QStyle::SC_None;
    QxtSpanSlider::HandleMovementMode movement = QxtSpanSlider::FreeMovement;
    bool firstMovement = false;
    bool blockTracking = false;

public:
    explicit QxtSpanSlider(QWidget* parent = nullptr);
    explicit QxtSpanSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    virtual ~QxtSpanSlider();

    HandleMovementMode handleMovementMode() const;
    void setHandleMovementMode(HandleMovementMode mode);

    int lowerValue() const;
    int upperValue() const;

    int lowerPosition() const;
    int upperPosition() const;
    
private:
    void initStyleOption(QStyleOptionSlider* option, QxtSpanSlider::SpanHandle handle = QxtSpanSlider::UpperHandle) const;
    int pick(const QPoint& pt) const;
    int pixelPosToRangeValue(int pos) const;
    void handleMousePress(const QPoint& pos, QStyle::SubControl& control, int value, QxtSpanSlider::SpanHandle handle);
    void drawHandle(QStylePainter* painter, QxtSpanSlider::SpanHandle handle) const;
    void setupPainter(QPainter* painter, Qt::Orientation orientation, qreal x1, qreal y1, qreal x2, qreal y2) const;
    void drawSpan(QStylePainter* painter, const QRect& rect) const;
    void triggerAction(QAbstractSlider::SliderAction action, bool main);
    void swapControls();

public Q_SLOTS:
    void setLowerValue(int lower);
    void setUpperValue(int upper);
    void setSpan(int lower, int upper);

    void setLowerPosition(int lower);
    void setUpperPosition(int upper);

private Q_SLOTS:
    void updateRange(int min, int max);
    void movePressedHandle();

Q_SIGNALS:
    void spanChanged(int lower, int upper);
    void lowerValueChanged(int lower);
    void upperValueChanged(int upper);

    void lowerPositionChanged(int lower);
    void upperPositionChanged(int upper);

    void sliderPressed(SpanHandle handle);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent* event);
};

#endif // QXTSPANSLIDER_H
