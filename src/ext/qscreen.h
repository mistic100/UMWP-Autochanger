#ifndef QSCREEN_H
#define QSCREEN_H

#include <QRect>


/**
 * @brief Object with width, height, top and left properties
 */
class QScreen : public QRect
{

public:
    QScreen() : QRect() {}
    QScreen(int x, int y, int w, int h) : QRect(x, y, w, h) {}

    QScreen scaled(float _ratio) const {
        return QScreen(
                    left()  *_ratio,
                    top()   *_ratio,
                    width() *_ratio,
                    height()*_ratio
        );
    }
};

#endif // QSCREEN_H
