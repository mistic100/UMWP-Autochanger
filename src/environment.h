#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QHash>

#include "settings.h"


/**
 * @brief Store data about new version
 */
struct NewVersion {
    QString code;
    QString link;
    QString hash;
};

Q_DECLARE_METATYPE(NewVersion)


/**
 * @brief Holds environment state
 */
class Environment
{
private:
    Settings*                m_settings;

    QString                  m_shortcutPath;
    QList<QString>           m_languages;  // languages packaged with UMWPA
    QHash<int, QRect>        m_wpSizes;    // monitors sizes
    NewVersion               m_newVersion; // struct to hold new version data

public:
    Environment(Settings* _settings);

    // main methods
    void log();
    bool refreshMonitors();
    void checkSettings();
    void setWallpaper(const QString &_file);

    // getters
    const QString       shortcutPath() const            { return m_shortcutPath; }
    const int           nbMonitors() const              { return m_wpSizes.size()-1; }
    const QRect         wpSize(int _i) const            { return m_wpSizes.value(_i); }
    const NewVersion    newVersion() const              { return m_newVersion; }
    const QList<QString> &languages() const             { return m_languages; }

    // setters
    void setNewVersion(const NewVersion &_version)      { m_newVersion = _version; }

    // shortcut
    const bool isAutostart() const;
    const bool canAddShortcut() const;
    void createShortcut();
    void deleteShortcut();

private:
    bool queryMonitors(QHash<int, QRect> &_sizes);
};

#endif // ENVIRONMENT_H
