#ifndef SETEDITDIALOG_H
#define SETEDITDIALOG_H

#include <QDialog>

#include "../main.h"
#include "../set.h"
#include "../customlayout.h"
#include "../controller.h"
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

    Controller* m_ctrl;
    Settings* m_settings;

    QList<Set*> m_sets;
    CustomLayout m_custLayout;

public:
    SetEditDialog(QWidget* _parent, Controller* _ctrl, const QList<Set*> &_sets);
    ~SetEditDialog();

    const Set result();

protected:
    void done(int result);

private slots:
    void on_selectStyle_currentDataChanged(QVariant data);
    void on_styleConfig_clicked();
    void on_freqHelp_clicked();
};

#endif // SETEDITDIALOG_H
