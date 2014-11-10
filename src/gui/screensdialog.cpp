#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QColorDialog>

#include "screensdialog.h"
#include "ui_screensdialog.h"


/**
 * @brief ScreensDialog::ScreensDialog
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
ScreensDialog::ScreensDialog(QWidget* _parent, Controller* _ctrl) :
    QDialog(_parent),
    ui(new Ui::ScreensDialog),
    m_ctrl(_ctrl),
    m_currentScreen(-1)
{
    ui->setupUi(this);

    for (int i=0, l=m_ctrl->enviro()->nbMonitors(); i<l; i++)
    {
        m_monitors.append(m_ctrl->settings()->monitor(i));
    }

    init();

    setWindowFlags(SimpleDialogFlag);

    layout()->setSizeConstraint(QLayout::SetFixedSize);

    qxtLog->trace("ScreensDialog openned");
}

/**
 * @brief ScreensDialog::~ScreensDialog
 */
ScreensDialog::~ScreensDialog()
{
    delete ui;

    qDeleteAll(m_thumbs);
}

/**
 * @brief Validate config before save
 * @param int result
 */
void ScreensDialog::done(int result)
{
    if (result == QDialog::Accepted)
    {
        bool one = false;
        foreach (Monitor mon, m_monitors) one|= mon.enabled;

        if (!one)
        {
            qxtLog->error("At least one monitor must be enabled");

            QMessageBox::critical(this, tr("Error"), tr("At least one monitor must be enabled"), QMessageBox::Ok, QMessageBox::Ok);
        }
        else
        {
            QDialog::done(result);
        }
    }
    else
    {
        QDialog::done(result);
    }
}

/**
 * @brief Save changes in Settings object
 */
void ScreensDialog::save()
{
    for (int i=0, l=m_ctrl->enviro()->nbMonitors(); i<l; i++)
    {
        m_ctrl->settings()->setMonitor(i, m_monitors[i]);
    }

    m_ctrl->settings()->save();

    qxtLog->trace("Configuration updated");
}

/**
 * @brief Build the scenes displaying monitors
 */
void ScreensDialog::init()
{
    m_ratio = 400.f/m_ctrl->enviro()->wpSize(-1).width();
    m_viewport = m_ctrl->enviro()->wpSize(-1).scaled(m_ratio);

    QRect rect(QPoint(0, 0), m_viewport.size());

    m_scene = new QGraphicsScene(ui->view);
    m_scene->installEventFilter(this);
    m_scene->setSceneRect(rect);
    m_scene->addRect(rect, Qt::NoPen, QBrush(Qt::gray));

    for (int i=0, l=m_ctrl->enviro()->nbMonitors(); i<l; i++)
    {
        addScreen(i, m_ctrl->enviro()->wpSize(i));
    }

    ui->view->setScene(m_scene);
}

/**
 * @brief Add a monitor in the scene
 * @param int _i
 * @param QScreen _screen
 */
void ScreensDialog::addScreen(int _i, const QScreen &_screen)
{
    QScreen scaled = _screen.scaled(m_ratio);
    scaled.translate(-m_viewport.left()+5, -m_viewport.top()+5);
    scaled.setWidth(scaled.width()-10);
    scaled.setHeight(scaled.height()-10);

    ScreenThumbnail* thumb = new ScreenThumbnail();
    m_thumbs.append(thumb);

    thumb->rect = m_scene->addRect(scaled);
    thumb->rect->setData(Qt::UserRole, _i);

    QFont font;
    font.setPixelSize(scaled.height()*0.6);

    thumb->text = m_scene->addSimpleText(QString::number(_i+1), font);
    thumb->text->setData(Qt::UserRole, _i);

    QRectF bound = thumb->text->boundingRect();
    QPoint pos = scaled.center();
    pos.rx()-= bound.width()/2;
    pos.ry()-= bound.height()/2;

    thumb->text->setPos(pos);

    updateScreen(_i);
}

/**
 * @brief Update aspect of a monitor
 * @param int _i
 */
void ScreensDialog::updateScreen(int _i)
{
    ScreenThumbnail* thumb = m_thumbs.at(_i);

    QColor color((QRgb) m_monitors[_i].color);

    if (m_monitors[_i].enabled)
    {
        thumb->rect->setBrush(QBrush(color));
        thumb->text->setBrush(QBrush(Qt::white));
    }
    else
    {
        thumb->rect->setBrush(QBrush(color, Qt::Dense3Pattern));
        thumb->text->setBrush(QBrush(QColor(255,255,255,150)));
    }

    thumb->text->setPen(QPen(Qt::black));

    if (_i == m_currentScreen)
    {
        thumb->rect->setPen(QPen(QBrush(QColor(51,153,255)), 4, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    }
    else
    {
        thumb->rect->setPen(QPen(Qt::white));
    }
}

/**
 * @brief Set a monitor as current selection
 * @param int _i
 */
void ScreensDialog::selectScreen(int _i)
{
    int previous = m_currentScreen;
    m_currentScreen = _i;

    if (previous > -1)
    {
        updateScreen(previous);
    }

    if (m_currentScreen > -1)
    {
        updateScreen(m_currentScreen);

        ui->enabled->setEnabled(true);
        ui->enabled->setChecked(m_monitors[m_currentScreen].enabled);
        ui->color->setEnabled(true);
    }
    else
    {
        ui->enabled->setEnabled(false);
        ui->enabled->setChecked(false);
        ui->color->setEnabled(false);
    }
}

/**
 * @brief Handle events on the scene
 * @param QEvent* _event
 * @return bool
 */
bool ScreensDialog::eventFilter(QObject*, QEvent* _event)
{
    if (_event->type() == QEvent::GraphicsSceneMouseRelease)
    {
        QGraphicsSceneMouseEvent* event = static_cast<QGraphicsSceneMouseEvent*>(_event);
        QGraphicsItem* item = m_scene->itemAt(event->scenePos(), QTransform());
        QVariant nb = item->data(Qt::UserRole);

        if (!nb.isNull())
        {
            selectScreen(nb.toInt());
            return true;
        }
        else
        {
            selectScreen(-1);
        }
    }

    return false;
}

/**
 * @brief Display color-picker dialog
 */
void ScreensDialog::on_color_clicked()
{
    QColor color = QColorDialog::getColor(QColor((QRgb) m_monitors[m_currentScreen].color),
                                          this, tr("Change color"));

    if (color.isValid())
    {
        m_monitors[m_currentScreen].color = (COLORREF) color.rgb();

        updateScreen(m_currentScreen);
    }
}

/**
 * @brief Toggle the state if the selected monitor
 * @param bool _enabled
 */
void ScreensDialog::on_enabled_clicked(bool _enabled)
{
    m_monitors[m_currentScreen].enabled = _enabled;

    updateScreen(m_currentScreen);
}
