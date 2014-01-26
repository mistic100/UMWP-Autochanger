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
    QToolBarExt(QWidget* _parent = 0);

    QToolButton* addButton(const QString &text);
    QToolButton* addButton(const QIcon &icon, const QString &text, Qt::ToolButtonStyle style=Qt::ToolButtonIconOnly);
    QToolButton* addButton(const QIcon &icon, const QString &text, QMenu* menu, Qt::ToolButtonStyle style=Qt::ToolButtonIconOnly);
};

#endif // QTOOLBAREXT_H
