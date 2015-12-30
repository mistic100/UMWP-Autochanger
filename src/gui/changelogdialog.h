#ifndef CHANGELOGDIALOG_H
#define CHANGELOGDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QDesktopServices>

#include "../settings.h"
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
    ChangelogDialog(QWidget* _parent, Settings* _settings) :
        QDialog(_parent),
        ui(new Ui::ChangelogDialog)
    {
        ui->setupUi(this);

        QString text;

        // Append CSS file
        text.append("<style>");

        QFile cssFile(":/lang/changelog.css");

        cssFile.open(QIODevice::ReadOnly);
        QTextStream cssContent(&cssFile);
        cssContent.setCodec("UTF-8");
        text.append(cssContent.readAll());
        cssFile.close();

        text.append("</style>");

        // Append HTML file
        QString lang = _settings->param(UM::CONF::language).toString();
        QFile file(":/lang/" + lang + "/changelog.htm");

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
