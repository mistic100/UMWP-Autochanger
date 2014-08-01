#ifndef QLINEEDITHOTKEY_H
#define QLINEEDITHOTKEY_H

#include <QLineEdit>
#include <QKeyEvent>


/**
 * @brief Custom QLineEdit to handle hotkeys inputs
 */
class QLineEditHotkey : public QLineEdit
{
    Q_OBJECT

public:
    QLineEditHotkey(QWidget* _parent = 0);

signals:
    void hotkeyChanged(int);

protected:
    void keyPressEvent(QKeyEvent* _event);
    
};

#endif // QLINEEDITHOTKEY_H
