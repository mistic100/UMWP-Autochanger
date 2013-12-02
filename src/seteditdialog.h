#ifndef SETEDITDIALOG_H
#define SETEDITDIALOG_H

#include <QDialog>

#include "main.h"
#include "set.h"

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

public:
    SetEditDialog(QWidget* _parent, Set* _poSet);
    ~SetEditDialog();

    const QString name() const;
    const UM::WALLPAPER type() const;
    const UM::IMAGE style() const;
};

#endif // SETEDITDIALOG_H
