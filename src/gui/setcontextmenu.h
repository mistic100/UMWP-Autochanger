#ifndef SETCONTEXTMENU_H
#define SETCONTEXTMENU_H

#include <QMenu>

#include "../main.h"
#include "../controller.h"

/**
 * @brief Custom context menu for main list
 */
class SetContextMenu : public QMenu
{
    Q_OBJECT

private:
    Controller* m_ctrl;
    Settings* m_settings;

    QList<Set*> m_sets;

public:
    SetContextMenu(QWidget* _parent, Controller* _ctrl, const QList<Set*> &_sets);

};

#endif // SETCONTEXTMENU_H
