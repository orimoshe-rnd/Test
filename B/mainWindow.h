
#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdexcept>      // std::invalid_argument
#include <ctime>    

#include <QtCore/QVariant>
#include <QCoreApplication>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QColorDialog>
#include <QSignalMapper>
#include <QListWidget>
#include <QDebug>
#include <QScrollArea>
#include <QFileDialog>
#include <QStringList>
#include <QPoint>
#include <QTimer>
#include <QThread>
#include <QTableWidget>
#include <QMessageBox>
#include <QtGui>
#include <QKeyEvent>

#include "Group.h"
#include "qtOsgWidget.h"
#include "mark.h"
#include "OsgMenu.h"
#include "markMenu.h"

#define RGB_MUL 255

enum MarkMenuField {
    x = 1,
    y,
    z,
    name
};

class NamedQTableWidgetItem : public QTableWidgetItem
{
    public:
        std::string name;
};

class osgMainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        osgMainWindow();
		
        QAction *actionOpen;
        QAction *actionNew;
        QAction *actionSave;
        QAction *actionExit;
        QAction *actionHelp;
        QWidget *centralwidget;
        QPushButton *addGroup;

        QVBoxLayout* groupLayout;
        QListWidget* groupList;
        QListWidgetItem* selectedGroup = nullptr;

        QWidget *osgWidget;
        QGridLayout* osgLayout;
        QVBoxLayout* tableLayout;
        QTableWidget* locationsTable;
        
        MarkMenu* markMenu;
 
        QGridLayout *mainLayout;
        QFrame *frame;
        QFrame *osgFrame;
        QFrame *guiFrame;
        QMenuBar *menubar;
        QMenu *menuFile;
        QMenu *menuHelp;
        QStatusBar *statusbar;
        QThread *thread1;
        bool OSGOpen = false;

        void createGUI();
        void retranslateUi();

        void closeEvent(QCloseEvent *event) override;
        
    protected:
        void keyReleaseEvent(QKeyEvent *event);
        
    private slots:
        void openOsg();
        void newProject();
        void createGroup();
        void deleteMarker();
        void deleteGroup();
        void exportGroup();
        void exportAll();

        QString getSelectedMarkMenuFieldText();
        void onUpdateRoutesOsg();

        void save();
        void exit();
        void help();
        void onUnloadProject();
        void updateColor();
        void onPickOsg(double x, double y, double z);
        void onSelectedMark();
        void onTableCellChange(QTableWidgetItem * location);

        void onUpdateShowRoutes(int newValue);

    
        void onSelectedGroup(QListWidgetItem* group);
    private:
        QListWidgetItem* getSelectetMarkMenuField();
        Spoiler* getSelectedGroupSpoiler();
        std::string getNameFromGroupSpoiler(Spoiler* GroupSpoiler);
        void changeTableCell(std::string name, QTableWidget* locationTable, std::string value, int row, int column);
        QList<QTableWidgetItem*> getSelectedMarker(); 
        void addMarker(QTableWidget* locationTable, std::string markName, std::string xStr, std::string yStr, std::string zStr);
        void changeMarker(QTableWidgetItem* locationTableItem, std::string markName, std::string xStr, std::string yStr, std::string zStr);
        std::string itemToName(QTableWidgetItem* item);
        QListWidgetItem* addGroupToGui(std::string groupName);
        void initLocationTable();
        bool isOnViewMode();
        bool isOnShowRouteMode();
        void exportGroups(std::string path, std::vector<std::string> groups);
        void loadGUI();
        bool userSureToExit();

    private:
        bool projectLoaded;
        bool changeSomething = false;
        std::string osgName;
        std::string instructions;
        QTimer *qtTimer;

    private:
        QPushButton* delMarkButton;
        QPushButton* delGroupButton;
        QPushButton* exportGroupButton;
        QPushButton* exportAllButton;
        QPushButton* colorButton;
        QtOSGWidget* qtOsgWidget;
        QVBoxLayout* downMenu;
        OsgMenu* osgMenu;   
        QCheckBox* viewModeCheck;
        QCheckBox* showRouteCheck;
        QLabel *modelNameLabel;
        QScrollArea* scrollArea;
};
