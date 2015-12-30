#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "../main.h"
#include "../settings.h"


/**
 * @brief Simple dialog to display the help
 */
class HelpDialog : public QMessageBox
{
    Q_OBJECT

public:
    HelpDialog(QWidget* _parent, Settings* _settings) :
        QMessageBox(_parent)
    {
        QString text;

        // Append CSS file
        text.append("<style>");

        QFile cssFile(":/lang/help.css");

        cssFile.open(QIODevice::ReadOnly);
        QTextStream cssContent(&cssFile);
        cssContent.setCodec("UTF-8");
        text.append(cssContent.readAll());
        cssFile.close();

        text.append("</style>");

        // Append HTML file
        QString lang = _settings->param(UM::CONF::language).toString();
        QFile file(":/lang/" + lang + "/help.htm");

        file.open(QIODevice::ReadOnly);
        QTextStream content(&file);
        content.setCodec("UTF-8");
        text.append(content.readAll());
        file.close();

        setIcon(QMessageBox::Information);
        setWindowTitle(tr("User guide"));
        setText(text);
    }
};

#endif // HELPDIALOG_H
