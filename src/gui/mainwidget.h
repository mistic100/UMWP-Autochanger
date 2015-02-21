#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QListWidget>

#include "main.h"
#include "controller.h"

namespace Ui {
    class MainWidget;
}


/**
 * @brief The main widget of the software
 */
class MainWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::MainWidget* ui;

    Controller*     m_ctrl;
    Settings*       m_settings;
    
public:
    MainWidget(QWidget* _parent, Controller* _ctrl);
    ~MainWidget();

    QList<int> getSelectedIndexes();

public slots:
    void onListChanged(bool _resetSel);

private slots:
    void onItemMoved(const QModelIndex &, int _from, int, const QModelIndex &, int _to);
    void on_mainList_customContextMenuRequested(const QPoint &_pos);
};

#endif // MAINWIDGET_H
