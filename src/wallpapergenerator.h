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
    /**
     * @brief The result of the async generation
     */
    struct Result {
        Set* set;
        QVector<QString> files;
    };

private:
    Environment* m_enviro;
    Settings* m_settings;
    CustomLayoutGenerator* m_custGenerator;

    std::mt19937 m_randomEngine; // mersenne_twister

public:
    WallpaperGenerator(Controller* _ctrl);
    ~WallpaperGenerator();

    Result generate();

private:
    Set* getRandomSet();

    QRect getDesktopEnabledRect();

    // source files getters
    QVector<QString> getFiles(Set* _set);
    QVector<QString> getFiles(Set* _set, int _nb);
    QString getNextFile(Set* _set);
    QString getRandomFile(Set* _set, const QVector<QString> &_files);

    // custom generation
    QVector<QString> getCustomFiles(Set* _set, QVector<QString> &_srcFiles);
    QString generateCustomFile(int _idx, Set* _set, QVector<QString> &_srcFiles);

    // standard generation
    QString adaptFileToMonitor(const QString &_file, int _idx, Set* _set);
    QVector<QString> adaptFiles(Set* _set, const QVector<QString> &_files);

    // final generation
    QString generateFile(Set* _set, const QVector<QString> &_files);

    // cache keys
    QString getDesktopWallpaperKey(UM::IMAGE _style);
    QString getCacheFilename(const QString &_file, const QRect &_rect, const QString &_key1, const QString &_key2);
    QString getCustLayoutTempFilename(int _idx, Set* _set);
};

#endif // WALLPAPERGENERATOR_H
