#ifndef QTABWIDGETEXT
#define QTABWIDGETEXT

#include <QTabWidget>

/**
 * @brief QTabWidget which allow to set the current tab by its name
 */
class QTabWidgetExt : public QTabWidget
{
    Q_OBJECT

public:
    QTabWidgetExt(QWidget* _parent = nullptr) : QTabWidget(_parent) {}

    void setCurrentTab(const QString &_tabName)
    {
        foreach (QWidget* child, findChildren<QWidget*>())
        {
            if (child->objectName() == _tabName)
            {
                setCurrentIndex(indexOf(child));
            }
        }
    }
};

#endif // QTABWIDGETEXT

