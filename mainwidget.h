#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QPushButton>
#include <QListWidget>
#include <QSpinBox>
#include "main.h"
#include "controller.h"

/*
 * the main widget of the software
 */
class MainWidget : public QWidget
{
    Q_OBJECT

private:
    Controller*     m_poCtrl;

    QPushButton*    m_poDeleteButton;
    QPushButton*    m_poActivateButton;
    QPushButton*    m_poUnactivateButton;

    QSpinBox*       m_poDelayInput;
    QListWidget*    m_poList;

public:
    MainWidget(QWidget* _parent=0, Controller* _poCtrl=0);
    ~MainWidget();

    void vUpdateList();
    void vSetListItemIcon(QListWidgetItem* _poItem, bool _active);
    QList<int> oGetSelectedIndexes();

public slots:
    void vSlotAddSet();
    void vSlotDeleteSets();
    void vSlotActivateSets();
    void vSlotUnactivateSets();
    void vSlotSelectionChanged();
    void vSlotItemDoubleClicked();
    void vSlotDelayChanged(int _val);
};

#endif // MAINWIDGET_H
