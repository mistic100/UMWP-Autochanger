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

    ui->view->installEventFilter(this);

    renderPreview();
}

bool CustomLayoutDialog::eventFilter(QObject* _target, QEvent* _event)
{
    if (_target == ui->view && _event->type() == QEvent::MouseButtonPress)
    {
        renderPreview();
        return true;
    }

    return QDialog::eventFilter(_target, _event);
}

/**
 * @brief Set the custom layout config
 * @param CustomLayout _layout
 */
void CustomLayoutDialog::setCustLayout(const CustomLayout &_layout)
{
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

    on_rows_valueChanged(_layout.rows);
    on_cols_valueChanged(_layout.cols);
}

/**
 * @brief Returns a new custom layout config
 * @return CustomLayout
 */
CustomLayout CustomLayoutDialog::getCustLayout() const
{
    CustomLayout layout;

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
 * @brief Render the preview
 */
void CustomLayoutDialog::renderPreview()
{
    try {
        CustomLayout layout = getCustLayout();
        QList<QRect> rawBlocks = m_generator->generate(layout);

        m_scene->clear();

        QStringList colors(COLORS);
        std::random_shuffle(colors.begin(), colors.end());

        QSize size = ui->view->size();
        QList<QRect> blocks;

        // scale blocks to wallpaper size
        double wRatio =  size.width() / (double) layout.cols;
        double hRatio =  size.height() / (double) layout.rows;

        foreach (const QRect block, rawBlocks)
        {
            QRect newBlock = UM::scaledRect(block, wRatio, hRatio);

            if (qAbs(newBlock.left() - size.width()) <= 2)
            {
                newBlock.setLeft(size.width());
            }

            if (qAbs(newBlock.bottom() - size.height()) <= 2)
            {
                newBlock.setBottom(size.height());
            }

            blocks.append(newBlock);
        }

        // draw blocks
        int i = 0;
        foreach (const QRect block, blocks)
        {
            m_scene->addRect(block, Qt::NoPen, QBrush(QColor(colors.at(i))));

            i++;
            if (i == colors.size()) i = 0;
        }

        // draw borders
        if (layout.borderEnabled)
        {
            // get border width for the view
            int monitorWidth = size.width();
            for (int i=0, l=m_ctrl->enviro()->nbMonitors(); i<l; i++)
            {
                if (m_ctrl->settings()->monitor(i).enabled)
                {
                    monitorWidth = m_ctrl->enviro()->wpSize(i).width();
                    break;
                }
            }

            QPen pen;
            pen.setColor(QColor(layout.borderColor));
            pen.setWidth(qRound((double) layout.borderWidth * size.width() / monitorWidth));

            foreach (const QRect block, blocks)
            {
                foreach (const QLine line, UM::rectBorders(block))
                {
                    if (
                            (line.x1()==0 && line.x2()==0) ||
                            (line.x1()==size.width()-1 && line.x2()==size.width()-1) ||
                            (line.y1()==0 && line.y2()==0) ||
                            (line.y1()==size.height()-1 && line.y2()==size.height()-1)
                            )
                    {
                        // do not draw extreme borders
                    }
                    else
                    {
                        m_scene->addLine(line, pen);
                    }
                }
            }
        }

        ui->view->update();
    }
    catch(const std::exception) {
        // do not update the view
    }
}
