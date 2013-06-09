#include "listproxystyle.h"


ListProxyStyle::ListProxyStyle() {}

void ListProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    if (element == QStyle::PE_IndicatorItemViewItemDrop)
    {
        QRect rect = option->rect.adjusted(0, 1, 0, 1);

        if (rect.width() == 0) return;

        painter->setPen(QColor(200,0,100));
        painter->setBrush(QColor(200,0,100));

        QPoint points[4] = {
            rect.topLeft()+QPoint(-3,0),
            rect.topLeft()+QPoint(0,-3),
            rect.topLeft()+QPoint(3,0),
            rect.topLeft()+QPoint(0,3),
        };

        painter->drawConvexPolygon(points, 4);
        painter->drawLine(rect.topLeft(), rect.topRight());
        painter->translate(QPoint(rect.width()-1,0));
        painter->drawConvexPolygon(points, 4);
    }
    else
    {
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}
