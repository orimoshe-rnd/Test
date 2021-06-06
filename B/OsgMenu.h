#pragma once
#include <functional>

#include <QGroupBox>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QMainWindow>
#include <QApplication>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QDebug>


class OsgMenu : public QGroupBox
{
    Q_OBJECT
    public:
        OsgMenu(std::function<void()> onPressshowRoute);
        QVBoxLayout* layout;
        bool isOnViewMode();   
        bool isOnShowRouteMode();   

    private:
        QCheckBox* viewModeCheck;
        QCheckBox* showRouteCheck;
        QLabel *modelNameLabel;
        QPlainTextEdit *instructionText;
        std::function<void()> onPressShowRoute;
        
    private slots:
        void onCheckShowRoute(int a);


};