#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "main.h"


/**
 * @brief Holds environment state
 */
class Environment
{
public:
    static QString APPDATA_DIR;

private:
    QString m_shortcutPath;
    QList<QString> m_languages; // languages packaged with UMWPA
    QHash<int, QRect> m_wpSizes; // monitors sizes
    UM::NewVersion m_newVersion; // struct to hold new version data
    bool m_isPortable = true;

public:
    Environment();

    // main methods
    void log();
    void refreshMonitors();
    void setWallpaper(const QString &_file);

    // getters
    const QString shortcutPath() const { return m_shortcutPath; }
    const int nbMonitors() const { return qMax(0, m_wpSizes.size()-1); }
    const QRect wpSize(int _i) const { return m_wpSizes.value(_i); }
    const UM::NewVersion newVersion() const { return m_newVersion; }
    const QList<QString> &languages() const { return m_languages; }
    const bool isPortable() const { return m_isPortable; }
    const QString getAppDataFolder() const;

    // setters
    void setNewVersion(const UM::NewVersion &_version) { m_newVersion = _version; }

    // shortcut
    const bool isAutostart() const;
    const bool canAddShortcut() const;
    void createShortcut();
    void deleteShortcut();
};

#endif // ENVIRONMENT_H
