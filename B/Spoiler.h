#pragma once
#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
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
#include <QCheckBox>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

#include "PushButton.h"

class Spoiler : public QWidget {
    Q_OBJECT
private:
    QGridLayout mainLayout;
      
    QToolButton toggleButton;
    QFrame headerLine;
    QParallelAnimationGroup toggleAnimation;
    QScrollArea contentArea;
    int animationDuration{300};

private slots:
    void changeState();

public:
    bool state;
    PushButton* button;
    QCheckBox* check;
    QLayout* layout;
    explicit Spoiler(const QString & title = "", const int animationDuration = 300, QWidget *parent = 0);
    void setContentLayout(QLayout & contentLayout);
};