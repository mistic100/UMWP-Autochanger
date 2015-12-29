#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QPainter>
#include <QDesktopServices>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QMessageBox>
#include <QFontMetrics>

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

    setWindowFlags(SimpleDialogFlag);

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

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

    int i = 0;
    for (QVector<QString>::const_iterator it=m_ctrl->files().constBegin(); it!=m_ctrl->files().constEnd(); ++it)
    {
        if ((*it).isEmpty())
        {
            continue;
        }

        // label with filename
        QString text = fontMetrics().elidedText(QFileInfo(*it).fileName(), Qt::ElideRight, width);
        QLabel* label = new QLabel(text);
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        label->setCursor(Qt::IBeamCursor);

        m_layout->addWidget(label, 0, i);

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

        m_layout->addWidget(thumb, 1, i);

        // delete button, only if wallpapers are not custom
        if (m_ctrl->currentSet()->style() != UM::IM_CUSTOM)
        {
            QPushButton* button = new QPushButton();
            button->setIcon(QIcon(":/images/icons/bullet_cross.png"));
            button->setText(tr("Delete"));
            button->setProperty("path", *it);
            button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

            connect(button, SIGNAL(clicked()), this, SLOT(onDeleteButtonClicked()));

            m_layout->addWidget(button, 2, i);
        }

        i++;
    }
}

/**
 * @brief Open image file with default application
 */
void PreviewDialog::onThumbnailClicked()
{
    QWidget* thumb = (QWidget*)QObject::sender();
    QString path = thumb->property("path").toString();

    QLOG_TRACE() << "Open " << path;

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
        QLOG_TRACE() << "Delete " << path;

        m_ctrl->moveFileToTrash(path);
        m_ctrl->update();
    }
}
