#ifndef UMUTILS_H
#define UMUTILS_H

#include "main.h"


/**
 * @brief Collection of utilities
 */
namespace UM
{
    bool moveFileToTrash(const QString &_filename);

    void addSimpleTextNode(QDomDocument* _dom, QDomNode* _parent, const QString &_name, const QString &_value);

    QRect scaledRect(const QRect &_rect, float _xRatio, float _yRatio);

    QList<QLine> rectBorders(const QRect &_rect);
}

#endif // UMUTILS_H
