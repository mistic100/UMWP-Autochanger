#include <QCryptographicHash>
#include <QKeySequence>

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

/**
 * @brief Return a scaled copy of a rectangle
 * @param QRect _rect
 * @param float _ratio
 * @return QRect
 */
QRect UM::scaledRect(const QRect &_rect, float _xRatio, float _yRatio)
{
    return QRect(
                qRound(_rect.left() * _xRatio),
                qRound(_rect.top() * _yRatio),
                qRound(_rect.width() * _xRatio),
                qRound(_rect.height() * _yRatio)
    );
}

/**
 * @brief Hash a string
 * @param string _value
 * @param int _algorythm
 * @return string
 */
QString UM::hash(const QString &_value, QCryptographicHash::Algorithm _algorythm)
{
    return QString(QCryptographicHash::hash(_value.toUtf8(), _algorythm).toHex());
}

/**
 * @brief Converts a key sequence to it's int code
 * @param _seq
 * @return int
 */
int UM::keySequenceToInt(const QKeySequence &_seq)
{
    int code = 0;

    for (int i = 0; i < _seq.count(); i++)
    {
        code+= _seq[i];
    }

    return code;
}
