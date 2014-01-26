#include "qlineedithotkey.h"


/**
 * @brief QLineEditHotkey::QLineEditHotkey
 * @param QWidget* _parent
 */
QLineEditHotkey::QLineEditHotkey(QWidget* _parent) : QLineEdit(_parent) {}

/**
 * @brief QLineEditHotkey::keyPressEvent
 * @param QKeyEvent* _event
 */
void QLineEditHotkey::keyPressEvent(QKeyEvent* _event)
{
    int key = _event->key();
    Qt::KeyboardModifiers modifiers = _event->modifiers();

    // erase content if escape of return pressed
    if (key == Qt::Key_Escape || key == Qt::Key_Backspace)
    {
        m_hotkey = 0;
        setText(NULL);
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

    m_hotkey = key + mod;

    setText(QKeySequence(m_hotkey).toString(QKeySequence::NativeText));

    emit textChanged(text());
}

/**
 * @brief QLineEditHotkey::vSetHotkey
 * @param int _hotkey
 */
void QLineEditHotkey::setHotkey(const int &_hotkey)
{
    m_hotkey = _hotkey;

    setText(QKeySequence(m_hotkey).toString(QKeySequence::NativeText));
}
