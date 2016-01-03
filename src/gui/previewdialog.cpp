#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFontMetrics>

#include "previewdialog.h"
#include "../umutils.h"


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
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);
    setWindowTitle(tr("Active files"));
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

    // rare case
    if (m_ctrl->currentSet() == NULL)
    {
        return;
    }

    // width of the thumbnail
    int width = 150;
    if (m_ctrl->currentSet()->style() == UM::IM_CUSTOM)
    {
        width = 100;
    }
    else if (m_ctrl->currentSet()->type() == UM::W_DESKTOP)
    {
        width*= m_ctrl->settings()->nbEnabledMonitors();
    }

    QPen pen(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(QColor(0, 160, 255));

    int col = 0; int row = 0;
    foreach (const QString file, m_ctrl->currentFiles())
    {
        if (file.isEmpty())
        {
            continue;
        }

        // resize image and add blue border
        QPixmap image = QPixmap(file).scaledToWidth(width, Qt::FastTransformation);
        QPainter painter(&image);
        painter.setPen(pen);
        painter.drawRect(image.rect().adjusted(1, 1, -1, -1));

        // thumbnail in a button
        QPushButton* thumb = new QPushButton();
        thumb->setIcon(image);
        thumb->setIconSize(image.size());
        thumb->setStyleSheet("border:none;");
        thumb->setProperty("path", file);
        thumb->setCursor(Qt::PointingHandCursor);

        connect(thumb, SIGNAL(clicked()), this, SLOT(onThumbnailClicked()));

        m_layout->addWidget(thumb, row, col, Qt::AlignBottom);

        // label with filename
        QString text = fontMetrics().elidedText(QFileInfo(file).fileName(), Qt::ElideRight, width);
        QLabel* label = new QLabel(text);
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        label->setCursor(Qt::IBeamCursor);

        m_layout->addWidget(label, row+1, col);

        // delete button
        QPushButton* button = new QPushButton();
        button->setIcon(QIcon(":/images/icons/bullet_cross.png"));
        button->setText(tr("Delete"));
        button->setProperty("path", file);
        button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

        connect(button, SIGNAL(clicked()), this, SLOT(onDeleteButtonClicked()));

        m_layout->addWidget(button, row+2, col);

        col++;
        if (col > 5)
        {
            col = 0;
            row+= 3;
        }
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

        UM::moveFileToTrash(path);
        m_ctrl->update();
    }
}
