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
    Settings*           m_pSettings;
    bool                m_delayChanged;
    
public:
    ConfigDialog(QWidget* _parent, Settings* _pSettings);
    ~ConfigDialog();

    void save();
    const bool delayChanged() const { return m_delayChanged; }

protected:
    void done(int result);

private slots:
    void on_optionDelay_valueChanged();
    void on_optionUseHotkeys_toggled(bool checked);
};

#endif // CONFIGDIALOG_H
