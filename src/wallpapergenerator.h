#ifndef WALLPAPERGENERATOR_H
#define WALLPAPERGENERATOR_H

#include <random>

#include "main.h"
#include "environment.h"
#include "settings.h"


using namespace std::tr1;

class Controller;


/**
 * @brief Utility to select and process wallpaper files
 */
class WallpaperGenerator : public QObject
{
    Q_OBJECT

private:
    Controller*     m_ctrl;
    Environment*    m_enviro;
    Settings*       m_settings;

    mt19937         m_randomEngine; // mersenne_twister

public:
    WallpaperGenerator(Controller* _ctrl);

    Set* getRandomSet();
    QVector<QString> getFiles(const Set* _set);
    QVector<QString> adaptFiles(const Set* _set, const QVector<QString> &_files);

private:
    QString getRandomFile(const Set* _set, const QVector<QString> &_files);
    QVector<QString> adaptFileDesktopWithDisabled(const Set* _set, const QVector<QString> &_file);
    QVector<QString> adaptFilesFillMode(const Set* _set, const QVector<QString> &_files);
    QString getConfigurationKey(UM::IMAGE _style);
    QString getCacheFilename(const QString &_originalFile, const QScreen &_rect, const QString &_key);

};

#endif // WALLPAPERGENERATOR_H
