#include "listproxystyle.h"


/**
 * @brief ListProxyStyle::ListProxyStyle
 */
ListProxyStyle::ListProxyStyle() {}

/**
 * @brief ListProxyStyle::drawPrimitive
 */
void ListProxyStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    if (element == QStyle::PE_IndicatorItemViewItemDrop)
    {
        QRect rect = option->rect.adjusted(0, 1, 0, 1);

        if (rect.width() == 0) return;

        painter->setPen(QColor(200,0,100));
        painter->setBrush(QColor(200,0,100));

        QPoint points[4] = {
            QPoint(-3,0),
            QPoint(0,-3),
            QPoint(3,0),
            QPoint(0,3),
        };

        painter->translate(rect.topLeft());
        painter->drawConvexPolygon(points, 4);
        painter->drawLine(QPoint(0,0), QPoint(rect.width(),0));
        painter->translate(QPoint(rect.width()-1,0));
        painter->drawConvexPolygon(points, 4);
    }
    else
    {
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}
