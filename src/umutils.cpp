#include "umutils.h"

/**
 * @brief Move a file to trash bin
 * @param string _filename
 * @return bool
 */
bool UM::moveFileToTrash(const QString &_filename)
{
    wchar_t path[MAX_PATH];
    memset(path, 0, sizeof(path));
    int l = _filename.toWCharArray(path);
    path[l] = '\0';

    SHFILEOPSTRUCT shfos = {0};
    shfos.wFunc  = FO_DELETE;
    shfos.pFrom  = path;
    shfos.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

    int ret = SHFileOperation(&shfos);

    return ret == 0;
}

/*
 * helper for simple XML text nodes
 */
void UM::addSimpleTextNode(QDomDocument* _dom, QDomNode* _parent, const QString &_name, const QString &_value)
{
    QDomElement element = _dom->createElement(_name);
    element.appendChild(_dom->createTextNode(_value));
    _parent->appendChild(element);
}

/**
 * @brief Return a scaled copy of a rectangle
 * @param QRect _rect
 * @param float _ratio
 * @return QRect
 */
QRect UM::scaledRect(const QRect &_rect, float _xRatio, float _yRatio)
{
    return QRect(
                _rect.left() * _xRatio,
                _rect.top() * _yRatio,
                _rect.width() * _xRatio,
                _rect.height() * _yRatio
    );
}
