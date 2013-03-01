#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QPushButton>
#include "mainwindow.h"
#include "errorwidget.h"


/*
 * constructor
 */
ErrorWidget::ErrorWidget(QWidget* _parent, Controller* _poCtrl) : QWidget(_parent)
{
    m_poCtrl = _poCtrl;
    m_poEditPath = NULL;
    int iState = m_poCtrl->settings()->iState();


    QGridLayout* poMainLayout = new QGridLayout(); // 6 columns

    // error icon
    QLabel* poIcon = new QLabel();
    poIcon->setPixmap(QPixmap(":/img/error"));
    poMainLayout->addWidget(poIcon, 0, 0, 2, 1);

    // oups !
    QLabel* poTitle = new QLabel(tr("Oops !"));
    poTitle->setFont(QFont("Calibri", 16));
    poMainLayout->addWidget(poTitle, 0, 1, 1, 5);

    // error description
    QLabel* poDescription = new QLabel();
    poDescription->setWordWrap(true);
    poMainLayout->addWidget(poDescription, 1, 1, 1, 5);

    // spacer
    QSpacerItem* poSpacer = new QSpacerItem(100, 100, QSizePolicy::Ignored, QSizePolicy::Ignored);
    poMainLayout->addItem(poSpacer, 2, 0, 1, 6);

    // ASK ULTRAMON.EXE PATH
    if (iState & UM_NOT_INSTALLED)
    {
        poDescription->setText(tr("Unable to locate UltraMon install directory.<br>Please indicate the location of <b>UltraMonDesktop.exe</b>"));

        QPushButton* poButtonBrowse = new QPushButton(tr("Browse"));
        QPushButton* poButtonSubmit = new QPushButton(tr("Continue"));
        m_poEditPath = new QLineEdit(m_poCtrl->settings()->sExePath());

        poMainLayout->addWidget(m_poEditPath, 3, 0, 1, 5);
        poMainLayout->addWidget(poButtonBrowse, 3, 5, 1, 1);
        poMainLayout->addWidget(poButtonSubmit, 4, 5, 1, 1);

        connect(poButtonBrowse, SIGNAL(clicked()), this, SLOT(vSlotBrowse()));
        connect(poButtonSubmit, SIGNAL(clicked()), this, SLOT(vSlotSubmit()));
    }
    // OTHER ERRORS
    else
    {
        if (iState & UM_FILE_NOT_FOUND)
        {
            QString text = tr("<b>default.wallpaper</b> fil not found, impossible to continue.<br><br>Sould be at: %1")
                                .arg("<i>"+m_poCtrl->settings()->sWallPath()+"</i>");
            poDescription->setText(text);
        }
        else if (iState & UM_BAD_VERSION)
        {
            QString text = tr("%1 is incompatible with the current version of UltraMon (%2)")
                                .arg(QString::fromAscii(APP_NAME)+" "+QString::fromAscii(APP_VERSION))
                                .arg(m_poCtrl->settings()->sUMVersion());
            poDescription->setText(text);
        }
    }

    setLayout(poMainLayout);
}

/*
 * open file dialog
 */
void ErrorWidget::vSlotBrowse()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Locate UltraMonDesktop"), QDir::rootPath(), "UltraMonDesktop (UltraMonDesktop.exe)");

    if (!filename.isEmpty())
    {
        filename.replace('/', '\\');
        m_poEditPath->setText(filename);
    }
}

/*
 * submit UltraMonDesktop.exe path
 */
void ErrorWidget::vSlotSubmit()
{
    QString filename = m_poEditPath->text();

    if (m_poCtrl->settings()->bSetExePath(filename))
    {
        MainWindow* w = qobject_cast<MainWindow*>(parent());
        w->vInit();
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid path"), QMessageBox::Ok);
    }
}
