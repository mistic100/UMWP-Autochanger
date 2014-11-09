#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>

#include "previewdialog.h"


/**
 * @brief PreviewDialog::PreviewDialog
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
PreviewDialog::PreviewDialog(QWidget* _parent, Controller* _ctrl) :
    QDialog(_parent),
    m_ctrl(_ctrl)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    m_layout = new QGridLayout();

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout->addLayout(m_layout);
    mainLayout->addWidget(buttons);
    setLayout(mainLayout);

    connect(m_ctrl, SIGNAL(wallpaperChanged()), this, SLOT(draw()));
    draw();

    setWindowTitle(tr("Active files"));

    qxtLog->trace("PreviewDialog openned");
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

    // rare case but generates segfault
    if (m_ctrl->currentSet() == NULL)
    {
        return;
    }

    // width of the thumbnail
    int width = 150;
    if (m_ctrl->currentSet()->type() == UM::W_DESKTOP)
    {
        width*= m_ctrl->settings()->nbEnabledMonitors();
    }

    QPen pen(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(QColor(0, 160, 255));

    int i = 0, height = 0;
    for (QVector<QString>::const_iterator it=m_ctrl->files().constBegin(); it!=m_ctrl->files().constEnd(); ++it)
    {
        if ((*it).isEmpty())
        {
            continue;
        }

        // label with filename
        QLabel* label = new QLabel(QFileInfo(*it).fileName());
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        label->setCursor(Qt::IBeamCursor);

        // resize image and add blue border
        QPixmap image = QPixmap(*it).scaledToWidth(width, Qt::FastTransformation);
        QPainter painter(&image);
        painter.setPen(pen);
        painter.drawRect(image.rect().adjusted(1, 1, -1, -1));

        // thumbnail in a button
        QPushButton* thumb = new QPushButton();
        thumb->setIcon(image);
        thumb->setIconSize(image.size());
        thumb->setStyleSheet("border:none;");
        thumb->setProperty("path", *it);
        thumb->setCursor(Qt::PointingHandCursor);

        connect(thumb, SIGNAL(clicked()), this, SLOT(onThumbnailClicked()));

        // delete button
        QPushButton* button = new QPushButton();
        button->setIcon(QIcon(":/icon/bullet_cross"));
        button->setText(tr("Delete"));
        button->setProperty("path", *it);
        button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

        connect(button, SIGNAL(clicked()), this, SLOT(onDeleteButtonClicked()));

        m_layout->addWidget(label, 0, i);
        m_layout->addWidget(thumb, 1, i);
        m_layout->addWidget(button, 2, i);

        i++;
        height = qMax(height, image.height());
    }

    setFixedSize(QSize(i*width+22+(i-1)*6, height+100));
}

/**
 * @brief Open image file with default application
 */
void PreviewDialog::onThumbnailClicked()
{
    QWidget* thumb = (QWidget*)QObject::sender();
    QString path = thumb->property("path").toString();

    qxtLog->trace("Open "+ path);

    QDesktopServices::openUrl(QUrl("file:///"+ path));
}

/**
 * @brief Delete image file and refresh wallpaper
 */
void PreviewDialog::onDeleteButtonClicked()
{
    QWidget* button = (QWidget*)QObject::sender();
    QString path = button->property("path").toString();

    int ret = QMessageBox::warning(this, tr("Delete"), tr("Are you sure?"),
                                   QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        qxtLog->trace("Delete "+ path);

        m_ctrl->moveFileToTrash(path);
        m_ctrl->onUpdate(true);
    }
}
