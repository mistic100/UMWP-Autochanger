#ifndef QCOMBOBOXEXT_H
#define QCOMBOBOXEXT_H

#include <QWidget>
#include <QComboBox>


/**
 * @brief QComboBox With direct access to item data
 */
class QComboBoxExt : public QComboBox
{
    Q_OBJECT

public:
    QComboBoxExt(QWidget* _parent = 0) : QComboBox(_parent) {}

    void setCurrentData(const QVariant &_data)
    {
        setCurrentIndex(findData(_data));
    }
    
    QVariant currentData()
    {
        return itemData(currentIndex());
    }
};

#endif // QCOMBOBOXEXT_H
