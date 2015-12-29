#ifndef NEWVERSIONDIALOG_H
#define NEWVERSIONDIALOG_H

#include <QtWidgets/QDialog>
#include <QFile>
#include <QNetworkReply>

#include "../main.h"
#include "../controller.h"

namespace Ui {
    class NewVersionDialog;
}


/**
 * @brief Dialog to download new installer
 */
class NewVersionDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::NewVersionDialog* ui;
    Controller* m_ctrl;

    UM::NewVersion  m_version;

    QFile           m_file;
    QNetworkReply*  m_reply;
    
public:
    NewVersionDialog(QWidget* _parent, Controller* _ctrl);
    ~NewVersionDialog();

private:
    void errorMessage();

private slots:
    void on_updateButton_clicked();

    void onDataReady();
    void onDownloadFinished();
    void onDownloadProgress(qint64 _received, qint64 _total);
};

#endif // NEWVERSIONDIALOG_H
