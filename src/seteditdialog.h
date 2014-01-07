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

    Settings* m_poSettings;

public:
    SetEditDialog(QWidget* _parent, Set* _poSet, Settings* _poSettings);
    ~SetEditDialog();

    const QString       name() const;
    const UM::WALLPAPER type() const;
    const UM::IMAGE     style() const;
    const int           hotkey() const;

protected:
    void done(int result);

};

#endif // SETEDITDIALOG_H
