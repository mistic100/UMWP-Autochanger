#ifndef SCREENSDIALOG_H
#define SCREENSDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QGraphicsScene>

#include "main.h"
#include "controller.h"
#include "environment.h"

namespace Ui {
    class ScreensDialog;
}


/**
 * @brief Store monitor rect and text together
 */
struct ScreenThumbnail
{
    QGraphicsRectItem* rect;
    QGraphicsSimpleTextItem* text;
};


/**
 * @brief Dialog for monitors configuration
 */
class ScreensDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::ScreensDialog* ui;

    Controller* m_ctrl;

    QVector<Monitor> m_monitors;

    QList<ScreenThumbnail*> m_thumbs;
    QGraphicsScene* m_scene;

    QRect m_viewport;
    float m_ratio;
    int m_currentScreen;
    
public:
    ScreensDialog(QWidget* _parent, Controller* _ctrl);
    ~ScreensDialog();

    void save();

private:
    void init();
    void addScreen(int _i, const QRect &_screen);
    void updateScreen(int _i);
    void selectScreen(int _i);

    QRect scaledRect(const QRect &_rect, float _ratio);

protected:
    void done(int result);
    bool eventFilter(QObject*, QEvent* _event);

private slots:
    void on_color_clicked();
    void on_enabled_clicked(bool _enabled);
};

#endif // SCREENSDIALOG_H
