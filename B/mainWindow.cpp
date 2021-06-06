
#include "mainWindow.h"

void osgMainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (projectLoaded == false) 
    {
        return;
    }

	qtOsgWidget->controller->keyReleased(event->text().toStdString().c_str()[0]);
}

osgMainWindow::osgMainWindow()
{
    std::ifstream t("instructions.txt");
    instructions = std::string((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());

    qtOsgWidget = NULL;
    projectLoaded = false;
    createGUI();

    qtTimer = new QTimer(this);

    qtTimer->setInterval(40);
    qtTimer->start();
    
    auto glambda = [=]() 
    { 
        if(OSGOpen != true)
            return;
        qtOsgWidget->repaint();
    };

    connect(qtTimer, &QTimer::timeout, this, glambda);
    connect(qtTimer, &QTimer::timeout, this, glambda);
}

void osgMainWindow::closeEvent(QCloseEvent *event)
{
    if (changeSomething)
    {
        if (userSureToExit() == false)
        {
            event->ignore();
        }
    }
}

void osgMainWindow::save()
{
    if (projectLoaded == false)
    {
        return;
    }
    
    QMainWindow* window = new QMainWindow();
    //this func will open a file and write all neccessey data to it
    std::string fname = QFileDialog::getSaveFileName(window,"save work",".osgtool","osgtool (*.osgtool)").toStdString();
    
    delete window;

    if (fname == "")
    {
        return;
    }

    qtOsgWidget->save(fname);

    changeSomething = false;
}

void osgMainWindow::exit()
{
    if (changeSomething)
    {
        if (userSureToExit() == false)
        {
            return;
        }
    }
    QCoreApplication::quit();
}

void osgMainWindow::help() 
{
    QMessageBox msgBox;
    msgBox.setText(instructions.c_str());
    msgBox.exec();
}

void osgMainWindow::onUnloadProject() 
{
    delete groupList;
    delete modelNameLabel;
    delete colorButton;
    delete addGroup;
    delete delMarkButton;
    delete delGroupButton;
    delete exportGroupButton;
    delete exportAllButton;
    delete viewModeCheck;
    delete showRouteCheck;
    delete locationsTable;

    this->selectedGroup = nullptr;
    changeSomething = false;
}

void osgMainWindow::deleteGroup()
{
    if (this->selectedGroup == nullptr)
    {
        return;
    }


    qtOsgWidget->removeGroupApi(this->selectedGroup->text().toStdString());
    
    delete this->locationsTable;
    
    initLocationTable();

    QPalette pal = colorButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::gray));
    colorButton->setAutoFillBackground(true);
    colorButton->setPalette(pal);
    colorButton->update();

    delete this->selectedGroup;

    this->selectedGroup = nullptr;
    changeSomething = true;
}

void osgMainWindow::deleteMarker()
{
    if (this->selectedGroup == nullptr)
    {
        return;
    }

    std::string groupName = this->selectedGroup->text().toStdString();

    auto selectedMarks = locationsTable->selectedItems();

    std::set<std::string> markersToDelete;
    std::vector<int> rows;
   
    for (auto item : selectedMarks)
    {
        std::string mark = itemToName(item);
        bool alreadyMoved = markersToDelete.find(mark) != markersToDelete.end();
        if (alreadyMoved)
        {
            continue;
        }

        markersToDelete.insert(mark);
        rows.push_back(item->column());
    }

    for (auto markerName : markersToDelete)
    {
        qtOsgWidget->removeLocationApi(groupName, markerName);
    }

    for (auto row: rows)
    {
        locationsTable->removeRow(row);
    }

    changeSomething = true;
}

void osgMainWindow::createGUI()
{   
    this->resize(1200, 800);

    /*   Set Main List for Groups  */
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    

    /*   Window layout   */
    centralwidget = new QWidget(this);

    mainLayout = new QGridLayout(centralwidget);

    groupLayout = new QVBoxLayout();
    tableLayout = new QVBoxLayout();
    
    

    osgWidget = new QWidget();
    osgLayout = new QGridLayout(osgWidget); // a special layout only for the qtosgwidget
    
    #pragma region /*     Menu Actions    */
        actionOpen = new QAction(tr("Load Project"), this);
        connect(actionOpen, SIGNAL(triggered()), this, SLOT(openOsg()));

        actionNew = new QAction(tr("Load OSG Model"), this);
        connect(actionNew, SIGNAL(triggered()), this, SLOT(newProject()));
        
        actionSave = new QAction(tr("Save Project"), this);
        connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
        
        actionExit = new QAction(tr("Exit"), this);
        connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

        actionHelp = new QAction(tr("Help"), this);
        connect(actionHelp, SIGNAL(triggered()), this, SLOT(help()));
    #pragma endregion

    #pragma region /* Menu Bar    */
        menuFile = menuBar()->addMenu(QStringLiteral("File"));
        menuHelp = menuBar()->addMenu(QStringLiteral("Help"));
        
        menuBar()->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionNew);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        
        menuBar()->addAction(menuHelp->menuAction());
        menuHelp->addAction(actionHelp);
    #pragma endregion

    #pragma region /* osg Meny */

    #pragma region add widgets to grid layout
        mainLayout->addLayout(groupLayout, 0, 0, 120, 40);
        mainLayout->addLayout(tableLayout, 0, 40 + 120 + 50, 0, 90);

        mainLayout->addWidget(osgWidget, 0, 40, 120, 160);
    #pragma endregion


    centralwidget->setLayout(mainLayout);
    this->setCentralWidget(centralwidget); // * set the central widget of the main window to the widget with the main layout

    QMetaObject::connectSlotsByName(this);
}

void osgMainWindow::retranslateUi()
{
    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
    actionOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
    actionNew->setText(QApplication::translate("MainWindow", "New", Q_NULLPTR));
    actionSave->setText(QApplication::translate("MainWindow", "Save", Q_NULLPTR));
    actionExit->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
    menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
}

void osgMainWindow::onSelectedGroup(QListWidgetItem* group) 
{
    selectedGroup = group;

    delete this->locationsTable;
    
    initLocationTable();
    

    auto groupStruct = qtOsgWidget->getGroupLocation(group->text().toStdString());

    for (auto const& [markerName, markerData] : groupStruct.groupLocations)
    {
        addMarker(this->locationsTable, markerName, std::to_string(markerData.x), std::to_string(markerData.y), std::to_string(markerData.z));
    }


    QPalette pal = this->colorButton->palette();
    pal.setColor(QPalette::Button, QColor(groupStruct.groupColor[0]*RGB_MUL, groupStruct.groupColor[1]*RGB_MUL, groupStruct.groupColor[2]*RGB_MUL, groupStruct.groupColor[3]*RGB_MUL));
    this->colorButton->setAutoFillBackground(true);    
    this->colorButton->setPalette(pal);
    this->colorButton->update();

    qDebug() << "NOT CRASH L)";

    onUpdateRoutesOsg();
}

void osgMainWindow::newProject()
{
    
    osgName = QFileDialog::getOpenFileName(this, "Open Osg File", "./", "open file (*.*)").toStdString();

    if (osgName == "")
    {
        return;
    }

    loadGUI();

    if (qtOsgWidget->loadScene(osgName) == false)
    {
        QMessageBox msgBox;
        msgBox.setText("Can't load the osg file.");
        msgBox.exec();
    }

    modelNameLabel->setText(QString::fromStdString(osgName));
}

void osgMainWindow::openOsg()
{
    osgName = QFileDialog::getOpenFileName(this, "Open Osg tool File", "./", "osgtool file (*.osgtool)").toStdString();
    if (osgName == "")
    {
        qDebug() << "not selected";
        return;
    }

    loadGUI();
    
    if (qtOsgWidget->load(osgName) == false)
    {
        onUnloadProject();
        QMessageBox msgBox;
        msgBox.setText("Can't load the project save.");
        msgBox.exec();
        return;
    }

    modelNameLabel->setText(QString::fromStdString(qtOsgWidget->osgtoolData.modelNameLabel));

    for (auto& [groupName, group] : qtOsgWidget->osgtoolData.groupsDB)
    {
        addGroupToGui(groupName);  
    }
}

void osgMainWindow::changeTableCell(std::string name, QTableWidget* locationTable, std::string value, int row, int column)
{
    NamedQTableWidgetItem *item = new NamedQTableWidgetItem();
    item->name = name;
        
    item->setText(QString::fromStdString(value));
    if (locationTable->item(row, column) != nullptr)
    {
        delete locationTable->item(row, column);
    }
    locationTable->setItem(row, column, item);

}


void osgMainWindow::addMarker(QTableWidget* locationTable, std::string markName, std::string xStr, std::string yStr, std::string zStr) 
{
    disconnect(locationsTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(onTableCellChange(QTableWidgetItem *)));
    
    int newRowIndex = locationTable->rowCount();
    locationTable->setRowCount(newRowIndex + 1);

    changeTableCell(markName, locationTable, xStr, newRowIndex, 0);
    changeTableCell(markName, locationTable, yStr, newRowIndex, 1);
    changeTableCell(markName, locationTable, zStr, newRowIndex, 2);
    
    connect(locationsTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(onTableCellChange(QTableWidgetItem *)));
}

void osgMainWindow::changeMarker(QTableWidgetItem* locationTableItem, std::string markName, std::string xStr, std::string yStr, std::string zStr) 
{
    disconnect(locationsTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(onTableCellChange(QTableWidgetItem *)));

    auto locationTable = locationTableItem->tableWidget();
    auto rowIndex = locationTableItem->row();

    changeTableCell(markName, locationTable, xStr, rowIndex, 0);
    changeTableCell(markName, locationTable, yStr, rowIndex, 1);
    changeTableCell(markName, locationTable, zStr, rowIndex, 2);

    connect(locationsTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(onTableCellChange(QTableWidgetItem *)));
}

std::string osgMainWindow::itemToName(QTableWidgetItem* item) 
{
    auto namedItem = (NamedQTableWidgetItem*)item;
    return namedItem->name;
}

QListWidgetItem* osgMainWindow::addGroupToGui(std::string groupName) 
{
    QListWidgetItem* newItem = new QListWidgetItem(groupName.c_str());
        
    groupList->addItem(newItem);

    return newItem;
}

bool osgMainWindow::isOnViewMode() {
    bool isNotOnViewMode = viewModeCheck->checkState() == Qt::CheckState::Unchecked;
    return !isNotOnViewMode;
}

bool osgMainWindow::isOnShowRouteMode() 
{
    bool isNotOnViewMode = showRouteCheck->checkState() == Qt::CheckState::Unchecked;
    return !isNotOnViewMode;
}

void osgMainWindow::exportGroups(std::string path, std::vector<std::string> groups) 
{
    std::ofstream myFile;
    myFile.open(path);
    for (auto& groupName : groups)
    {
        std::map<int, MarkerLocation*> orderMarkers;
        for (auto& [name, marker] : qtOsgWidget->getGroupLocation(groupName).groupLocations)
        {
            int asIndex = std::stoi(name);
            orderMarkers[asIndex] = &marker;
        }
        
        for (auto const& [index, markerPtr] : orderMarkers)
        {
            myFile << std::to_string(markerPtr->x) << ",";
            myFile << std::to_string(markerPtr->y) << ",";
            myFile << std::to_string(markerPtr->z) << ",";
            myFile << "\n";
        }
    }
    
    myFile.close();
}

void osgMainWindow::loadGUI() 
{
    if (projectLoaded)
    {
        onUnloadProject();
    }

    groupList = new QListWidget();
    connect(groupList, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(onSelectedGroup(QListWidgetItem *)));
    groupLayout->addWidget(groupList);

    projectLoaded = true;

    modelNameLabel = new QLabel();
    groupLayout->addWidget(modelNameLabel);
    
    colorButton = new QPushButton();
    connect(colorButton, &QPushButton::pressed, this, &osgMainWindow::updateColor);
    QPalette pal = colorButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::gray));
    colorButton->setAutoFillBackground(true);
    colorButton->setPalette(pal);
    colorButton->update();
    groupLayout->addWidget(colorButton); 


    addGroup = new QPushButton(tr("Add Group"));
    connect(addGroup, SIGNAL(clicked()), this, SLOT(createGroup()));
    groupLayout->addWidget(addGroup);
    addGroup->setText(QApplication::translate("MainWindow", "Add Group", Q_NULLPTR));

    delMarkButton = new QPushButton(tr("Delete Selected Marks"));
    connect(delMarkButton, SIGNAL(clicked()), this, SLOT(deleteMarker()));

    groupLayout->addWidget(delMarkButton);

    delGroupButton = new QPushButton(tr("Delete Group"));
    
    connect(delGroupButton, SIGNAL(clicked()), this, SLOT(deleteGroup()));

    groupLayout->addWidget(delGroupButton);


    exportGroupButton = new QPushButton(tr("Export Selected Group"));
    groupLayout->addWidget(exportGroupButton);
    exportAllButton = new QPushButton(tr("Export All"));
    groupLayout->addWidget(exportAllButton);

    connect(exportGroupButton, SIGNAL(clicked()), this, SLOT(exportGroup()));
    connect(exportAllButton, SIGNAL(clicked()), this, SLOT(exportAll()));
    
    viewModeCheck = new QCheckBox(tr("View mode"));
    groupLayout->addWidget(viewModeCheck);
    showRouteCheck = new QCheckBox(tr("Show route between selected markers"));
    connect(showRouteCheck, SIGNAL(stateChanged(int)), this, SLOT(onUpdateShowRoutes(int)));
    groupLayout->addWidget(showRouteCheck);

    if (qtOsgWidget)
    {
        delete qtOsgWidget;
    }

    qtOsgWidget = new QtOSGWidget(osgWidget);

    OSGOpen = true;

    connect(qtOsgWidget, &QtOSGWidget::activateUpdateLocation, this, &osgMainWindow::onPickOsg);
    
    osgLayout->addWidget(qtOsgWidget, 0,40);

    initLocationTable();
}

bool osgMainWindow::userSureToExit() 
{
    QMessageBox msgBox;
    msgBox.setText("The project has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Save)
    {
        save();
        bool notSave = changeSomething;
        if (notSave)
        {
            QMessageBox msgBox;
            msgBox.setText("The project is not saved!!!");
            msgBox.exec();
            return false;
        }
        return true;
    }
    else if (ret == QMessageBox::Discard)
    {
        return true;
    }
    else if (ret == QMessageBox::Cancel)
    {
        return false;
    }
    qDebug() << "error";

    return false;
}


void osgMainWindow::onPickOsg(double x, double y, double z)
{
    if (isOnViewMode())
    {
        return;
    }

    if (this->selectedGroup == nullptr)
    {
        return;
    }

    std::string groupName = this->selectedGroup->text().toStdString();

    QList<QTableWidgetItem*> selectedMarks = locationsTable->selectedItems();    

    bool needToMakeNewMark = selectedMarks.count() == 0;

    QColor color = colorButton->palette().button().color();

    if (needToMakeNewMark)
    {
        std::string markName = qtOsgWidget->getGroupLocationName(groupName);
        
        if (qtOsgWidget != NULL)
            qtOsgWidget->updateOrAddLocationApi(groupName, MarkerLocation(x, y, z), markName,
                                        color.red(), color.green(), color.blue());

        addMarker(this->locationsTable, markName, std::to_string(x), std::to_string(y), std::to_string(z));
    }
    else
    {
        struct MarkNameTuple
        {
            QTableWidgetItem* mark;
            std::string name;
        };
        
        std::set<std::string> alreadyMovedMarkers;
        std::vector<MarkNameTuple> changeList;
        qDebug() << selectedMarks.count();
        for (QTableWidgetItem* item : selectedMarks)
        {
            if (item == nullptr)
            {
                qDebug() << "WTF";
                continue;
            }
            std::string mark = itemToName(item);
            qDebug() << "std::string mark = itemToName(item): " << mark.c_str();
            bool alreadyMoved = alreadyMovedMarkers.find(mark) != alreadyMovedMarkers.end();
            if (alreadyMoved)
            {
                qDebug() << "alreadyMoved";
                continue;
            }

            alreadyMovedMarkers.insert(mark);

            if (qtOsgWidget != NULL)
                qtOsgWidget->updateOrAddLocationApi(groupName, MarkerLocation(x, y, z), mark,
                                            color.red(), color.green(), color.blue());

            changeList.push_back({item, mark});
            
        }

        for (auto& item : changeList)
        {
            changeMarker(item.mark, item.name, std::to_string(x), std::to_string(y), std::to_string(z));
        }
    }

    changeSomething = true;

    onUpdateRoutesOsg();
}


void osgMainWindow::onTableCellChange(QTableWidgetItem * current) 
{
    
    int row = current->row();
    int column = current->column();
    std::string groupName = this->selectedGroup->text().toStdString();
    std::string markName = itemToName(current);

    std::string xStr = this->locationsTable->item(row, 0)->text().toStdString();
    std::string yStr = this->locationsTable->item(row, 1)->text().toStdString();
    std::string zStr = this->locationsTable->item(row, 2)->text().toStdString();

    double x;
    double y;
    double z;
    try
    {
        x = stod(xStr);
        y = stod(yStr);
        z = stod(zStr);
    }
    catch(...)
    {
        qDebug() << "can't read numbers";
        return;
    }
    changeSomething = true;

    
    MarkerLocation location = qtOsgWidget->getLocationApi(groupName, markName);
    location.x = x;
    location.y = y;
    location.z = z;
    qtOsgWidget->updateOrAddLocationApi(groupName, location, markName);
}

void osgMainWindow::onUpdateShowRoutes(int newValue)
{
    onUpdateRoutesOsg();
} 

void osgMainWindow::onUpdateRoutesOsg() 
{
    if (isOnShowRouteMode())
    {
        if (this->selectedGroup == nullptr)
        {
            return;
        }
        std::string groupName = this->selectedGroup->text().toStdString();

        qtOsgWidget->drawRoutes(groupName);
    }
    else
    {
        qtOsgWidget->clearRoutes();
    }
}

QString osgMainWindow::getSelectedMarkMenuFieldText() 
{
    // get the text
    return getSelectetMarkMenuField()->text();
}

QListWidgetItem* osgMainWindow::getSelectetMarkMenuField() {
    
    // TODO:
    return nullptr;
}

void osgMainWindow::updateColor()
{    
    if (selectedGroup == nullptr)
    {
        return;
    }

    QColor color = QColorDialog::getColor();
    if(color.isValid())
    {
        QPalette pal = this->colorButton->palette();
        pal.setColor(QPalette::Button, color);
        this->colorButton->setAutoFillBackground(true);
        this->colorButton->setPalette(pal);
        this->colorButton->update();
        qtOsgWidget->changeGroupColorApi(selectedGroup->text().toStdString(), (float)color.redF(), (float)color.greenF(), (float)color.blueF());
        onUpdateRoutesOsg();

        changeSomething = true;
    }
    else
        qDebug() << "Error";
}

std::string osgMainWindow::getNameFromGroupSpoiler(Spoiler* GroupSpoiler) {
    return GroupSpoiler->objectName().toStdString();
}

void osgMainWindow::exportGroup()
{

    if (this->selectedGroup == nullptr)
    {
        return;
    }
    QMainWindow* window = new QMainWindow();

    std::string fname = QFileDialog::getSaveFileName(window, "save group", ".csv", "Excel (*.csv *.xls)").toStdString();

    delete window;

    if (fname == "")
    {
        return;
    }
    std::vector<std::string> groupVector;
    groupVector.push_back(this->selectedGroup->text().toStdString());


    // TODO index: 
    // sort the group by index befor sending it to the api
    // api sort the markers of the groups (of each group) by index (parse index from the name)
    // make the space of the name empty (make the table smaller)
    exportGroups(fname, groupVector);
}

void osgMainWindow::exportAll()
{
    if (this->selectedGroup == nullptr)
    {
        return;
    }
    QMainWindow* window = new QMainWindow();

    std::string fname = QFileDialog::getSaveFileName(window, "save group", ".csv", "Excel (*.csv *.xls)").toStdString();

    delete window;

    if (fname == "")
    {
        return;
    }
    std::vector<std::string> groupVector;
    for (size_t i = 0; i < this->groupList->count(); i++)
    {
        groupVector.push_back(this->groupList->item(i)->text().toStdString());   
    }
    
    exportGroups(fname, groupVector);

    return;
}

void osgMainWindow::createGroup()
{
    std::string newGroupName = qtOsgWidget->addGroupName();

    addGroupToGui(newGroupName);

    changeSomething = true;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    osgMainWindow* mainWindow = new osgMainWindow();
    mainWindow->show();
    app.exec();
    return 0;
}

void osgMainWindow::initLocationTable() 
{
    locationsTable = new QTableWidget();

    connect(locationsTable, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectedMark()));
    connect(locationsTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(onTableCellChange(QTableWidgetItem *)));
    
    locationsTable->setColumnCount(3);
    locationsTable->setHorizontalHeaderLabels(QStringList() << "x" << "y" << "z");
    
    locationsTable->setObjectName("markList");

    
    tableLayout->addWidget(locationsTable);
}

void osgMainWindow::onSelectedMark()
{
    // onUpdateRoutesOsg();
}