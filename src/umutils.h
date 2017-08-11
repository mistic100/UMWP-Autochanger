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

    QString hash(const QString &_input, QCryptographicHash::Algorithm _algorythm);

    int keySequenceToInt(const QKeySequence &_seq);
}

#endif // UMUTILS_H
