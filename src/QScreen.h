#ifndef QSCREEN_H
#define QSCREEN_H

#include <QSize>

/**
 * @brief Object with width, height, top and left properties
 */
class QScreen : public QSize
{
private:
    int _top;
    int _left;

public:
    QScreen() : QSize(0, 0), _top(0), _left(0) {}
    QScreen(int w, int h) : QSize(w, h), _top(0), _left(0) {}
    QScreen(int w, int h, int t, int l) : QSize(w, h), _top(t), _left(l) {}

    const int left() const { return _left; }
    const int top() const { return _top; }

    void setLeft(int l) { _left = l; }
    void setTop(int t) { _top = t; }
};

#endif // QSCREEN_H
