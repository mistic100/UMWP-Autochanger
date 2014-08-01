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
    
public:
    MainWidget(QWidget* _parent, Controller* _ctrl);
    ~MainWidget();

    QList<int> getSelectedIndexes();

public slots:
    void slotUpdateList(bool _resetSel);
    void slotMoveItem(const QModelIndex &, int _from, int, const QModelIndex &, int _to);

private slots:
    void on_buttonAdd_clicked();
    void on_buttonActivate_clicked();
    void on_buttonDeactivate_clicked();
    void on_buttonDelete_clicked();

    void on_mainList_itemSelectionChanged();
    void on_mainList_itemDoubleClicked(QListWidgetItem*);
};

#endif // MAINWIDGET_H
