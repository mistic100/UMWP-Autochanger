#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QPushButton>

#include "mainwindow.h"
#include "errorwidget.h"


/**
 * @brief ErrorWidget::ErrorWidget
 * @param QWidget* _parent
 * @param Controller* _poCtrl
 */
ErrorWidget::ErrorWidget(QWidget* _parent, Controller* _poCtrl) : QWidget(_parent)
{
    m_poCtrl = _poCtrl;

    m_poEditPath = NULL;


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
    QString sDescription;
    QLabel* poDescription = new QLabel();
    poDescription->setWordWrap(true);
    poMainLayout->addWidget(poDescription, 1, 1, 1, 5);

    // spacer
    QSpacerItem* poSpacer = new QSpacerItem(100, 100, QSizePolicy::Ignored, QSizePolicy::Ignored);
    poMainLayout->addItem(poSpacer, 2, 0, 1, 6);

    int iState = m_poCtrl->settings()->iState();

    // ASK ULTRAMON.EXE PATH
    if (iState & UM_NOT_INSTALLED)
    {
        sDescription = tr("Unable to locate UltraMon install directory.<br>Please indicate the location of <b>UltraMonDesktop.exe</b>");

        QPushButton* poButtonBrowse = new QPushButton(tr("Browse"));
        QPushButton* poButtonSubmit = new QPushButton(tr("Continue"));
        m_poEditPath = new QLineEdit(m_poCtrl->settings()->sParam("umpath"));

        poMainLayout->addWidget(m_poEditPath, 3, 0, 1, 5);
        poMainLayout->addWidget(poButtonBrowse, 3, 5, 1, 1);
        poMainLayout->addWidget(poButtonSubmit, 4, 5, 1, 1);

        connect(m_poEditPath,   SIGNAL(returnPressed()), this, SLOT(slotSubmit()));
        connect(poButtonBrowse, SIGNAL(clicked()), this, SLOT(slotBrowse()));
        connect(poButtonSubmit, SIGNAL(clicked()), this, SLOT(slotSubmit()));
    }
    // OTHER ERRORS
    else if (iState & UM_FILE_NOT_FOUND)
    {
        sDescription = tr("<b>default.wallpaper</b> fil not found, impossible to continue.<br><br>Sould be at: %1")
                        .arg("<i>" + m_poCtrl->settings()->sEnv("wallpath") + "</i>");
    }
    else if (iState & UM_BAD_VERSION)
    {
        sDescription = tr("%1 is incompatible with the current version of UltraMon (%2)")
                        .arg(QString::fromAscii(APP_NAME) + " " + QString::fromAscii(APP_VERSION))
                        .arg(m_poCtrl->settings()->sEnv("umversion"));
    }

    poDescription->setText(sDescription);

    setLayout(poMainLayout);
}

/**
 * @brief Open file dialog
 */
void ErrorWidget::slotBrowse()
{
    QString sFilename = QFileDialog::getOpenFileName(this, tr("Locate UltraMonDesktop"),
                                                     QDir::rootPath(), "UltraMonDesktop (UltraMonDesktop.exe)");

    if (!sFilename.isEmpty())
    {
        sFilename.replace('/', '\\');
        m_poEditPath->setText(sFilename);
    }
}

/**
 * @brief Submit UltraMonDesktop.exe path
 * If success this widget will be destroyed and replaced by MainWidget
 */
void ErrorWidget::slotSubmit()
{
    QString filename = m_poEditPath->text();

    if (m_poCtrl->settings()->bSetExePath(filename))
    {
        emit pathSaved();
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid path"), QMessageBox::Ok);
    }
}
