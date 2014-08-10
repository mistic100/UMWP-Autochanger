#ifndef QTOOLBAREXT_H
#define QTOOLBAREXT_H

#include <QToolBar>
#include <QMenu>
#include <QToolButton>


/**
 * @brief Custom QToolBar with addButton method
 */
class QToolBarExt : public QToolBar
{
    Q_OBJECT

public:
    QToolBarExt(QWidget* _parent) : QToolBar(_parent) {}

    QToolButton* addButton(const QString &_text)
    {
        QToolButton* button = new QToolButton(this);
        QAction* action = addWidget(button);
        button->setText(_text);
        action->setText(_text);
        button->setDefaultAction(action);
        return button;
    }

    QToolButton* addButton(const QIcon &_icon, const QString &_text, Qt::ToolButtonStyle _style = Qt::ToolButtonTextBesideIcon)
    {
        QToolButton* button = addButton(_text);
        button->setIcon(_icon);
        button->setToolButtonStyle(_style);
        return button;
    }

    QToolButton* addMenu(const QIcon &_icon, const QString &_text, QMenu* _menu, Qt::ToolButtonStyle _style = Qt::ToolButtonTextBesideIcon)
    {
        QToolButton* button = addButton(_icon, _text, _style);
        button->setMenu(_menu);
        button->setPopupMode(QToolButton::InstantPopup);
        return button;
    }
};

#endif // QTOOLBAREXT_H
