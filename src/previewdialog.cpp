#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QDesktopServices>

#include "previewdialog.h"
#include "ui_previewdialog.h"


/**
 * @brief PreviewDialog::PreviewDialog
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
PreviewDialog::PreviewDialog(QWidget* _parent, Controller* _ctrl) : QDialog(_parent),
    ui(new Ui::PreviewDialog)
{
    ui->setupUi(this);

    int width = 150;
    if (_ctrl->set()->type() == UM::W_DESKTOP)
    {
        width*= _ctrl->settings()->iEnv("nb_monitors");
    }

    QPen pen(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(QColor(0, 160, 255));

    int i = 0, height = 0;
    for (QVector<QString>::const_iterator it=_ctrl->files().begin(); it!=_ctrl->files().end(); ++it)
    {
        // resize image and add blue border
        QPixmap image = QPixmap(*it).scaledToWidth(width, Qt::SmoothTransformation);
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

        connect(button, SIGNAL(clicked()), this, SLOT(slotImageClicked()));

        ui->gridLayout->addWidget(label, 0, i);
        ui->gridLayout->addWidget(button, 1, i);

        i++;
        height = qMax(height, image.height());
    }

    setFixedSize(QSize(i*width+22+(i-1)*6, height+70));
}

/**
 * @brief PreviewDialog::~PreviewDialog
 */
PreviewDialog::~PreviewDialog()
{
    delete ui;
}

/**
 * @brief Open image file with default application
 */
void PreviewDialog::slotImageClicked()
{
    QWidget* image = (QWidget*)QObject::sender();

    QDesktopServices::openUrl(QUrl("file:///"+ image->property("path").toString()));
}
