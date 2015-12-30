#include "../customlayoutgenerator.h"

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

    m_generator = new CustomLayoutGenerator();

    setFixedSize(size());
    setWindowFlags(UM::SimpleDialogFlag);

    m_rowsSpins.append(ui->maxRows);
    m_rowsSpins.append(ui->minRows);
    m_rowsSpins.append(ui->mainRows);
    m_colsSpins.append(ui->maxCols);
    m_colsSpins.append(ui->minCols);
    m_colsSpins.append(ui->mainCols);

    ui->mainPos->addItem(tr("Center"),        UM::ALIGN_CENTER_CENTER);
    ui->mainPos->addItem(tr("Top-Left"),      UM::ALIGN_TOP_LEFT);
    ui->mainPos->addItem(tr("Top-Center"),    UM::ALIGN_TOP_CENTER);
    ui->mainPos->addItem(tr("Top-Right"),     UM::ALIGN_TOP_RIGHT);
    ui->mainPos->addItem(tr("Center-Left"),   UM::ALIGN_CENTER_LEFT);
    ui->mainPos->addItem(tr("Center-Right"),  UM::ALIGN_CENTER_RIGHT);
    ui->mainPos->addItem(tr("Bottom-Left"),   UM::ALIGN_BOTTOM_LEFT);
    ui->mainPos->addItem(tr("Bottom-Center"), UM::ALIGN_BOTTOM_CENTER);
    ui->mainPos->addItem(tr("Bottom-Right"),  UM::ALIGN_BOTTOM_RIGHT);
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

    on_previewButton_clicked();
}

/**
 * @brief Set the custom layout config
 * @param CustomLayout _layout
 */
void CustomLayoutDialog::setCustLayout(const CustomLayout &_layout)
{
    ui->rows->setValue(_layout.rows);
    ui->cols->setValue(_layout.cols);
    ui->minRows->setValue(_layout.minRows);
    ui->minCols->setValue(_layout.minCols);
    ui->maxRows->setValue(_layout.maxRows);
    ui->maxCols->setValue(_layout.maxCols);

    ui->mainEnabled->setChecked(_layout.mainEnabled);
    ui->mainRows->setValue(_layout.mainRows);
    ui->mainCols->setValue(_layout.mainCols);
    ui->mainPos->setCurrentData(_layout.mainPos);

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
    layout.minRows = ui->minRows->value();
    layout.minCols = ui->minCols->value();
    layout.maxRows = ui->maxRows->value();
    layout.maxCols = ui->maxCols->value();

    layout.mainEnabled = ui->mainEnabled->isChecked();
    layout.mainRows = ui->mainRows->value();
    layout.mainCols = ui->mainCols->value();
    layout.mainPos = static_cast<UM::ALIGN>(ui->mainPos->currentData().toInt());

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
    foreach (QSpinBox* spinBox, m_rowsSpins)
    {
        spinBox->setMaximum(_val);
        if (spinBox->value() > _val)
        {
            spinBox->setValue(_val);
        }
    }
}

/**
 * @brief Change cols spinners bounds when the main one changes
 * @param int _val
 */
void CustomLayoutDialog::on_cols_valueChanged(int _val)
{
    foreach (QSpinBox* spinBox, m_colsSpins)
    {
        spinBox->setMaximum(_val);
        if (spinBox->value() > _val)
        {
            spinBox->setValue(_val);
        }
    }
}

/**
 * @brief Render the preview
 */
void CustomLayoutDialog::on_previewButton_clicked()
{
    CustomLayout layout = getCustLayout();
    QList<QRect> rects = m_generator->generate(layout);

    QSize size = ui->view->size();
    m_scene->clear();

    QStringList colors(COLORS);
    std::random_shuffle(colors.begin(), colors.end());

    int i = 0;
    foreach (const QRect rect, rects)
    {
        QRect sceneRect(
                    qRound(rect.x() / (double) layout.cols * size.width()),
                    qRound(rect.y() / (double) layout.rows * size.height()),
                    qRound(rect.width() / (double) layout.cols * size.width()),
                    qRound(rect.height() / (double) layout.rows * size.height())
                    );

        m_scene->addRect(sceneRect, Qt::NoPen, QBrush(QColor(colors.at(i))));

        i++;
        if (i == colors.size()) i = 0;
    }

    ui->view->update();
}
