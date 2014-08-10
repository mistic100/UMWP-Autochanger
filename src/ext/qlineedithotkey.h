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
    QLineEditHotkey(QWidget* _parent) : QLineEdit(_parent) {}

signals:
    void hotkeyChanged(int);

protected:
    void keyPressEvent(QKeyEvent* _event)
    {
        int key = _event->key();
        Qt::KeyboardModifiers modifiers = _event->modifiers();

        // erase content if escape of return pressed
        if (key == Qt::Key_Escape || key == Qt::Key_Backspace)
        {
            emit hotkeyChanged(0);
            return;
        }

        // some prohibited keys
        if (key == Qt::Key_Shift || key == Qt::Key_Control || key == Qt::Key_Alt || key == Qt::Key_Meta
            || key == Qt::Key_AltGr || key == Qt::Key_NumLock || key == Qt::Key_Tab
            || modifiers == Qt::KeypadModifier || modifiers.testFlag(Qt::NoModifier))
        {
            return;
        }

        // compute modifiers
        int mod = 0;
        if (modifiers.testFlag(Qt::ShiftModifier))
        {
            mod+= Qt::SHIFT;
        }
        if (modifiers.testFlag(Qt::ControlModifier))
        {
            mod+= Qt::CTRL;
        }
        if (modifiers.testFlag(Qt::AltModifier))
        {
            mod+= Qt::ALT;
        }
        if (modifiers.testFlag(Qt::MetaModifier))
        {
            mod+= Qt::META;
        }

        emit hotkeyChanged(key + mod);
    }
};

#endif // QLINEEDITHOTKEY_H
