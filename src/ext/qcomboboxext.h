#ifndef QCOMBOBOXEXT_H
#define QCOMBOBOXEXT_H

#include <QWidget>
#include <QComboBox>


/**
 * @brief QComboBox With direct access to item data
 * @signal currentDataChanged(QVariant)
 */
class QComboBoxExt : public QComboBox
{
    Q_OBJECT

public:
    QComboBoxExt(QWidget* _parent = nullptr) : QComboBox(_parent)
    {
        connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(on_currentIndexChanged(int)));
    }

    void setCurrentData(const QVariant &_data)
    {
        setCurrentIndex(findData(_data));
    }
    
    QVariant currentData()
    {
        return itemData(currentIndex());
    }

signals:
    void currentDataChanged(QVariant);

private slots:
    void on_currentIndexChanged(int index)
    {
        emit currentDataChanged(itemData(index));
    }
};

#endif // QCOMBOBOXEXT_H
