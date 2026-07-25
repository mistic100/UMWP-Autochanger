#ifndef GLOBALSHORTCUT_H
#define GLOBALSHORTCUT_H

#include "qhotkey.h"

/**
 * @brief QHotkey with custom attributes
 */
class GlobalShortcut : public QHotkey
{
    Q_OBJECT

public:
    enum Type {
        NOOP,
        SETS,
        STARTPAUSE,
        REFRESH,
        SHOWHIDE,
        DELAY,
        LOCKUNLOCK
    };

private:
    QList<int> m_sets;
    GlobalShortcut::Type m_type;

public:
    GlobalShortcut(const QKeySequence &_shortcut, QObject* _parent = nullptr) : QHotkey(_shortcut, true, _parent), m_type(NOOP) {}

    void setSets(const QList<int> &_ai)             { m_sets =_ai; }
    void setType(const GlobalShortcut::Type _type)  { m_type = _type; }

    const QList<int> sets() const       { return m_sets; }
    GlobalShortcut::Type type() const   { return m_type; }
};

#endif // GLOBALSHORTCUT_H
