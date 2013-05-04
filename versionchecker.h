#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>

#include "main.h"


/*
 * Name: VersionChecker
 * Description: Used to download version file and compare to current version
 */
class VersionChecker : public QObject
{
    Q_OBJECT

public:
    VersionChecker(QObject* _parent);

public slots:
    void doCheckVersion();
    
signals:
    void newVersionAvailable(QString);
    
};

#endif // PROCESS_H
