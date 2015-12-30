#ifndef QTOOLBAREXT_H
#define QTOOLBAREXT_H

#include <QToolBar>
#include <QMenu>
#include <QToolButton>


/**
 * @brief Custom QToolBar allowing to add a button/menu with text and icon
 */
class QToolBarExt : public QToolBar
{
    Q_OBJECT

public:
    QToolBarExt(QWidget* _parent) : QToolBar(_parent) {}

    /**
     * @brief Add a button with text only
     */
    QToolButton* addButton(const QString &_text)
    {
        QToolButton* button = new QToolButton(this);
        QAction* action = addWidget(button);
        button->setText(_text);
        action->setText(_text);
        button->setDefaultAction(action);
        return button;
    }

    /**
     * @brief Add a button with text and icon
     */
    QToolButton* addButton(const QIcon &_icon, const QString &_text,
                           Qt::ToolButtonStyle _style = Qt::ToolButtonTextBesideIcon)
    {
        QToolButton* button = addButton(_text);
        button->setIcon(_icon);
        button->setToolButtonStyle(_style);
        return button;
    }

    /**
     * @brief Add a menu with text only
     */
    QToolButton* addMenu(const QString &_text, QMenu* _menu,
                         QToolButton::ToolButtonPopupMode _mode = QToolButton::InstantPopup)
    {
        QToolButton* button = addButton(_text);
        button->setMenu(_menu);
        button->setPopupMode(_mode);
        return button;
    }

    /**
     * @brief Add a menu with text and icon
     */
    QToolButton* addMenu(const QIcon &_icon, const QString &_text, QMenu* _menu,
                         Qt::ToolButtonStyle _style = Qt::ToolButtonTextBesideIcon,
                         QToolButton::ToolButtonPopupMode _mode = QToolButton::InstantPopup)
    {
        QToolButton* button = addButton(_icon, _text, _style);
        button->setMenu(_menu);
        button->setPopupMode(_mode);
        return button;
    }
};

#endif // QTOOLBAREXT_H
