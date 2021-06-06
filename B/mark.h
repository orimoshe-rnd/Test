#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class Mark : public QWidget
{
    Q_OBJECT
public:
    
    QHBoxLayout *layout;
    QLineEdit *name;
    QLabel *label_x;
    QLineEdit *x;
    QLabel *label_y;
    QLineEdit *y;
    QLabel *label_z;
    QLineEdit *z;

    Mark(QWidget* parent, QString name, double X, double Y, double Z);
    void update(QString objectName, double X, double Y, double Z);

};

