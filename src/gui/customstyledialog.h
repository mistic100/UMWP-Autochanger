#ifndef CUSTOMSTYLEDIALOG_H
#define CUSTOMSTYLEDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QGraphicsScene>
#include <QShowEvent>

#include "../main.h"
#include "../controller.h"
#include "../customlayout.h"
#include "../customlayoutgenerator.h"


namespace Ui {
    class CustomStyleDialog;
}

/**
 * @brief Dialog used to configure and preview the custom layout
 */
class CustomStyleDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::CustomStyleDialog* ui;
    QGraphicsScene* m_scene;

    Controller*            m_ctrl;
    CustomLayoutGenerator* m_generator;

    QList<QSpinBox*> m_rowsSpins;
    QList<QSpinBox*> m_colsSpins;

public:
    CustomStyleDialog(QWidget* _parent, Controller* _ctrl);
    ~CustomStyleDialog();

    void setCustLayout(const CustomLayout &_layout);
    CustomLayout getCustLayout() const;

protected:
    void showEvent(QShowEvent*);

private slots:
    void on_rows_valueChanged(int _val);
    void on_cols_valueChanged(int _val);
    void on_previewButton_clicked();
};

#endif // CUSTOMSTYLEDIALOG_H
