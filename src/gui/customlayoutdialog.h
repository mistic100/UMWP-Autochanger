#ifndef CustomLayoutDialog_H
#define CustomLayoutDialog_H

#include <QDialog>
#include <QSpinBox>
#include <QGraphicsScene>
#include <QShowEvent>

#include "../main.h"
#include "../controller.h"
#include "../customlayout.h"
#include "../customlayoutgenerator.h"


namespace Ui {
    class CustomLayoutDialog;
}

/**
 * @brief Dialog used to configure and preview the custom layout
 */
class CustomLayoutDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::CustomLayoutDialog* ui;
    QGraphicsScene* m_scene;

    Controller* m_ctrl;
    CustomLayoutGenerator* m_generator;

    QList<QSpinBox*> m_rowsSpins;
    QList<QSpinBox*> m_colsSpins;

public:
    CustomLayoutDialog(QWidget* _parent, Controller* _ctrl);
    ~CustomLayoutDialog();

    void setCustLayout(const CustomLayout &_layout);
    CustomLayout getCustLayout() const;

protected:
    void showEvent(QShowEvent*);

private slots:
    void on_rows_valueChanged(int _val);
    void on_cols_valueChanged(int _val);
    void on_previewButton_clicked();
};

#endif // CustomLayoutDialog_H
