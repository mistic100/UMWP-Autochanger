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
    setWindowTitle(tr("Active files"));

    connect(m_ctrl, SIGNAL(generationFinished()), this, SLOT(draw()));

    draw();

    QLOG_TRACE() << "PreviewDialog openned";
}

/**
 * @brief PreviewDialog::~PreviewDialog
 */
PreviewDialog::~PreviewDialog()
{
    if (m_needsUpdate)
    {
        m_ctrl->update();
    }
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

    int width;
    if (current.type == UM::W_DESKTOP && current.sets.at(0)->style() != UM::IM_CUSTOM)
    {
        width = 200 * m_ctrl->settings()->nbEnabledMonitors();
    }
    else if (current.files.size() <= 4)
    {
        width = 150;
    }
    else
    {
        width = 100;
    }

    int col = 0; int row = 0;

    bool showEdit = !m_settings->param(UM::CONF::open_program).toString().isEmpty();

    foreach (const QString &file, current.files)
    {
        if (file.isEmpty())
        {
            continue;
        }

        QFileInfo info(file);
        QString image = info.isFile() ? file : getFolderImage(file);

        PreviewWidget* widget = new PreviewWidget(file, image, width, showEdit && info.isFile(), info.isDir(), this);

        m_layout->addWidget(widget, row, col);

        col++;
        if (col > 5)
        {
            col = 0;
            row+= 1;
        }
    }
}

/**
 * @brief Get the first image of a folder
 * @param string _folder
 * @return string
 */
QString PreviewDialog::getFolderImage(const QString &_folder)
{
    for (int i = 0; i < m_settings->nbSets(); i++)
    {
        Set* set = m_settings->set(i);

        for (int j = 0; j < set->nbFolders(); j++)
        {
            if (_folder == set->folder(j))
            {
                return set->fileInFolder(_folder, 0);
            }
        }
    }

    return "";
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
int PreviewDialog::onDeleteButtonClicked(const QString &_path)
{
    int ret = QMessageBox::warning(this, tr("Delete"), tr("Are you sure?"),
                                   QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        QLOG_TRACE() << "Delete" << _path;

        UM::moveFileToTrash(_path);
        m_needsUpdate = true;
    }

    return ret;
}

/**
 * @brief Open file explorer with file selected
 * @param string _path
 */
void PreviewDialog::onOpenButtonClicked(const QString &_path)
{
    QLOG_TRACE() << "Locate" << _path;

    QFileInfo info(_path);

    if (info.isDir())
    {
        QDesktopServices::openUrl(QUrl("file:///"+ _path));
    }
    else
    {
        QProcess::startDetached(Environment::EXPLORER_PATH, QStringList() << "/select," + _path);
    }
}
