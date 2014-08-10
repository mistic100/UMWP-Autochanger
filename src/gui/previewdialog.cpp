#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QDialogButtonBox>

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
        width*= m_ctrl->enviro()->get("nb_monitors").toInt();
    }

    QPen pen(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(QColor(0, 160, 255));

    int i = 0, height = 0;
    for (QVector<QString>::const_iterator it=m_ctrl->files().constBegin(); it!=m_ctrl->files().constEnd(); ++it)
    {
        // resize image and add blue border
        QPixmap image = QPixmap(*it).scaledToWidth(width, Qt::FastTransformation);
        QPainter painter(&image);
        painter.setPen(pen);
        painter.drawRect(image.rect().adjusted(1, 1, -1, -1));

        // display image in a button
        QPushButton* button = new QPushButton();
        button->setIcon(image);
        button->setIconSize(image.size());
        button->setStyleSheet("border:none;");
        button->setProperty("path", *it);
        button->setCursor(Qt::PointingHandCursor);

        // add label with filename
        QLabel* label = new QLabel(QFileInfo(*it).fileName());
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        label->setCursor(Qt::IBeamCursor);

        connect(button, SIGNAL(clicked()), this, SLOT(onThumbnailClicked()));

        m_layout->addWidget(label, 0, i);
        m_layout->addWidget(button, 1, i);

        i++;
        height = qMax(height, image.height());
    }

    setFixedSize(QSize(i*width+22+(i-1)*6, height+70));
}

/**
 * @brief Open image file with default application
 */
void PreviewDialog::onThumbnailClicked()
{
    QWidget* image = (QWidget*)QObject::sender();

    QDesktopServices::openUrl(QUrl("file:///"+ image->property("path").toString()));

    qxtLog->trace("Open "+ image->property("path").toString());
}
