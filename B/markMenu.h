#pragma once
#include <QGroupBox>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include <QFormLayout>
#include <QPushButton>


class MarkMenu : public QGroupBox
{

    public:
        QLineEdit *name;
        QLabel* label_name;
        QLabel *label_x;
        QLineEdit *x;
        QLabel *label_y;
        QLineEdit *y;
        QLabel *label_z;
        QLineEdit *z;
        QFormLayout* layout;
        QVBoxLayout* buttonLayout;
        QPushButton* delMarkButton;
        QPushButton* delGroupButton;
        QPushButton* exportGroupButton;
        QPushButton* exportAllButton;

        MarkMenu(QWidget* parent);
        MarkMenu(QWidget* parent, QString objectName, double X, double Y, double Z);
        void updateFields(std::string objectName, double x, double y, double z);
        void clearFields();
    private slots:
        

};