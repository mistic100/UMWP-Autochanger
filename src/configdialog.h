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
    
public:
    ConfigDialog(QWidget* _parent, Settings* _poSettings);
    ~ConfigDialog();

    void vSave();
    const bool bDelayChanged() const { return m_bDelayChanged; }

protected:
    void done(int result);

private slots:
    void on_optionDelay_valueChanged();
    void on_optionUseHotkeys_toggled(bool checked);
};

#endif // CONFIGDIALOG_H
