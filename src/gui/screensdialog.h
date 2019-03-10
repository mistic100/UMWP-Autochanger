#ifndef SCREENSDIALOG_H
#define SCREENSDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QGraphicsScene>

#include "../main.h"
#include "../controller.h"
#include "../environment.h"
#include "../settings.h"

namespace Ui {
    class ScreensDialog;
}


/**
 * @brief Dialog for monitors configuration
 */
class ScreensDialog : public QDialog
{
    Q_OBJECT

private:
    /**
     * @brief Store monitor UI elements
     */
    struct ScreenThumbnail
    {
        QGraphicsRectItem* rect;
        QGraphicsSimpleTextItem* text;
    };

private:
    Ui::ScreensDialog* ui;

    Controller* m_ctrl;
    Settings* m_settings;
    Environment* m_enviro;

    QVector<UM::Monitor> m_monitors;
    QList<ScreenThumbnail*> m_thumbs;
    QGraphicsScene* m_scene;

    QRect m_viewport;
    double m_ratio = 0.0;
    int m_currentScreen = -1;
    
public:
    ScreensDialog(QWidget* _parent, Controller* _ctrl);
    ~ScreensDialog();

    void save();

private:
    void init();

    void addScreen(int _i, const QRect &_screen);
    void updateScreen(int _i);
    void selectScreen(int _i);

protected:
    void done(int result);
    bool eventFilter(QObject*, QEvent* _event);

private slots:
    void on_color_clicked();
    void on_enabled_clicked(bool _enabled);
};

#endif // SCREENSDIALOG_H
