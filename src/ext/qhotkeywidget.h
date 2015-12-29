#ifndef QHOTKEYWIDGET_H
#define QHOTKEYWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

#include "qlineedithotkey.h"


/**
 * @brief Widget combining a QLineEditHotKey and a clear button
 */
class QHotKeyWidget : public QWidget
{
    Q_OBJECT

public:
    static const int KEEP_KEY = -1;
    static const int NO_KEY = 0;

private:
    int m_hotkey;
    QLineEditHotkey* ui_key;
    
public:
    QHotKeyWidget(QWidget* _parent = 0) :
        QWidget(_parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setMargin(0);
        layout->setSpacing(0);
        setLayout(layout);

        ui_key = new QLineEditHotkey(this);
        layout->addWidget(ui_key);

        QPushButton* erase = new QPushButton(this);
        erase->setFlat(true);
        erase->setIcon(QIcon("://images/icons/bullet_cross.png"));
        erase->setToolTip(tr("Reset"));
        layout->addWidget(erase);

        connect(ui_key, SIGNAL(hotkeyChanged(int)), this, SLOT(onHotkeyChanged(int)));
        connect(erase, SIGNAL(clicked()), this, SLOT(onEraseClicked()));

        setHotkey(NO_KEY);
    }

    const int hotkey() const
    {
        return m_hotkey;
    }

    void setHotkey(const int _hotkey)
    {
        m_hotkey = _hotkey;

        if (m_hotkey == KEEP_KEY)
        {
            ui_key->setText(tr("[keep]"));
        }
        else
        {
            ui_key->setText(QKeySequence(m_hotkey).toString(QKeySequence::NativeText));
        }
    }

private slots:
    void onHotkeyChanged(const int _hotkey)
    {
        setHotkey(_hotkey);
    }

    void onEraseClicked()
    {
        setHotkey(0);
    }
};

#endif // QHOTKEYWIDGET_H
