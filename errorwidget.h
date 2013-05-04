#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H

#include <QLineEdit>

#include "main.h"
#include "controller.h"

/*
 * Name: ErrorWidget
 * Description: Widget displayed when UltraMon can't be located
 */
class ErrorWidget : public QWidget
{
    Q_OBJECT

private:
    Controller*     m_poCtrl;

    QLineEdit*      m_poEditPath;

public:
    ErrorWidget(QWidget* _parent, Controller* _poCtrl);
    
public slots:
    void vSlotBrowse();
    void vSlotSubmit();
};

#endif // ERRORWIDGET_H
