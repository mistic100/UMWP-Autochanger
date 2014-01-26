#include "qtoolbarext.h"


/**
 * @brief QToolBarExt::QToolBarExt
 * @param QWidget* _parent
 */
QToolBarExt::QToolBarExt(QWidget* _parent) : QToolBar(_parent) {}

/**
 * @brief QToolBarExt::addButton
 * @param string text
 * @return QToolButton*
 */
QToolButton* QToolBarExt::addButton(const QString &text)
{
    QToolButton* button = new QToolButton(this);
    QAction* action = addWidget(button);
    button->setText(text);
    action->setText(text);
    button->setDefaultAction(action);
    return button;
}

/**
 * @brief QToolBarExt::addButton
 * @param QIcon icon
 * @param string text
 * @param Qt::ToolButtonStyle style
 * @return QToolButton*
 */
QToolButton* QToolBarExt::addButton(const QIcon &icon, const QString &text, Qt::ToolButtonStyle style)
{
    QToolButton* button = addButton(text);
    button->setIcon(icon);
    button->setToolButtonStyle(style);
    return button;
}

/**
 * @brief QToolBarExt::addButton
 * @param QIcon icon
 * @param string text
 * @param QMenu* menu
 * @param Qt::ToolButtonStyle style
 * @return QToolButton*
 */
QToolButton* QToolBarExt::addButton(const QIcon &icon, const QString &text, QMenu* menu, Qt::ToolButtonStyle style)
{
    QToolButton* button = addButton(icon, text, style);
    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);
    return button;
}
