#ifndef MENUBAR_H
#define MENUBAR_H

#include <QAction>
#include <QToolBar>
#include "../ext/qwidgetblinker.h"

#include "../main.h"
#include "../controller.h"


// cyclic reference
class MainWindow;

/**
 * @brief Menubar of the main window
 */
class MenuBar : public QToolBar
{
    Q_OBJECT

private:
    Controller* m_ctrl;

    QAction* m_actionLock;
    QAction* m_actionUnlock;
    QAction* m_actionPause;
    QWidgetBlinker* m_pauseBlinker;

public:
    MenuBar(MainWindow* _parent, Controller* _ctrl);

private:
    QAction* addMenu(const QIcon &_icon, const QString &_text, QMenu* _menu);

private slots:
    void setStartPause(bool _start);
    void setLocked(bool _locked);
    void setLockEnabled(bool _lockEnabled);
};

#endif // MENUBAR_H
