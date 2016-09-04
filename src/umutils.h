#ifndef UMUTILS_H
#define UMUTILS_H

#include "main.h"


/**
 * @brief Collection of utilities
 */
namespace UM
{
    bool moveFileToTrash(const QString &_filename);

    QRect scaledRect(const QRect &_rect, float _xRatio, float _yRatio);

    QList<QLine> rectBorders(const QRect &_rect);

    QString hash(const QString &_input, QCryptographicHash::Algorithm _algorythm);
}

#endif // UMUTILS_H
