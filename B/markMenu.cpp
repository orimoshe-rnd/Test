#include <markMenu.h>

MarkMenu::MarkMenu(QWidget* parent, QString objectName, double X, double Y, double Z) : QGroupBox(parent)
{
    this->setTitle("Mark Menu");
    
    layout = new QFormLayout();
    layout->setObjectName(QStringLiteral("layout"));
    
    buttonLayout = new QVBoxLayout();


    delMarkButton = new QPushButton(tr("Delete Mark"));
    delGroupButton = new QPushButton(tr("Delete Group"));
    exportGroupButton = new QPushButton(tr("Export Group"));
    exportAllButton = new QPushButton(tr("Export All"));


    label_name = new QLabel();
    label_name->setText("name:");
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
    layout->addRow(label_name, name);
    layout->addRow(label_x, x);
    layout->addRow(label_y, y);
    layout->addRow(label_z, z);

    buttonLayout->addLayout(layout);
    buttonLayout->addWidget(delMarkButton);
    buttonLayout->addWidget(exportGroupButton);
    buttonLayout->addWidget(exportAllButton);
    buttonLayout->addWidget(delGroupButton);

    
    this->setLayout(buttonLayout);
    // this->show();
}

MarkMenu::MarkMenu(QWidget* parent) : QGroupBox(parent)
{
    this->setTitle("Mark Menu");
    
    layout = new QFormLayout();
    layout->setObjectName(QStringLiteral("layout"));
    
    buttonLayout = new QVBoxLayout();


    delMarkButton = new QPushButton(tr("Delete Mark"));
    delGroupButton = new QPushButton(tr("Delete Group"));
    exportGroupButton = new QPushButton(tr("Export Group"));
    exportAllButton = new QPushButton(tr("Export All"));

    label_name = new QLabel();
    label_name->setText("name:");

    name = new QLineEdit();
    name->setObjectName(QStringLiteral("name"));
    // name->setText(objectName);
    

    label_x = new QLabel();
    label_x->setObjectName(QStringLiteral("label_2"));
    label_x->setText("x:");
    

    x = new QLineEdit();
    x->setObjectName(QStringLiteral("x"));
    // x->setText(QString::number(X));

    

    label_y = new QLabel();
    label_y->setObjectName(QStringLiteral("label"));
    label_y->setText("y:");
    

    y = new QLineEdit();
    y->setObjectName(QStringLiteral("y"));
    // y->setText(QString::number(Y));

    

    label_z = new QLabel();
    label_z->setObjectName(QStringLiteral("label_3"));
    label_z->setText("z:");
    

    z = new QLineEdit();
    z->setObjectName(QStringLiteral("z"));
    // z->setText(QString::number(Z));

    

    /* set layout */
    layout->addRow(label_name, name);
    layout->addRow(label_x, x);
    layout->addRow(label_y, y);
    layout->addRow(label_z, z);

    buttonLayout->addLayout(layout);
    buttonLayout->addWidget(delMarkButton);
    buttonLayout->addWidget(exportGroupButton);
    buttonLayout->addWidget(exportAllButton);
    buttonLayout->addWidget(delGroupButton);

    this->setLayout(buttonLayout);
    this->show();
}

void MarkMenu::updateFields(std::string objectName, double xLocation, double yLocation, double zLocation)
{
    name->setText(QString::fromStdString(objectName));
    x->setText(QString::fromStdString(std::to_string(xLocation)));
    y->setText(QString::fromStdString(std::to_string(yLocation)));
    z->setText(QString::fromStdString(std::to_string(zLocation)));
}

void MarkMenu::clearFields() 
{
    name->setText(QString::fromStdString(""));
    x->setText(QString::fromStdString(""));
    y->setText(QString::fromStdString(""));
    z->setText(QString::fromStdString(""));
}