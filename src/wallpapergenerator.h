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
        QVector<Set*> sets;
        QVector<QString> files;
        UM::WALLPAPER type;
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
    QRect getDesktopEnabledRect();
    QString getFile(int _idx, Result* _context);

    // random access
    Set* getRandomSet(const int _monitor=-1);
    QString getRandomFolder(Set* _set);
    QString getRandomFile(Set* _set, const QString &_folder="", const QVector<QString> &_files=QVector<QString>());
    QString getNextFile(Set* _set);

    // custom generation
    QString generateCustomFile(int _idx, Result* _context);
    QVector<QString> getFilesForCustom(Set* _set, int _nb, Result* _context);

    // standard generation
    QString getAndAdaptFile(int _idx, Result* _context);
    QString adaptFileToMonitor(const QString &_file, int _idx, Result* _context);

    // final generation
    QString generateFile(const QVector<QString> &_files, Result* _context);

    // temporary files
    void cleanTempFiles();
    QString getTempFilename(int _idx, Set* _set);
};

#endif // WALLPAPERGENERATOR_H
