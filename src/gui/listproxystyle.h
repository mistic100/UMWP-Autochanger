#ifndef LISTPROXYSTYLE_H
#define LISTPROXYSTYLE_H

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOption>
#include <QtMath>


/**
 * @brief A custom display of QListWidget drop indicator
 */
class ListProxyStyle : public QProxyStyle
{
public:
    void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
    {
        if (element == QStyle::PE_IndicatorItemViewItemDrop)
        {
            if (option->rect.width() == 0) return;

            QRect rect = option->rect.adjusted(0, 1, 0, 1);
            rect.setTop(qFloor(rect.top()/35)*35); // correct 1 pixel offset between bottom of element n and top of element n+1

            painter->setPen(QColor(200,0,100));
            painter->setBrush(QColor(200,0,100));

            QPoint points[8] = {
                QPoint(-4,0),
                QPoint(-1,-3),
                QPoint(0,-3),
                QPoint(3,0),
                QPoint(3,1),
                QPoint(0,4),
                QPoint(-1,4),
                QPoint(-4,1)
            };

            painter->translate(rect.topLeft());
            painter->drawConvexPolygon(points, 8);
            painter->drawRect(4, 0, rect.width()-8, 1);
            painter->translate(rect.width(), 0);
            painter->drawConvexPolygon(points, 8);
        }
        else
        {
            QProxyStyle::drawPrimitive(element, option, painter, widget);
        }
    }
};

#endif // LISTPROXYSTYLE_H
