#ifndef QPREVIEWWIDGET
#define QPREVIEWWIDGET

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileInfo>
#include <QPainter>
#include <QLabel>

#include "../main.h"
#include "previewdialog.h"

const int borderWidth = 2;
const QColor borderColor = QColor(0, 160, 255);


/**
 * @brief Widget used by PreviewDialog
 */
class PreviewWidget : public QWidget
{
    Q_OBJECT

public:
    PreviewWidget(const QString &_path, const QString &_image, int _width, bool _showEdit, bool _showOverlay, PreviewDialog* _parent) :
        QWidget(_parent)
    {
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setAlignment(Qt::AlignBottom);
        layout->setMargin(0);
        layout->setSpacing(0);

        // resize image and add blue border
        QPixmap image = QPixmap(_image).scaledToWidth(_width, Qt::FastTransformation);

        QPen pen(Qt::SolidLine);
        pen.setWidth(borderWidth);
        pen.setColor(borderColor);

        QPainter painter(&image);
        painter.setPen(pen);
        painter.drawRect(image.rect().adjusted(1, 1, -1, -1));

        if (_showOverlay)
        {
            QImage overlay = QImage(":/images/folder-overlay.png");
            painter.drawImage(image.width()-overlay.width()-5, image.height()-overlay.height()-5, overlay);
        }

        // thumbnail
        QPushButton* thumb = new QPushButton();
        thumb->setIcon(image);
        thumb->setIconSize(image.size());
        thumb->setStyleSheet("border:none;");
        thumb->setCursor(Qt::PointingHandCursor);

        layout->addWidget(thumb);
        connect(thumb, &QPushButton::clicked, _parent, [=]{ _parent->onThumbnailClicked(_path); });

        // label with filename
        QString text = fontMetrics().elidedText(QFileInfo(_path).completeBaseName(), Qt::ElideRight, _width);
        QLabel* label = new QLabel(text);
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        label->setCursor(Qt::IBeamCursor);

        layout->addWidget(label);

        // buttons
        QHBoxLayout* buttons = new QHBoxLayout();
        buttons->setAlignment(Qt::AlignRight);
        buttons->setMargin(0);
        buttons->setSpacing(0);
        layout->addLayout(buttons);

        // delete button
        QPushButton* deleteButton = new QPushButton();
        deleteButton->setIcon(QIcon(":/images/icons/cross.png"));
        deleteButton->setFlat(true);

        buttons->addWidget(deleteButton);
        connect(deleteButton, &QPushButton::clicked, _parent, [=]{ _parent->onDeleteButtonClicked(_path); });

        // edit button
        if (_showEdit)
        {
            QPushButton* editButton = new QPushButton();
            editButton->setIcon(QIcon(":/images/icons/edit.png"));
            editButton->setFlat(true);

            buttons->addWidget(editButton);
            connect(editButton, &QPushButton::clicked, _parent, [=]{ _parent->onEditButtonClicked(_path); });
        }

        // open button
        QPushButton* openButton = new QPushButton();
        openButton->setIcon(QIcon(":/images/icons/folder_go.png"));
        openButton->setFlat(true);

        buttons->addWidget(openButton);
        connect(openButton, &QPushButton::clicked, _parent, [=]{ _parent->onOpenButtonClicked(_path); });

        setLayout(layout);
    }
};

#endif // QPREVIEWWIDGET

