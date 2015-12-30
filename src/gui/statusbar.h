#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QLabel>

#include "../main.h"
#include "../controller.h"


class StatusBar : public QStatusBar
{
    Q_OBJECT

private:
    QLabel* m_loaderLabel;

public:
    StatusBar(QWidget* _parent, Controller* _ctrl);

public slots:
    void showLoader();
    void hideLoader();
};

#endif // STATUSBAR_H
