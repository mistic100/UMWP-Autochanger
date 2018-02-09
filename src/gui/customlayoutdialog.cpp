#include <QToolTip>
#include <QGraphicsRectItem>
#include <QGraphicsDropShadowEffect>

#include "../customlayoutgenerator.h"
#include "../umutils.h"

#include "customlayoutdialog.h"
#include "ui_customlayoutdialog.h"

// 20 random colors generated with mistic100/RandomColor.php
static const QStringList COLORS = QStringList()
        <<"#aefc92"<<"#d779f7"<<"#bbc3f7"<<"#a7f470"<<"#fcfca6"<<"#9ac8f9"
        <<"#ccfffe"<<"#8dfca5"<<"#abfcf3"<<"#b0ef86"<<"#adff87"<<"#8cff9f"
        <<"#f9d4a9"<<"#f9fca6"<<"#91f28c"<<"#f99795"<<"#a5e1f7"<<"#9bffc9"
        <<"#f9ed9a"<<"#b2fcae"<<"#a4f9e8"<<"#abfcca"<<"#a7e3f2"<<"#f7c9ff";

/**
 * @brief CustomLayoutDialog::CustomLayoutDialog
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
CustomLayoutDialog::CustomLayoutDialog(QWidget* _parent, Controller* _ctrl) :
    QDialog(_parent),
    ui(new Ui::CustomLayoutDialog),
    m_ctrl(_ctrl)
{
    ui->setupUi(this);
    ui_mainPosition = new QButtonGroupExt(this);

    ui->tileRows->setHandleMovementMode(QxtSpanSlider::NoCrossing);
    ui->tileCols->setHandleMovementMode(QxtSpanSlider::NoCrossing);

    ui_mainPosition->addButton(ui->positionTopLeft,         UM::ALIGN_TOP_LEFT);
    ui_mainPosition->addButton(ui->positionTopCenter,       UM::ALIGN_TOP_CENTER);
    ui_mainPosition->addButton(ui->positionTopRight,        UM::ALIGN_TOP_RIGHT);
    ui_mainPosition->addButton(ui->positionCenterLeft,      UM::ALIGN_CENTER_LEFT);
    ui_mainPosition->addButton(ui->positionCenterCenter,    UM::ALIGN_CENTER_CENTER);
    ui_mainPosition->addButton(ui->positionCenterRight,     UM::ALIGN_CENTER_RIGHT);
    ui_mainPosition->addButton(ui->positionBottomLeft,      UM::ALIGN_BOTTOM_LEFT);
    ui_mainPosition->addButton(ui->positionBottomCenter,    UM::ALIGN_BOTTOM_CENTER);
    ui_mainPosition->addButton(ui->positionBottomRight,     UM::ALIGN_BOTTOM_RIGHT);

    setFixedSize(size());
    setWindowFlags(UM::SimpleDialogFlag);

    std::random_device rd;
    m_randomEngine.seed(rd());

    m_generator = new CustomLayoutGenerator();
}

/**
 * @brief CustomLayoutDialog::~CustomLayoutDialog
 */
CustomLayoutDialog::~CustomLayoutDialog()
{
    delete ui;
    delete m_generator;
}

/**
 * @brief Init the graphics view on show event
 * The view must be rendered to query its size
 */
void CustomLayoutDialog::showEvent(QShowEvent*)
{
    QRect rect(QPoint(0, 0), ui->view->size());

    m_scene = new QGraphicsScene(ui->view);
    m_scene->setSceneRect(rect);
    ui->view->setRenderHint(QPainter::Antialiasing);
    ui->view->setScene(m_scene);

    connect(ui->rows,           SIGNAL(valueChanged(int)),      this, SLOT(renderPreview()));
    connect(ui->cols,           SIGNAL(valueChanged(int)),      this, SLOT(renderPreview()));
    connect(ui->tileCols,       SIGNAL(spanChanged(int,int)),   this, SLOT(renderPreview()));
    connect(ui->tileRows,       SIGNAL(spanChanged(int,int)),   this, SLOT(renderPreview()));
    connect(ui->mainEnabled,    SIGNAL(toggled(bool)),          this, SLOT(renderPreview()));
    connect(ui_mainPosition,    SIGNAL(buttonClicked(int)),     this, SLOT(renderPreview()));
    connect(ui->mainCols,       SIGNAL(valueChanged(int)),      this, SLOT(renderPreview()));
    connect(ui->mainRows,       SIGNAL(valueChanged(int)),      this, SLOT(renderPreview()));
    connect(ui->borderEnabled,  SIGNAL(toggled(bool)),          this, SLOT(renderPreview()));
    connect(ui->borderWidth,    SIGNAL(valueChanged(int)),      this, SLOT(renderPreview()));
    connect(ui->borderColor,    SIGNAL(colorChanged(QColor)),   this, SLOT(renderPreview()));
    connect(ui->shadowEnabled,  SIGNAL(toggled(bool)),          this, SLOT(renderPreview()));
    connect(ui->shadowWidth,    SIGNAL(valueChanged(int)),      this, SLOT(renderPreview()));
    connect(ui->shadowColor,    SIGNAL(colorChanged(QColor)),   this, SLOT(renderPreview()));
    connect(ui->variationEnabled,SIGNAL(toggled(bool)),         this, SLOT(renderPreview()));
    connect(ui->angleVariation, SIGNAL(valueChanged(int)),      this, SLOT(renderPreview()));
    connect(ui->sizeVariation,  SIGNAL(valueChanged(int)),      this, SLOT(renderPreview()));
    connect(ui->posVariation,   SIGNAL(valueChanged(int)),      this, SLOT(renderPreview()));

    renderPreview();
}

/**
 * @brief Set the custom layout config
 * @param CustomLayout _layout
 */
void CustomLayoutDialog::setCustLayout(const CustomLayout &_layout)
{
    ui->checkFolder->setChecked(_layout.perFolder);

    ui->rows->setValue(_layout.rows);
    ui->cols->setValue(_layout.cols);
    ui->tileRows->setSpan(_layout.minRows, _layout.maxRows);
    ui->tileCols->setSpan(_layout.minCols, _layout.maxCols);

    ui->mainEnabled->setChecked(_layout.mainEnabled);
    ui->mainRows->setValue(_layout.mainRows);
    ui->mainCols->setValue(_layout.mainCols);
    ui_mainPosition->setCheckedId(_layout.mainPos);

    ui->borderEnabled->setChecked(_layout.borderEnabled);
    ui->borderWidth->setValue(_layout.borderWidth);
    ui->borderColor->setColor(QColor(_layout.borderColor));

    ui->shadowEnabled->setChecked(_layout.shadowEnabled);
    ui->shadowWidth->setValue(_layout.shadowWidth);
    ui->shadowColor->setColor(QColor(_layout.shadowColor));

    ui->variationEnabled->setChecked(_layout.variationEnabled);
    ui->angleVariation->setValue(_layout.angleVariation * 10);
    ui->sizeVariation->setValue(_layout.sizeVariation * 10);
    ui->posVariation->setValue(_layout.posVariation * 10);

    on_rows_valueChanged(_layout.rows);
    on_cols_valueChanged(_layout.cols);
    on_borderWidth_valueChanged(_layout.borderWidth);
    on_shadowWidth_valueChanged(_layout.shadowWidth);
}

/**
 * @brief Returns a new custom layout config
 * @return CustomLayout
 */
CustomLayout CustomLayoutDialog::getCustLayout() const
{
    CustomLayout layout;

    layout.perFolder = ui->checkFolder->isChecked();

    layout.rows = ui->rows->value();
    layout.cols = ui->cols->value();
    layout.minRows = ui->tileRows->lowerValue();
    layout.maxRows = ui->tileRows->upperValue();
    layout.minCols = ui->tileCols->lowerValue();
    layout.maxCols = ui->tileCols->upperValue();

    layout.mainEnabled = ui->mainEnabled->isChecked();
    layout.mainRows = ui->mainRows->value();
    layout.mainCols = ui->mainCols->value();
    layout.mainPos = static_cast<UM::ALIGN>(ui_mainPosition->checkedId());

    layout.borderEnabled = ui->borderEnabled->isChecked();
    layout.borderWidth = ui->borderWidth->value();
    layout.borderColor = ui->borderColor->getColor().rgb();

    layout.shadowEnabled = ui->shadowEnabled->isChecked();
    layout.shadowWidth = ui->shadowWidth->value();
    layout.shadowColor = ui->shadowColor->getColor().rgb();

    layout.variationEnabled = ui->variationEnabled->isChecked();
    layout.angleVariation = ui->angleVariation->value() / 10.0;
    layout.sizeVariation = ui->sizeVariation->value() / 10.0;
    layout.posVariation = ui->posVariation->value() / 10.0;

    return layout;
}

/**
 * @brief Change rows spinners bounds when the main one changes
 * @param int _val
 */
void CustomLayoutDialog::on_rows_valueChanged(int _val)
{
    ui->tileRows->setMaximum(_val);
    ui->mainRows->setMaximum(_val);
    ui->tileRows->setDisabled(_val==1);
    ui->mainRows->setDisabled(_val==1);
    ui->tileRowsMaxLabel->setText(QString::number(_val));
    ui->mainRowsMaxLabel->setText(QString::number(_val));
}

/**
 * @brief Change cols spinners bounds when the main one changes
 * @param int _val
 */
void CustomLayoutDialog::on_cols_valueChanged(int _val)
{
    ui->tileCols->setMaximum(_val);
    ui->mainCols->setMaximum(_val);
    ui->tileCols->setDisabled(_val==1);
    ui->mainCols->setDisabled(_val==1);
    ui->tileColsMaxLabel->setText(QString::number(_val));
    ui->mainColsMaxLabel->setText(QString::number(_val));
}

/**
 * @brief Update border width label
 * @param _val
 */
void CustomLayoutDialog::on_borderWidth_valueChanged(int _val)
{
    ui->borderWidthLabel->setText(QString::number(_val) + " px");
}

/**
 * @brief Update shadow width label
 * @param _val
 */
void CustomLayoutDialog::on_shadowWidth_valueChanged(int _val)
{
    ui->shadowWidthLabel->setText(QString::number(_val) + " px");
}

/**
 * @brief Render the preview
 */
void CustomLayoutDialog::renderPreview()
{
    CustomLayout layout = getCustLayout();
    QList<QRect> rawBlocks = m_generator->generate(layout);

    if (rawBlocks.empty())
    {
        // do not update the view
        return;
    }

    m_scene->clear();

    QStringList colors(COLORS);
    std::shuffle(colors.begin(), colors.end(), m_randomEngine);

    QSize size = ui->view->size();
    QList<QRect> blocks;

    int monitorWidth = size.width();
    QColor monitorColor;
    for (int i=0, l=m_ctrl->enviro()->nbMonitors(); i<l; i++)
    {
        if (m_ctrl->settings()->monitor(i).enabled)
        {
            monitorWidth = m_ctrl->enviro()->wpSize(i).width();
            monitorColor = QColor(m_ctrl->settings()->monitor(i).color);
            break;
        }
    }

    // scale blocks to wallpaper size
    double wRatio =  size.width() / (double) layout.cols;
    double hRatio =  size.height() / (double) layout.rows;

    foreach (const QRect block, rawBlocks)
    {
        QRect newBlock = UM::scaledRect(block, wRatio, hRatio);

        if (qAbs(newBlock.right() - size.width()) <= 3)
        {
            newBlock.setRight(size.width()-1);
        }

        if (qAbs(newBlock.bottom() - size.height()) <= 3)
        {
            newBlock.setBottom(size.height()-1);
        }

        blocks.append(newBlock);
    }

    // draw background
    m_scene->addRect(QRect(QPoint(0, 0), size), Qt::NoPen, QBrush(monitorColor));

    // init random engine
    if (layout.variationEnabled)
    {
        std::shuffle(blocks.begin(), blocks.end(), m_randomEngine);
    }

    std::normal_distribution<double> randomAngle(0.0, 0.3 + 30.0 * layout.angleVariation);
    std::normal_distribution<double> randomSize(0.2 * layout.sizeVariation, 0.002 + 0.2 * layout.sizeVariation);
    std::normal_distribution<double> randomPos(0.0, 0.001 + 0.1 * layout.posVariation);

    QPen pen(Qt::NoPen);

    if (layout.borderEnabled)
    {
        pen.setStyle(Qt::SolidLine);
        pen.setColor(QColor(layout.borderColor));
        pen.setJoinStyle(Qt::MiterJoin);
        pen.setWidth(qRound((double) layout.borderWidth * size.width() / monitorWidth));
    }

    // draw blocks
    int i = 0;
    foreach (QRect block, blocks)
    {
        QGraphicsRectItem* item = m_scene->addRect(block, pen, QBrush(QColor(colors.at(i))));

        if (layout.shadowEnabled)
        {
            QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
            effect->setColor(QColor(layout.shadowColor));
            effect->setOffset(0);
            effect->setBlurRadius(layout.shadowWidth);

            item->setGraphicsEffect(effect);
        }

        if (layout.variationEnabled)
        {
            int newWidth = block.width() * (1 + randomSize(m_randomEngine));
            int newHeight = block.height() * (1 + randomSize(m_randomEngine));
            int newLeft = block.left() + (block.width() - newWidth) / 2 + randomPos(m_randomEngine) * newWidth;
            int newTop = block.top() + (block.height() - newHeight) / 2 + randomPos(m_randomEngine) * newHeight;
            double angle = randomAngle(m_randomEngine);

            block = QRect(newLeft, newTop, newWidth, newHeight);
            item->setRect(block);
            item->setTransformOriginPoint(block.center());
            item->setRotation(angle);
        }

        i++;
        if (i == colors.size()) i = 0;
    }

    ui->view->update();
}

void CustomLayoutDialog::on_helpFolder_clicked()
{
    QToolTip::showText(QCursor::pos(), tr("When this box is checked, the images will be grabbed from one of the sub-folders of the set instead of the entire set.<br><b>Note:</b> this option is only available in random mode."), this);
}

void CustomLayoutDialog::on_buttonRefresh_clicked()
{
    renderPreview();
}
