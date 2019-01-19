#ifndef DIRECTORYSCANNER_H
#define DIRECTORYSCANNER_H

#include "main.h"
#include "set.h"


class Controller;

/**
 * @brief Utility to scan sets' directories
 */
class DirectoryScanner : public QObject
{
    Q_OBJECT

public:
    DirectoryScanner(Controller *_ctrl);

    bool scan(Set* _set);

private:
    double lastChange(const QString &_path, const int _level=0);
    void filesList(const QString &_path, QVector<QString>* _files, const int _level=0);
    void foldersList(const QString &_path, QVector<QString>* _folders);
};

#endif // DIRECTORYSCANNER_H
