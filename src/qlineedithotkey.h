#ifndef QLINEEDITHOTKEY_H
#define QLINEEDITHOTKEY_H

#include <QLineEdit>
#include <QKeyEvent>

#include "main.h"


/**
 * @brief Custom QLineEdit to handle hotkeys inputs
 */
class QLineEditHotkey : public QLineEdit
{
    Q_OBJECT

private:
    Hotkey m_oHotkey;

public:
    QLineEditHotkey(QWidget* parent = 0);

    const Hotkey hotkey() const { return m_oHotkey; }

    void vSetHotkey(const Hotkey &_hotkey);

protected:
    void keyPressEvent(QKeyEvent* _event);
    
};

#endif // QLINEEDITHOTKEY_H
