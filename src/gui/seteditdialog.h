#ifndef SETEDITDIALOG_H
#define SETEDITDIALOG_H

#include <QDialog>

#include "main.h"
#include "set.h"
#include "settings.h"

namespace Ui {
    class SetEditDialog;
}


/**
 * @brief Dialog for Set edition
 */
class SetEditDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::SetEditDialog *ui;
    Settings* m_settings;

public:
    SetEditDialog(QWidget* _parent, Set* _set, Settings* _settings);
    ~SetEditDialog();

    void save(int index);

protected:
    void done(int result);
};

#endif // SETEDITDIALOG_H