#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H

#include <QLineEdit>

#include "main.h"
#include "controller.h"


/**
 * @brief Widget displayed when UltraMon can't be located
 */
class ErrorWidget : public QWidget
{
    Q_OBJECT

private:
    Controller* m_pCtrl;
    QLineEdit*  m_pEditPath;

public:
    ErrorWidget(QWidget* _parent, Controller* _pCtrl);
    
public slots:
    void slotBrowse();
    void slotSubmit();

signals:
    void pathSaved();
};

#endif // ERRORWIDGET_H
