#include "mark.h"

Mark::Mark(QWidget* parent, QString objectName, double X, double Y, double Z) : QWidget(parent)
{
    layout = new QHBoxLayout();
    layout->setObjectName(QStringLiteral("layout"));
    
    name = new QLineEdit();
    name->setObjectName(QStringLiteral("name"));
    name->setText(objectName);
    

    label_x = new QLabel();
    label_x->setObjectName(QStringLiteral("label_2"));
    label_x->setText("x:");
    

    x = new QLineEdit();
    x->setObjectName(QStringLiteral("x"));
    x->setText(QString::number(X));

    

    label_y = new QLabel();
    label_y->setObjectName(QStringLiteral("label"));
    label_y->setText("y:");
    

    y = new QLineEdit();
    y->setObjectName(QStringLiteral("y"));
    y->setText(QString::number(Y));

    

    label_z = new QLabel();
    label_z->setObjectName(QStringLiteral("label_3"));
    label_z->setText("z:");
    

    z = new QLineEdit();
    z->setObjectName(QStringLiteral("z"));
    z->setText(QString::number(Z));

    

    /* set layout */
    layout->addWidget(name);
    
    layout->addWidget(label_z);
    layout->addWidget(x);

    layout->addWidget(label_y);
    layout->addWidget(y);

    layout->addWidget(label_z);
    layout->addWidget(z);

    this->setLayout(layout);
    this->show();
}

void Mark::update(QString objectName, double X, double Y, double Z)
{
    name->setText(objectName);
    x->setText(QString::number(X));
    y->setText(QString::number(Y));
    z->setText(QString::number(Z));
}

