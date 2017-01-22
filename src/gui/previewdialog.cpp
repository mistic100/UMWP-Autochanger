#include <QDesktopServices>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>

#include "previewdialog.h"
#include "previewwidget.h"
#include "../umutils.h"


/**
 * @brief PreviewDialog::PreviewDialog
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
PreviewDialog::PreviewDialog(QWidget* _parent, Controller* _ctrl) :
    QDialog(_parent),
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings())
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    m_layout = new QGridLayout();

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout->addLayout(m_layout);
    mainLayout->addWidget(buttons);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);
    setWindowFlags(UM::SimpleDialogFlag);

    connect(m_ctrl, SIGNAL(generationFinished()), this, SLOT(draw()));

    draw();

    QLOG_TRACE() << "PreviewDialog openned";
}

/**
 * @brief Update dialog content
 */
void PreviewDialog::draw()
{
    // remove old widgets if any
    while (m_layout->count() > 0)
    {
        QLayoutItem* item = m_layout->takeAt(0);
        delete item->widget();
        delete item;
    }

    WallpaperGenerator::Result current = m_ctrl->current();

    setWindowTitle(current.set != NULL && current.set->perFolder() ? tr("Active folders") : tr("Active files"));

    // rare case
    if (current.set == NULL)
    {
        return;
    }

    // width of the thumbnail
    int width = 150;
    if (!current.set->perFolder())
    {
        if (current.set->style() == UM::IM_CUSTOM)
        {
            width = 100;
        }
        else if (current.set->type() == UM::W_DESKTOP)
        {
            width*= m_ctrl->settings()->nbEnabledMonitors();
        }
    }

    int col = 0; int row = 0;
    if (current.set->perFolder())
    {
        foreach (QString folder, current.folders)
        {
            PreviewWidget* widget = new PreviewWidget(folder, current.set->fileInFolder(folder, 0), width, false, true, this);

            m_layout->addWidget(widget, row, col);

            col++;
            if (col > 5)
            {
                col = 0;
                row+= 1;
            }
        }
    }
    else
    {
        bool showEdit = !m_settings->param(UM::CONF::open_program).toString().isEmpty();

        foreach (QString file, current.files)
        {
            if (file.isEmpty())
            {
                continue;
            }

            PreviewWidget* widget = new PreviewWidget(file, file, width, showEdit, false, this);

            m_layout->addWidget(widget, row, col);

            col++;
            if (col > 5)
            {
                col = 0;
                row+= 1;
            }
        }
    }
}

/**
 * @brief Open image file with default application
 * @param string _path
 */
void PreviewDialog::onThumbnailClicked(const QString &_path)
{
    QLOG_TRACE() << "Open" << _path;

    QDesktopServices::openUrl(QUrl("file:///"+ _path));
}

/**
 * @brief Open image file with custom application
 * @param string _path
 */
void PreviewDialog::onEditButtonClicked(const QString &_path)
{
    QLOG_TRACE() << "Edit" << _path;

    QString opener = m_settings->param(UM::CONF::open_program).toString();

    QProcess::startDetached(opener, QStringList() << _path);
}

/**
 * @brief Delete image file and refresh wallpaper
 * @param string _path
 */
void PreviewDialog::onDeleteButtonClicked(const QString &_path)
{
    int ret = QMessageBox::warning(this, tr("Delete"), tr("Are you sure?"),
                                   QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        QLOG_TRACE() << "Delete" << _path;

        UM::moveFileToTrash(_path);
        m_ctrl->update();
    }
}

/**
 * @brief Open file explorer with file selected
 * @param string _path
 */
void PreviewDialog::onOpenButtonClicked(const QString &_path)
{
    QLOG_TRACE() << "Locate" << _path;

    if (m_ctrl->current().set->perFolder())
    {
        QDesktopServices::openUrl(QUrl("file:///"+ _path));
    }
    else
    {
        QProcess::startDetached(Environment::EXPLORER_PATH, QStringList() << "/select," + _path);
    }
}
