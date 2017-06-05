#ifndef SETCONTEXTMENU_H
#define SETCONTEXTMENU_H

#include <QMenu>

#include "../main.h"
#include "../set.h"

class MainWindow;

/**
 * @brief Custom context menu for main list
 */
class SetContextMenu : public QMenu
{
    Q_OBJECT

private:
    MainWindow* m_parent;

    QList<Set*> m_sets;

public:
    SetContextMenu(MainWindow *_parent, const QList<Set*> &_sets);

};

#endif // SETCONTEXTMENU_H
