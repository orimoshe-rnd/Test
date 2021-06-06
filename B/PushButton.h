#pragma once
#include <QPushButton>
#include <QMouseEvent>
#include <Qt>

class PushButton : public QPushButton
{
    Q_OBJECT

    public:
        PushButton(QWidget *parent = NULL) : QPushButton(parent){}

        virtual void mousePressEvent(QMouseEvent * event)
        {
            QPushButton::mousePressEvent(event);
            if(event->button() == Qt::RightButton)
                emit rightClick();
        }

    signals:
        void rightClick();

};