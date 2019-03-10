#ifndef QBUTTONGROUPEXT
#define QBUTTONGROUPEXT

#include <QButtonGroup>
#include <QAbstractButton>

/**
 * @brief QButtonGroup which allow to set the checked radio button by its id
 */
class QButtonGroupExt : public QButtonGroup
{
    Q_OBJECT

public:
    QButtonGroupExt(QWidget* _parent = nullptr) : QButtonGroup(_parent) {}

    void setCheckedId(int _id)
    {
        foreach (QAbstractButton* button, buttons())
        {
            if (id(button) == _id)
            {
                button->setChecked(true);
                break;
            }
        }
    }
};

#endif // QBUTTONGROUPEXT

