#ifndef SETEDITDIALOG_H
#define SETEDITDIALOG_H

#include <QtWidgets/QDialog>

#include "../main.h"
#include "../set.h"
#include "../settings.h"

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
    QList<int> m_sets;

public:
    SetEditDialog(QWidget* _parent, Settings* _settings, const QList<int> &_sets);
    ~SetEditDialog();

    void save();

protected:
    void done(int result);
};

#endif // SETEDITDIALOG_H
