#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

#include "../main.h"
#include "../controller.h"
#include "../ext/qbuttongroupext.h"

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
    Ui::ConfigDialog* ui;
    QButtonGroupExt* ui_lockType;

    Controller* m_ctrl;
    Settings* m_settings;
    Environment* m_enviro;
    
public:
    ConfigDialog(QWidget* _parent, Controller* _ctrl);
    ~ConfigDialog();

    void save();

protected:
    void done(int result);

private slots:
    void on_optionUseHotkeys_toggled(bool _checked);
    void on_optionLockEnabled_toggled(bool _checked);
    void on_lockType_toggled(int _id, bool _checked);
    void on_optionOpenProgram_currentIndexChanged(int);
    void on_lockHelp_clicked();
};

#endif // CONFIGDIALOG_H
