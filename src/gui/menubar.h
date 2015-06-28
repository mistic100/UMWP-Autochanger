#ifndef MENUBAR_H
#define MENUBAR_H

#include <QAction>
#include "ext/qtoolbarext.h"
#include "ext/qwidgetblinker.h"

#include "main.h"
#include "controller.h"


// cyclic reference
class MainWindow;

/**
 * @brief Menubar of the main window
 */
class MenuBar : public QToolBarExt
{
    Q_OBJECT

private:
    Controller*  m_ctrl;

    QToolButton* m_actionPause;
    QAction*     m_actionFiles;

    QWidgetBlinker* m_pauseBlinker;

public:
    MenuBar(MainWindow* _parent, Controller* _ctrl);
    
    void setPause(bool _pause);
};

#endif // MENUBAR_H