#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

#include "main.h"
#include "settings.h"

namespace Ui {
    class ConfigDialog;
}


/**
 * @brief Dialog for configuration parameters
 */
class ConfigDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::ConfigDialog*   ui;
    Settings*           m_poSettings;
    bool                m_bDelayChanged;
    bool                m_bHotkeysChanged;
    
public:
    ConfigDialog(QWidget* _parent, Settings* _poSettings);
    ~ConfigDialog();

    void vSave();
    const bool bDelayChanged() const    { return m_bDelayChanged; }
    const bool bHotkeysChanged() const  { return m_bHotkeysChanged; }

private slots:
    void hotkeysChanged();
    void on_optionDelay_valueChanged();
};

#endif // CONFIGDIALOG_H
