#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QPushButton>

#include "mainwindow.h"
#include "errorwidget.h"

extern short UMWP_STATE;


/**
 * @brief ErrorWidget::ErrorWidget
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
ErrorWidget::ErrorWidget(QWidget* _parent, Controller* _ctrl) : QWidget(_parent)
{
    m_ctrl = _ctrl;
    m_inputPath = NULL;

    QGridLayout* mainLayout = new QGridLayout(); // 6 columns

    // error icon
    QLabel* icon = new QLabel();
    icon->setPixmap(QPixmap(":/img/error"));
    mainLayout->addWidget(icon, 0, 0, 2, 1);

    // oups !
    QLabel* title = new QLabel(tr("Oops !"));
    title->setFont(QFont("Calibri", 16));
    mainLayout->addWidget(title, 0, 1, 1, 5);

    // error description
    QLabel* description = new QLabel();
    description->setWordWrap(true);
    mainLayout->addWidget(description, 1, 1, 1, 5);

    // spacer
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Ignored, QSizePolicy::Ignored);
    mainLayout->addItem(spacer, 2, 0, 1, 6);

    // ASK ULTRAMON.EXE PATH
    if (UMWP_STATE & UMWP::NOT_INSTALLED)
    {
         description->setText(tr("Unable to locate UltraMon install directory.<br>Please indicate the location of <b>UltraMonDesktop.exe</b>"));

        QPushButton* pButtonBrowse = new QPushButton(tr("Browse"));
        QPushButton* pButtonSubmit = new QPushButton(tr("Continue"));
        m_inputPath = new QLineEdit(m_ctrl->settings()->sParam("umpath"));

        mainLayout->addWidget(m_inputPath, 3, 0, 1, 5);
        mainLayout->addWidget(pButtonBrowse, 3, 5, 1, 1);
        mainLayout->addWidget(pButtonSubmit, 4, 5, 1, 1);

        connect(m_inputPath,   SIGNAL(returnPressed()), this, SLOT(slotSubmit()));
        connect(pButtonBrowse, SIGNAL(clicked()), this, SLOT(slotBrowse()));
        connect(pButtonSubmit, SIGNAL(clicked()), this, SLOT(slotSubmit()));
    }
    // OTHER ERRORS
    else if (UMWP_STATE & UMWP::BAD_VERSION)
    {
         description->setText(tr("%1 is incompatible with the current version of UltraMon (%2).<br>Minimum version: %3")
                             .arg(QString::fromAscii(APP_NAME) + " " + QString::fromAscii(APP_VERSION))
                             .arg(m_ctrl->settings()->sEnv("umversion"))
                             .arg(QString::fromAscii(APP_MIN_UM_VERSION))
                             );
    }
    else if (UMWP_STATE & UMWP::FILE_NOT_FOUND)
    {
         description->setText(tr("<b>default.wallpaper</b> file not found, impossible to continue.<br><br>Sould be at: %1")
                             .arg("<i>" + m_ctrl->settings()->sEnv("wallpath") + "</i>")
                             );
    }
    else if (UMWP_STATE & UMWP::UNKNOWN_ERROR)
    {
         description->setText(tr("An unknown error appened!"));
    }

    setLayout(mainLayout);
}

/**
 * @brief Open file dialog
 */
void ErrorWidget::slotBrowse()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Locate UltraMonDesktop"),
                                                    QDir::rootPath(), "UltraMonDesktop (UltraMonDesktop.exe)");

    if (!filename.isEmpty())
    {
        filename.replace('/', '\\');
        m_inputPath->setText(filename);
    }
}

/**
 * @brief Submit UltraMonDesktop.exe path
 * If success this widget will be destroyed and replaced by MainWidget
 */
void ErrorWidget::slotSubmit()
{
    QString filename = m_inputPath->text();

    if (m_ctrl->settings()->setExePath(filename))
    {
        emit pathSaved();
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid path"), QMessageBox::Ok);
    }
}
