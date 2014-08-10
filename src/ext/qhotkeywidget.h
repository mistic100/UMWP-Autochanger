#ifndef QHOTKEYWIDGET_H
#define QHOTKEYWIDGET_H

#include <QWidget>

#include "ui_qhotkeywidget.h"

namespace Ui {
    class QHotKeyWidget;
}


/**
 * @brief Widget combining a QLineEditHotKey and a clear button
 */
class QHotKeyWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::QHotKeyWidget* ui;

    int m_hotkey;
    
public:
    QHotKeyWidget(QWidget* _parent = 0) :
        QWidget(_parent),
        ui(new Ui::QHotKeyWidget)
    {
        ui->setupUi(this);
    }

    ~QHotKeyWidget()
    {
        delete ui;
    }

    const int hotkey() const
    {
        return m_hotkey;
    }

    void setHotkey(const int _hotkey)
    {
        m_hotkey = _hotkey;
        ui->key->setText(QKeySequence(m_hotkey).toString(QKeySequence::NativeText));
    }

private slots:
    void on_key_hotkeyChanged(const int _hotkey)
    {
        setHotkey(_hotkey);
    }

    void on_pushButton_clicked()
    {
        setHotkey(0);
    }
};

#endif // QHOTKEYWIDGET_H
