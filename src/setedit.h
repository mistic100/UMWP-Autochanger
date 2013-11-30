#ifndef SETEDIT_H
#define SETEDIT_H

#include <QDialog>
#include "main.h"

namespace Ui {
    class SetEdit;
}

class SetEdit : public QDialog
{
    Q_OBJECT
    
public:
    explicit SetEdit(QString const &_sName, const int _iType, const int _iStyle, QWidget* parent=0);
    ~SetEdit();

    QString const name() const;
    int const type() const;
    int const style() const;
    
private:
    Ui::SetEdit *ui;
};

#endif // SETEDIT_H
