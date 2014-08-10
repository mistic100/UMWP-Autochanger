#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QHash>

#include "ext/qscreen.h"
#include "settings.h"


/**
 * @brief Holds environment state
 */
class Environment
{
private:
    Settings*                m_settings;

    QHash<QString, QVariant> m_env;     // environnement variables
    QHash<int, QScreen>      m_wpSizes;
    QByteArray               m_header;
    QPair<QString, QString>  m_newVersion;

public:
    Environment(Settings* _settings);

    // main methods
    void init();
    void log();
    bool refreshMonitors();

    // getters
    const QVariant      get(const QString &_key) const  { return m_env.value(_key); }

    const QScreen       wpSize(int _i) const            { return m_wpSizes.value(_i); }
    const QByteArray    &header() const                 { return m_header; }
    const QPair<QString, QString> newVersion()          { return m_newVersion; }

    // setters
    void setNewVersion(const QString &_ver, const QString &_link);

    // shortcut
    const bool isAutostart() const;
    const bool canAddShortcut() const;
    void createShortcut();
    void deleteShortcut();
};

#endif // ENVIRONMENT_H
