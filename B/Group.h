#pragma once
#include <stdio.h>

#include <QGroupBox>
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QListWidget>
#include <QPushButton>
#include <QListWidgetItem>
#include <QPoint>
#include <QTableWidget>


#include "Spoiler.h"

class GroupBox : public QObject
{
    Q_OBJECT
    public:
        GroupBox(QListWidget* groupList, QVBoxLayout* groupLayout);
        QVBoxLayout* layout;
        QVBoxLayout* mainLayout;
        QTableWidget* locationsTable;
        QPushButton* color;
        QGroupBox* groupBox;
        Spoiler* spoiler;
        
};
