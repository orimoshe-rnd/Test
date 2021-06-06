#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDesktopWidget>
#include <QScreen>
#include <QtGlobal>
#include <QWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug> 
#include <QWidget>


#include <osgDB/ReadFile>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osgGA/EventQueue>
#include <osg/io_utils>
#include <osgGA/OrbitManipulator>
#include <osgGA/FirstPersonManipulator>

#include <list>
#include <map>

#include "osgbillboard.h"

#include <nlohmann/json.hpp>

#define TRANSPARENT_WAYPOINT 1

// for convenience
using json = nlohmann::json;

struct MarkerLocation
{
    double x;
    double y;
    double z;
    osg::Geode* axis;
    osg::ref_ptr<osg::Node> imageAxis;
    MarkerLocation(){}

    MarkerLocation(double X, double Y, double Z)
    {
        x = X;
        y = Y;
        z = Z;
    }

    osg::Vec3 getPostion() {
        return osg::Vec3(x, y, z);
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(MarkerLocation, x, y, z);
};

struct GroupStruct
{
    std::map<std::string, MarkerLocation> groupLocations;
    std::vector<float> groupColor;
    osg::ref_ptr<osg::Group> gameObjects;
    int lastIndex = -1;

    GroupStruct() {
        this->gameObjects = new osg::Group();
    }

    //TODO: need?
    // ~GroupStruct() {
    //     delete this->gameObjects;
    // }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GroupStruct, lastIndex, groupLocations, groupColor);
};

struct OsgToolData {
    std::map<std::string, GroupStruct> groupsDB;
    int lastIndex = -1;
    std::string modelNameLabel;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OsgToolData, groupsDB, lastIndex, modelNameLabel);
};

class PickHandler : public QObject, public osgGA::GUIEventHandler
{
    Q_OBJECT
    public:
        

        PickHandler(osg::Group* group, osg::ref_ptr<osgViewer::Viewer> modelView) : QObject()
        {
            this->group = group;
            this->modelView = modelView;
        }

        bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
        {
            switch (ea.getEventType())
            {
                case(osgGA::GUIEventAdapter::RELEASE):
                {
                    osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa); 
                    if (view == this->modelView) 
                    {
                        pick(view, ea);
                    }
                        
                    return false;
                }
                default:
                    return false;
                    
            }
        }

        virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
        {
            osgUtil::LineSegmentIntersector::Intersections intersections;

            if (view->computeIntersections(ea, intersections))
            {
                osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
                if (hitr != intersections.end())
                {
                    const auto& point = hitr->getLocalIntersectPoint();
                    emit updateLocation(point.x(), point.y(), point.z());
                }
            }
            else
                qDebug() << "no intersection";
        }


    signals:
        void updateLocation(double x, double y, double z);

    
    protected:
        osg::Group* group;
        osg::ref_ptr<osgViewer::Viewer> modelView;


};

// FirstPersonController.h
class FirstPersonController : public QWidget, osgGA::OrbitManipulator
{
public:
    FirstPersonController(osgViewer::Viewer *inputViewer) : _viewer(inputViewer)
    {
        // Start frame timer
        mainTimer.setStartTick();
        setAllowThrow(false); // não continua movendo ao soltar o botão do mouse
    }
    virtual bool performMovement();

    void keyReleased(char keyReleased);

protected:
   osgViewer::Viewer *_viewer;
   osg::Timer mainTimer;
};

class QtOSGWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
        QtOSGWidget(QWidget* parent = 0) : QOpenGLWidget(parent)
            , _mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded( this->x(), this->y(), this->width(), this->height() ) )
            , _mViewer(new osgViewer::Viewer)
            , m_scale(QApplication::desktop()->devicePixelRatio()) // take care of HDPI screen, e.g. Retina display on Mac
        {
            controller = new FirstPersonController(_mViewer);
        }

        bool load(std::string path);
        void save(std::string path);
        bool loadScene(std::string path);        

        void changeGroupColorApi(std::string groupName, float r, float g, float b);
       
        void updateOrAddLocationApi(std::string groupName, MarkerLocation newLocation, std::string LocationName, float r, float g, float b);
        void updateOrAddLocationApi(std::string groupName, MarkerLocation newLocation, std::string LocationName);
        
        void changeLocationName(std::string groupName, std::string oldLocationName, std::string newLocationName);
        
        void drawRoutes(const std::string& groupName);
        void clearRoutes();
        
        MarkerLocation getLocationApi(std::string groupName, std::string LocationName);
        MarkerLocation addMarker(std::string groupName);
        
        std::string addGroupName();
        std::string getGroupLocationName(std::string groupName);
        GroupStruct getGroupLocation(std::string groupName);
        
        void changeLocationNameApi(std::string groupName, std::string oldLocationName, std::string newLocationName);
        void changeGroupNameApi(std::string oldGroupName, std::string newGroupName);
        void removeLocationApi(std::string groupName, std::string LocationName);
        void removeGroupApi(std::string groupName);

    
        virtual ~QtOSGWidget(){}

    protected:

        virtual void paintGL() {
            _mViewer->frame();
        }

        virtual void resizeGL( int width, int height ) 
        {
            this->getEventQueue()->windowResize(this->x()*m_scale, this->y() * m_scale, width*m_scale, height*m_scale);
            _mGraphicsWindow->resized(this->x()*m_scale, this->y() * m_scale, width*m_scale, height*m_scale);
            osg::Camera* camera = _mViewer->getCamera();
            camera->setViewport(0, 0, this->width()*m_scale, this->height()* m_scale);
        }
        
        virtual void mouseMoveEvent(QMouseEvent* event) 
        {
            this->getEventQueue()->mouseMotion(event->x()*m_scale, event->y()*m_scale);
            this->update();
        }

        virtual void mousePressEvent(QMouseEvent* event) 
        {
            unsigned int button = 0;
            switch (event->button()){
            case Qt::LeftButton:
                button = 1;
                break;
            case Qt::MiddleButton:
                button = 2;
                break;
            case Qt::RightButton:
                button = 3;
                break;
            default:
                break;
            }
            this->getEventQueue()->mouseButtonPress(event->x()*m_scale, event->y()*m_scale, button);
            this->update();
        }

        virtual void mouseReleaseEvent(QMouseEvent* event) 
        {
            unsigned int button = 0;
            switch (event->button()){
            case Qt::LeftButton:
                button = 1;
                break;
            case Qt::MiddleButton:
                button = 2;
                break;
            case Qt::RightButton:
                button = 3;
                break;
            default:
                break;
            }
            this->getEventQueue()->mouseButtonRelease(event->x()*m_scale, event->y()*m_scale, button);
            this->update();
        }
        
        virtual void wheelEvent(QWheelEvent* event) 
        {
            int delta = event->delta();
            osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
                        osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
            this->getEventQueue()->mouseScroll(motion);
            this->update();
        }

    public:
        FirstPersonController* controller;  // control the osg

    private slots:
        void updateLocation(double x, double y, double z)
        {
            emit activateUpdateLocation(x,y,z);
        }

    signals:
        void activateUpdateLocation(double x, double y, double z);

    public:
        OsgToolData osgtoolData;
        
    private:
        osg::ref_ptr<osg::Group> routesParent;
        std::vector<osg::Geode*> routesGeode;
        osg::ref_ptr<osg::Group> markersParent;
        PickHandler* pickHandle;
        osg::Image* imageWaypoint;
        
    
        osgGA::EventQueue* getEventQueue() const {
            osgGA::EventQueue* eventQueue = _mGraphicsWindow->getEventQueue();
            return eventQueue;
        }

        osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _mGraphicsWindow;
        osgViewer::Viewer* _mViewer;
        qreal m_scale;
};

