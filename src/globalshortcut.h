#ifndef QTGLOBALSHORTCUT_H
#define QTGLOBALSHORTCUT_H

#include <QxtGlobalShortcut>
#include <QHash>
#include <QVariant>


/**
 * @brief QxtGlobalShortcut with custom attributes
 */
class GlobalShortcut : public QxtGlobalShortcut
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
    QList<int>  m_aiSets;
    GlobalShortcut::Type m_type;

public:
    GlobalShortcut(QObject* _parent=0) : QxtGlobalShortcut(_parent)
    {
        m_type = NOOP;
    }

    void vSetSets(const QList<int> _ai) { m_aiSets =_ai; m_type = SETS; }
    void vSetType(GlobalShortcut::Type _type) { m_type = _type; }

    const QList<int> sets() const { return m_aiSets; }
    const GlobalShortcut::Type type() const { return m_type; }
};

#endif // QTGLOBALSHORTCUT_H
