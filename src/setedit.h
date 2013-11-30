#ifndef SETEDIT_H
#define SETEDIT_H

#include <QDialog>

#include "main.h"
#include "set.h"

namespace Ui {
    class SetEdit;
}


/**
 * @brief Dialog for Set edition
 */
class SetEdit : public QDialog
{
    Q_OBJECT

private:
    Ui::SetEdit *ui;

public:
    SetEdit(QWidget* _parent, Set* _poSet);
    ~SetEdit();

    const QString name() const;
    const int type() const;
    const int style() const;
};

#endif // SETEDIT_H
