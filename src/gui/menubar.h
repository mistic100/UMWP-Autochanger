#ifndef MENUBAR_H
#define MENUBAR_H

#include <QtWidgets/QAction>
#include "../ext/qtoolbarext.h"
#include "../ext/qwidgetblinker.h"

#include "../main.h"
#include "../controller.h"


// cyclic reference
class MainWindow;

/**
 * @brief Menubar of the main window
 */
class MenuBar : public QToolBarExt
{
    Q_OBJECT

private:
    QToolButton* m_actionPause;
    QWidgetBlinker* m_pauseBlinker;

public:
    MenuBar(MainWindow* _parent, Controller* _ctrl);

private slots:
    void setStartPause(bool _start);
};

#endif // MENUBAR_H
