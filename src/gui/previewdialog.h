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

    bool m_needsUpdate = false;

    QGridLayout* m_layout;
    
public:
    PreviewDialog(QWidget* _parent, Controller* _ctrl);
    ~PreviewDialog();

private:
    QString getFolderImage(const QString &_folder);

public slots:
    void draw();

    void onThumbnailClicked(const QString &_path);
    int onDeleteButtonClicked(const QString &_path);
    void onEditButtonClicked(const QString &_path);
    void onOpenButtonClicked(const QString &_path);
};

#endif // PREVIEWDIALOG_H
