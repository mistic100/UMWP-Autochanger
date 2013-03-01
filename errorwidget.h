#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H

#include <QLineEdit>
#include "main.h"
#include "controller.h"

class ErrorWidget : public QWidget
{
    Q_OBJECT

private:
    Controller*     m_poCtrl;
    QLineEdit*      m_poEditPath;

public:
    ErrorWidget(QWidget* _parent=0, Controller* _poCtrl=0);
    
public slots:
    void vSlotBrowse();
    void vSlotSubmit();
};

#endif // ERRORWIDGET_H
