#ifndef LISTPROXYSTYLE_H
#define LISTPROXYSTYLE_H

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOption>

#include "main.h"


/**
 * @brief A custom display of QListWidget drop indicator
 */
class ListProxyStyle : public QProxyStyle
{
public:
    ListProxyStyle();

    void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
};

#endif // LISTPROXYSTYLE_H
