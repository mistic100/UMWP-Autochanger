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

private:
    QList<int>  m_aiSets;
    bool        m_bStartPause;
    bool        m_bRefresh;
    bool        m_bShowHide;

public:
    GlobalShortcut(QObject* _parent=0) : QxtGlobalShortcut(_parent)
    {
        m_bStartPause = m_bRefresh = m_bShowHide = false;
    }

    void vSetSets(const QList<int> _ai) { m_aiSets =_ai; }
    void vSetStartPause()               { m_bStartPause=true; }
    void vSetRefresh()                  { m_bRefresh=true; }
    void vSetShowHide()                 { m_bShowHide=true; }

    const QList<int> sets() const { return m_aiSets; }
    const bool startPause() const { return m_bStartPause; }
    const bool refresh() const    { return m_bRefresh; }
    const bool showHide() const   { return m_bShowHide; }
};

#endif // QTGLOBALSHORTCUT_H
