#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>
#include <QGridLayout>

#include "../main.h"
#include "../controller.h"
#include "../settings.h"


/**
 * @brief Dialog used to display current files as thumbnails
 */
class PreviewDialog : public QDialog
{
    Q_OBJECT

private:
    Controller* m_ctrl;
    Settings*   m_settings;

    QGridLayout* m_layout;
    
public:
    PreviewDialog(QWidget* _parent, Controller* _ctrl);

public slots:
    void draw();

private slots:
    void onThumbnailClicked();
    void onDeleteButtonClicked();
};

#endif // PREVIEWDIALOG_H
