#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QListWidget>

#include "../main.h"
#include "../controller.h"

class MainWindow;

/**
 * @brief The main widget of the software
 */
class MainList : public QListWidget
{
    Q_OBJECT

private:
    Controller* m_ctrl;
    Settings* m_settings;
    MainWindow* m_parent;
    
public:
    MainList(MainWindow* _parent, Controller* _ctrl);

    QList<int> getSelectedIndexes() const;
    QList<Set*> getSelectedSets() const;

protected:
    bool eventFilter(QObject* _target, QEvent* _event);

public slots:
    void onListChanged(bool _resetSel);

private slots:
    void onItemMoved(const QModelIndex &, int _from, int, const QModelIndex &, int _to);
    void onItemDoubleClicked(QListWidgetItem*);
    void onContextMenu(const QPoint &_pos);
};

#endif // MAINWIDGET_H
