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
    int m_iHotkey;

public:
    QLineEditHotkey(QWidget* parent = 0);

    const int hotkey() const { return m_iHotkey; }

    void vSetHotkey(const int &_iHotkey);

protected:
    void keyPressEvent(QKeyEvent* _event);
    
};

#endif // QLINEEDITHOTKEY_H
