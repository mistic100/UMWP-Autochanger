#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QtWidgets/QStatusBar>
#include <QtWidgets/QLabel>

#include "../main.h"
#include "../controller.h"


// cyclic reference
class MainWindow;

class StatusBar : public QStatusBar
{
    Q_OBJECT

private:
    QLabel* m_loaderLabel;

public:
    StatusBar(MainWindow* _parent, Controller* _ctrl);

public slots:
    void showLoader();
    void hideLoader();

};

#endif // STATUSBAR_H
