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
};

Q_DECLARE_METATYPE(NewVersion)


/**
 * @brief Holds environment state
 */
class Environment
{
private:
    Settings*                m_settings;

    QHash<QString, QVariant> m_env;        // environnement variables
    QList<QString>           m_languages;  // languages packaged with UMWPA
    QHash<int, QRect>        m_wpSizes;    // monitors sizes
    QByteArray               m_header;     // .wallpaper file header
    NewVersion               m_newVersion; // struct to hold new version data

public:
    Environment(Settings* _settings);

    // main methods
    void init();
    void log();
    bool refreshMonitors();
    void checkSettings();

    // getters
    const QVariant      get(const QString &_key) const  { return m_env.value(_key); }
    const int           nbMonitors() const              { return get("nb_monitors").toInt(); }
    const QRect         wpSize(int _i) const            { return m_wpSizes.value(_i); }
    const QByteArray    &header() const                 { return m_header; }
    const NewVersion    newVersion() const              { return m_newVersion; }
    const QList<QString> &languages() const             { return m_languages; }

    // setters
    void setNewVersion(const NewVersion &_version)      { m_newVersion = _version; }

    // shortcut
    const bool isAutostart() const;
    const bool canAddShortcut() const;
    void createShortcut();
    void deleteShortcut();
};

#endif // ENVIRONMENT_H
