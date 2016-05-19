#ifndef DELAYDIALOG_H
#define DELAYDIALOG_H

#include <QDialog>
#include <QKeyEvent>

#include "../main.h"
#include "../settings.h"

namespace Ui {
    class DelayDialog;
}


/**
 * @brief Dialog for delay configuration only
 */
class DelayDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::DelayDialog* ui;

    Settings* m_settings;

public:
    DelayDialog(QWidget* _parent, Settings* _settings);
    ~DelayDialog();

    void save();

protected:
    void done(int result);
};

#endif // DELAYDIALOG_H
