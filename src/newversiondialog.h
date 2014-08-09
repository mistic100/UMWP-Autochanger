#ifndef NEWVERSIONDIALOG_H
#define NEWVERSIONDIALOG_H

#include <QDialog>
#include <QFile>
#include <QNetworkReply>

#include "main.h"
#include "controller.h"

namespace Ui {
    class NewVersionDialog;
}


class NewVersionDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::NewVersionDialog*   ui;
    Controller*             m_ctrl;

    QPair<QString, QString> m_version;

    QFile           m_file;
    QNetworkReply*  m_reply;
    
public:
    NewVersionDialog(Controller* _ctrl, QWidget* _parent = 0);
    ~NewVersionDialog();

private:
    void errorMessage();

private slots:
    void on_updateButton_clicked();

    void slotDataReady();
    void slotDownloadFinished();
    void slotDownloadProgress(qint64 _received, qint64 _total);
};

#endif // NEWVERSIONDIALOG_H
