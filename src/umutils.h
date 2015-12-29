#ifndef UMUTILS_H
#define UMUTILS_H

#include <QtXml>

#include "main.h"


namespace UM
{
    bool moveFileToTrash(const QString &_filename);

    void addSimpleTextNode(QDomDocument* _dom, QDomNode* _parent, const QString &_name, const QString &_value);
}

#endif // UMUTILS_H
