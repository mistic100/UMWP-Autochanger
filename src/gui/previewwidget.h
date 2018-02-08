#ifndef QPREVIEWWIDGET
#define QPREVIEWWIDGET

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileInfo>
#include <QPainter>
#include <QLabel>
#include <QMessageBox>

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

    PreviewDialog* m_parent;

    QString m_path;

    QPushButton* m_thumb;
    QPushButton* m_deleteButton;
    QPushButton* m_editButton;
    QPushButton* m_openButton;

public:
    PreviewWidget(const QString &_path, const QString &_image, int _width, bool _showEdit, bool _showOverlay, PreviewDialog* _parent) :
        QWidget(_parent),
        m_path(_path),
        m_parent(_parent)
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

        painter.end();

        // thumbnail
        m_thumb = new QPushButton();
        m_thumb->setIcon(image);
        m_thumb->setIconSize(image.size());
        m_thumb->setStyleSheet("border:none;");
        m_thumb->setCursor(Qt::PointingHandCursor);

        layout->addWidget(m_thumb);
        connect(m_thumb, &QPushButton::clicked, _parent, [=]{ _parent->onThumbnailClicked(_path); });

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
        m_deleteButton = new QPushButton();
        m_deleteButton->setIcon(QIcon(":/images/icons/cross.png"));
        m_deleteButton->setFlat(true);

        buttons->addWidget(m_deleteButton);
        connect(m_deleteButton, SIGNAL(clicked(bool)), this, SLOT(onDelete()));

        // edit button
        if (_showEdit)
        {
            m_editButton = new QPushButton();
            m_editButton->setIcon(QIcon(":/images/icons/edit.png"));
            m_editButton->setFlat(true);

            buttons->addWidget(m_editButton);
            connect(m_editButton, &QPushButton::clicked, _parent, [=]{ _parent->onEditButtonClicked(_path); });
        }
        else {
            m_editButton = nullptr;
        }

        // open button
        m_openButton = new QPushButton();
        m_openButton->setIcon(QIcon(":/images/icons/folder_go.png"));
        m_openButton->setFlat(true);

        buttons->addWidget(m_openButton);
        connect(m_openButton, &QPushButton::clicked, _parent, [=]{ _parent->onOpenButtonClicked(_path); });

        setLayout(layout);
    }

private slots:
    void onDelete()
    {
        if (m_parent->onDeleteButtonClicked(m_path) == QMessageBox::Ok)
        {
            QPixmap image = m_thumb->icon().pixmap(m_thumb->icon().availableSizes().first());

            QPen pen(Qt::SolidLine);
            pen.setWidth(borderWidth);
            pen.setColor(borderColor);

            QBrush brush(Qt::SolidPattern);
            brush.setColor(Qt::white);

            QPainter painter(&image);
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(image.rect());

            QPixmap icon = QPixmap(":/images/icons/broken-image.png");
            painter.drawPixmap(QPoint(4, 4), icon, icon.rect());

            painter.end();

            m_thumb->setIcon(image);

            m_thumb->setDisabled(true);
            m_deleteButton->setDisabled(true);
            m_openButton->setDisabled(true);
            if (m_editButton != nullptr)
            {
                m_editButton->setDisabled(true);
            }
        }
    }
};

#endif // QPREVIEWWIDGET

