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

private:
    int m_hotkey;

public:
    QLineEditHotkey(QWidget* _parent = 0);

    const int hotkey() const { return m_hotkey; }

    void setHotkey(const int &_hotkey);

protected:
    void keyPressEvent(QKeyEvent* _event);
    
};

#endif // QLINEEDITHOTKEY_H
