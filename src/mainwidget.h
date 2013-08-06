#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QPushButton>
#include <QListWidget>

#include "main.h"
#include "controller.h"


/*
 * Name: MainWidget
 * Description: The main widget of the software
 */
class MainWidget : public QWidget
{
    Q_OBJECT

private:
    Controller*     m_poCtrl;

    QPushButton*    m_poDeleteButton;
    QPushButton*    m_poActivateButton;
    QPushButton*    m_poUnactivateButton;

    QListWidget*    m_poList;

    QString         m_sLastDir;

public:
    MainWidget(QWidget* _parent, Controller* _poCtrl);

    QList<int> oGetSelectedIndexes();

public slots:
    void vUpdateList();

    void vSlotAddSet();
    void vSlotDeleteSets();
    void vSlotActivateSets();
    void vSlotUnactivateSets();

    void vSlotSelectionChanged();
    void vSlotItemDoubleClicked();
    void vSlotItemMoved(const QModelIndex &, int from, int, const QModelIndex &, int to);
};

#endif // MAINWIDGET_H
