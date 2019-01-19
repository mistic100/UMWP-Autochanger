#include "directoryscanner.h"
#include "controller.h"

static const QStringList FILES_FILTER = QStringList()<<"*.jpeg"<<"*.jpg"<<"*.bmp"<<"*.png"<<"*.gif";


/**
 * @brief DirectoryScanner::DirectoryScanner
 * @param Controller* _ctrl
 */
DirectoryScanner::DirectoryScanner(Controller* _ctrl) :
    QObject(_ctrl)
{

}

/**
 * @brief Rescan a directory if needed a writes the cache file
 * @param Set* _set
 * @return bool
 */
bool DirectoryScanner::scan(Set *_set)
{
    QTime timer;
    timer.start();

    double lastModif = lastChange(_set->path());

    if (lastModif <= _set->lastModif())
    {
        return false;
    }

    QLOG_DEBUG() << _set->uuid()<<"has been modified, updating files list";

    QVector<QString> files;
    QVector<QString> folders;

    filesList(_set->path(), &files);
    foldersList(_set->path(), &folders);

    _set->update(lastModif, files, folders);

    QLOG_INFO() << "Scanned"<<_set->uuid()<<"in"<<timer.elapsed()<<"milliseconds";

    return true;
}

/**
 * @brief Recursively read the last modification date of a folder
 * @param string _path
 * @param int _level
 * @return double
 */
double DirectoryScanner::lastChange(const QString &_path, const int _level)
{
    double date = QFileInfo(_path).lastModified().toTime_t();

    if (_level < APP_MAX_TRAVERSAL)
    {
        QDirIterator dir(_path, QDir::AllDirs | QDir::NoDotAndDotDot);

        while (dir.hasNext())
        {
            QString path = dir.next();

            double subDate = lastChange(path, _level+1);

            if (subDate > date)
            {
                date = subDate;
            }
        }
    }

    return date;
}

/**
 * @brief Recursively construct the list of files
 * @param string _path
 * @param string[]* _files
 * @param int _level
 */
void DirectoryScanner::filesList(const QString &_path, QVector<QString>* _files, const int _level)
{
    if (_level < APP_MAX_TRAVERSAL)
    {
        QDir dir(_path);
        QStringList files = dir.entryList(FILES_FILTER,
                                          QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot,
                                          QDir::Name | QDir::DirsLast | QDir::IgnoreCase
                                          );

        foreach (const QString path, files)
        {
            if (QFileInfo(_path + path).isDir())
            {
                filesList(_path + path + QDir::separator(), _files, _level+1);
            }
            else
            {
                _files->append(_path + path);
            }
        }
    }
}

/**
 * @brief Construct the list of folders
 * @param string _path
 * @param string[]* _folders
 */
void DirectoryScanner::foldersList(const QString &_path, QVector<QString>* _folders)
{
    QDir dir(_path);
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);

    foreach (const QString path, folders)
    {
        if (QDir(_path + path).entryList(FILES_FILTER, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot).count() > 0)
        {
            _folders->append(_path + path);
        }
    }
}
