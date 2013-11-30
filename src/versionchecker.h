#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QObject>

#include "main.h"


/**
 * @brief Used to download version file and compare to current version
 */
class VersionChecker : public QObject
{
    Q_OBJECT

public:
    VersionChecker(QObject* _parent);

public slots:
    void run();
    
signals:
    void newVersionAvailable(const QString);
    void finished();
    
};

#endif // VERSIONCHECKER_H
