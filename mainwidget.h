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

    QSpinBox*       m_poDelayInput;
    QListWidget*    m_poList;

    int             m_iSelectedSet; // index of selected item in the list

public:
    MainWidget(QWidget* _parent=0, Controller* _poCtrl=0);
    ~MainWidget();

    void vUpdateList();
    void vSetListItemIcon(QListWidgetItem* _poItem, bool _active);

public slots:
    void vSlotAddSet();
    void vSlotDeleteSet();
    void vSlotSwitchSet();
    void vSlotItemDoubleClicked();
    void vSlotItemClicked(QListWidgetItem* _poItem);
    void vSlotDelayChanged(int _val);
};

#endif // MAINWIDGET_H
