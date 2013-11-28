#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QListWidget>

#include "main.h"
#include "controller.h"

namespace Ui {
    class MainWidget;
}

/*
 * Name: MainWidget
 * Description: The main widget of the software
 */
class MainWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::MainWidget* ui;

    Controller*     m_poCtrl;

    QString         m_sLastDir;
    
public:
    MainWidget(QWidget* _parent, Controller* _poCtrl);
    ~MainWidget();

    QList<int> oGetSelectedIndexes();

public slots:
    void vUpdateList(bool _bResetSel);
    void vSlotItemMoved(const QModelIndex &, int from, int, const QModelIndex &, int to);

    void on_m_poButtonAdd_clicked();
    void on_m_poButtonActivate_clicked();
    void on_m_poButtonDeactivate_clicked();
    void on_m_poButtonDelete_clicked();
    void on_m_poList_itemSelectionChanged();
    void on_m_poList_itemDoubleClicked(QListWidgetItem*);
};

#endif // MAINWIDGET_H
