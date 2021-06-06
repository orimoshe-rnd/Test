#include "Group.h"


GroupBox::GroupBox(QListWidget* groupList, QVBoxLayout* groupLayout) : QObject()
{
    mainLayout = new QVBoxLayout();
        
    layout = new QVBoxLayout();
    layout->setObjectName("This");

    locationsTable = new QTableWidget();
    locationsTable->setColumnCount(4);
    locationsTable->setHorizontalHeaderLabels(QStringList() << "name" << "x" << "y" << "z");
    
    locationsTable->setObjectName("markList");
    
    color = new QPushButton();
    QPalette pal = color->palette();
    pal.setColor(QPalette::Button, QColor(Qt::blue));
    color->setAutoFillBackground(true);
    color->setPalette(pal);
    color->update();
    
    layout->addWidget(locationsTable);
    layout->addWidget(color);
    
    
    spoiler = new Spoiler(QString::fromStdString("Group "+std::to_string(groupLayout->count()+1)), 300, groupLayout->parentWidget());
    spoiler->setContentLayout(*layout);
    
    spoiler->setContextMenuPolicy(Qt::CustomContextMenu);

    groupLayout->addWidget(spoiler);
}
