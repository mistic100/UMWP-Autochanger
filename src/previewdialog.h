#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>

#include "main.h"
#include "controller.h"

namespace Ui {
    class PreviewDialog;
}


/**
 * @brief Little dialog used to display current files as thumbnails
 */
class PreviewDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::PreviewDialog *ui;
    
public:
    PreviewDialog(QWidget* _parent, Controller* _ctrl);
    ~PreviewDialog();

public slots:
    void slotImageClicked();
};

#endif // PREVIEWDIALOG_H
