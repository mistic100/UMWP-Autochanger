#ifndef WALLPAPERGENERATOR_H
#define WALLPAPERGENERATOR_H

#include <random>

#include "main.h"
#include "environment.h"
#include "settings.h"
#include "customlayoutgenerator.h"


class Controller;

/**
 * @brief Utility to select and process wallpaper files
 */
class WallpaperGenerator : public QObject
{
    Q_OBJECT

public:
    struct Result {
        QString wallpaper;
        QVector<QString> files;
    };

private:
    Environment*    m_enviro;
    Settings*       m_settings;
    CustomLayoutGenerator* m_custGenerator;
    std::mt19937    m_randomEngine; // mersenne_twister

public:
    WallpaperGenerator(Controller* _ctrl);
    ~WallpaperGenerator();

    Result generate(Set* _set);
    Set* getRandomSet();

private:
    QVector<QString> getFiles(Set* _set);
    QVector<QString> getFiles(Set* _set, int _nb);
    QVector<QString> adaptFiles(Set* _set, const QVector<QString> &_files);
    QVector<QString> getCustomFiles(Set* _set);
    QString generateFile(Set* _set, const QVector<QString> &_files);
    QString getNextFile(Set* _set);
    QString getRandomFile(Set* _set, const QVector<QString> &_files);
    QString adaptFileToMonitor(const QString &_file, int _idx, const QRect &_scrRect, const QRect &_wpRect, Set* _set);
    QString generateCustomFile(const QRect &_scrRect, Set* _set);
    QRect   getDesktopEnabledRect();
    QString getDesktopWallpaperKey(UM::IMAGE _style);
    QString getCacheFilename(const QString &_file, const QRect &_rect, const QString &_key1, const QString &_key2);
    QString getCustLayoutTempFilename(const QRect &_rect, Set* _set);

};

#endif // WALLPAPERGENERATOR_H
