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
    QVector<QString> getFiles(Set* _set);
    QVector<QString> adaptFiles(const Set* _set, const QVector<QString> &_files);
    QString generateFile(const Set* _set, const QVector<QString> &_files);

private:
    QString getNextFile(Set* _set);
    QString getRandomFile(Set* _set, const QVector<QString> &_files);
    QString adaptFileToMonitor(const QString &_file, int _idx, const QRect &_scrRect, const QRect &_wpRect, const Set* _set);
    QRect   getDesktopEnabledRect();
    QString getDesktopWallpaperKey(UM::IMAGE _style);
    QString getCacheFilename(const QString &_file, const QRect &_rect, const QString &_key1, const QString &_key2);

};

#endif // WALLPAPERGENERATOR_H
