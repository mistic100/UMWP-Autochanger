#ifndef CHANGELOGDIALOG_H
#define CHANGELOGDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QDesktopServices>

#include "ui_changelogdialog.h"

namespace Ui {
    class ChangelogDialog;
}


/**
 * @brief Simple dialog with a rich text viewer
 */
class ChangelogDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::ChangelogDialog *ui;

public:
    ChangelogDialog(QWidget* _parent = 0) :
        QDialog(_parent),
        ui(new Ui::ChangelogDialog)
    {
        ui->setupUi(this);

        QFile file;
        QString lang = QLocale::system().name().section('_', 0, 0);
        if (lang.compare("fr")==0)
        {
            file.setFileName(":/lang/fr_FR/changelog.htm");
        }
        else
        {
            file.setFileName(":/lang/en_GB/changelog.htm");
        }

        QString text;
        text.append("<style>");
        text.append("* { font-family: \"Arial\", sans-serif; font-size: 12px; }");
        text.append("p { font-size:14px; }");
        text.append("dt { font-weight:bold; color:#4078C0; font-size:16px; }");
        text.append("dd { margin:0 0 2em 0; }");
        text.append("</style>");

        file.open(QIODevice::ReadOnly);
        QTextStream content(&file);
        content.setCodec("UTF-8");
        text.append(content.readAll());
        file.close();

        ui->content->setText(text);

        setWindowTitle(tr("Changelog"));
    }

    ~ChangelogDialog()
    {
        delete ui;
    }

private slots:
    void on_content_anchorClicked(const QUrl &_url)
    {
        QDesktopServices::openUrl(_url);
    }
};

#endif // CHANGELOGDIALOG_H
