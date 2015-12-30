#ifndef GLOBALSHORTCUT_H
#define GLOBALSHORTCUT_H

#include "ext/qglobalshortcut.h"


/**
 * @brief QGlobalShortcut with custom attributes
 */
class GlobalShortcut : public QGlobalShortcut
{
    Q_OBJECT

public:
    enum Type {
        NOOP,
        SETS,
        STARTPAUSE,
        REFRESH,
        SHOWHIDE
    };

private:
    QList<int> m_sets;
    GlobalShortcut::Type m_type;

public:
    GlobalShortcut(QObject* _parent = 0) : QGlobalShortcut(_parent), m_type(NOOP) {}

    void setSets(const QList<int> &_ai) { m_sets =_ai; }
    void setType(const GlobalShortcut::Type _type) { m_type = _type; }

    const QList<int> sets() const { return m_sets; }
    const GlobalShortcut::Type type() const { return m_type; }
};

#endif // GLOBALSHORTCUT_H
