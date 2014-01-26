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
    Controller* m_ctrl;
    QLineEdit*  m_inputPath;

public:
    ErrorWidget(QWidget* _parent, Controller* _ctrl);
    
public slots:
    void slotBrowse();
    void slotSubmit();

signals:
    void pathSaved();
};

#endif // ERRORWIDGET_H
