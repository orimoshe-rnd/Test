#include "qtOsgWidget.h"

bool QtOSGWidget::load(std::string path)
{
    std::ifstream i(path);
    json j;
    i >> j;
    j.get_to(this->osgtoolData);

    if (this->loadScene(this->osgtoolData.modelNameLabel) == false)
    {
        return false;
    }

    for (auto &[groupName, group] : this->osgtoolData.groupsDB)
    {
        markersParent->addChild(group.gameObjects);

        for (auto &[markerName, marker] : group.groupLocations)
        {
            marker.imageAxis = createImageModel(osg::Vec3(marker.x, marker.y, marker.z), osg::Vec4(group.groupColor[0], group.groupColor[1], group.groupColor[2], group.groupColor[3]), imageWaypoint);

            group.gameObjects->addChild(marker.imageAxis);
        }
    }
    return true;
}

void QtOSGWidget::save(std::string path)
{
    json j_map(this->osgtoolData);

    std::string s = j_map.dump();
    std::ofstream o(path);
    o << s << std::endl;
}

// FirstPersonController.cpp
const float moveSpeed = 0.6;
const float inputTimeInterval = 0.02;
osg::Vec3d tempMov;
double maxTick = inputTimeInterval;

void FirstPersonController::keyReleased(char keyPressed)
{
    // Set the viewer's "eye" position, which is located at the center of the camera.
    osg::Vec3d eyePos;
    osg::Matrix matrix = _viewer->getCameraManipulator()->getMatrix();
    eyePos = matrix.getTrans();
    osg::Quat camRotation = matrix.getRotate();
    
    // Movement of the camera correlates with W, A, S, D
    switch(keyPressed)
    {
        case 'w':
            tempMov.z() = -moveSpeed;

            break;
        case 'a':
            tempMov.x() = -moveSpeed;

            break;
        case 's':
            tempMov.z() = moveSpeed;

            break;
        case 'd':
            tempMov.x() = moveSpeed;

            break;
        default:
            break;
    }
            
    eyePos += camRotation * tempMov;
    matrix.setTrans(eyePos);
        // Check [mainTimer.time % (time divisor) == 0]
    if (mainTimer.time_s() >= maxTick)
    {
        _viewer->getCameraManipulator()->setByMatrix(matrix);
        maxTick += inputTimeInterval;
    }

	switch (keyPressed)
	{
	case 'w':
		tempMov.set(0, 0, 0);
		break;
	case 'a':
		tempMov.set(0, 0, 0);
		break;
	case 's':
		tempMov.set(0, 0, 0);
		break;
	case 'd':
		tempMov.set(0, 0, 0);
		break;
	default:
		break;
	}
}


bool FirstPersonController::performMovement()
{
    // return if less then two events have been added
    if( _ga_t0.get() == NULL || _ga_t1.get() == NULL )
    {
        return false;
    }
    // get delta time, throw warning if error with delta time
    double eventTimeDelta = _ga_t0->getTime() - _ga_t1->getTime();
    if( eventTimeDelta < 0. )
    {
        OSG_WARN << "Manipulator warning: eventTimeDelta = " << eventTimeDelta << std::endl;
        eventTimeDelta = 0.;
    }
    // get deltaX and deltaY
    float dx = _ga_t0->getXnormalized() - _ga_t1->getXnormalized();
    float dy = _ga_t0->getYnormalized() - _ga_t1->getYnormalized();
    std::cout << dx << ", " << dy << "\n";
    // return if there is no movement.
    if( dx == 0.0 && dy == 0.0 )
    {
        return false;
    }
	performMouseDeltaMovement(dx, dy);
    // call appropriate methods -- se chama um dos de baixo, apenas duplica (i.e. acelera) o movimento já feito na linha acima
    /*unsigned int buttonMask = _ga_t1->getButtonMask();
    if( buttonMask == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON )
    {
        return performMovementLeftMouseButton( eventTimeDelta, dx, dy );
    }
    else if( buttonMask == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON ||
            buttonMask == (osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON | osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) )
    {
        return performMovementMiddleMouseButton( eventTimeDelta, dx, dy );
    }
    else if( buttonMask == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON )
    {
        return performMovementRightMouseButton( eventTimeDelta, dx, dy );
    }*/

    return false;
}

bool QtOSGWidget::loadScene(std::string path)
{
    _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

    osg::ref_ptr<osg::Node> scene = osgDB::readRefNodeFile(path);
    
    osgtoolData.modelNameLabel = path;

    markersParent = dynamic_cast<osg::Group *>(scene.get());
    if (!markersParent)
    {
        markersParent = new osg::Group;
        markersParent->addChild(scene.get());
    }

    routesParent = dynamic_cast<osg::Group *>(scene.get());
    if (!routesParent)
    {
        routesParent = new osg::Group;
        routesParent->addChild(scene.get());
    }
    // add the handler for doing the picking
    pickHandle = new PickHandler(markersParent, _mViewer);
    connect(pickHandle, &PickHandler::updateLocation, this, &QtOSGWidget::updateLocation);
    _mViewer->addEventHandler(pickHandle);

    osg::Camera *camera = new osg::Camera;
    camera->setViewport(0, 0, this->width(), this->height());
    camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.f, 1.f));
    float aspectRatio = static_cast<float>(this->width()) / static_cast<float>(this->height());
    camera->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);
    camera->setGraphicsContext(_mGraphicsWindow);

    _mViewer->setCamera(camera);

    _mViewer->setSceneData(markersParent.get());
    _mViewer->setSceneData(routesParent.get());

    // KEYBOARD INPUT
    _mViewer->setCameraManipulator<osgGA::CameraManipulator>((osgGA::CameraManipulator*)controller);

    //osgGA::TrackballManipulator *manipulator = new osgGA::TrackballManipulator;
    //controller->setAllowThrow(true);
     //this->setMouseTracking(true);
    // _mViewer->setCameraManipulator(manipulator);

    _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    _mViewer->realize();

	return scene != nullptr;
}

void QtOSGWidget::changeGroupColorApi(std::string groupName, float r, float g, float b)
{
    auto groupIter = osgtoolData.groupsDB.find(groupName);
    bool groupExist = groupIter != osgtoolData.groupsDB.end();

    if (groupExist)
    {
        auto &group = osgtoolData.groupsDB[groupName];
        auto &groupLocations = group.groupLocations;

        for (auto const &[name, val] : group.groupLocations)
        {
            auto &location = groupLocations[name];

            group.gameObjects->removeChild(location.imageAxis);
            markersParent->removeChild(location.imageAxis);
            
            location.imageAxis = createImageModel(osg::Vec3(location.x, location.y, location.z), osg::Vec4(r, g, b, 1), imageWaypoint);
            group.gameObjects->addChild(location.imageAxis);
            markersParent->addChild(location.imageAxis);
        }
        group.groupColor = std::vector<float>({r, g, b, TRANSPARENT_WAYPOINT});
    }
}

void QtOSGWidget::updateOrAddLocationApi(std::string groupName, MarkerLocation newLocation, std::string LocationName, float r, float g, float b)
{
    auto groupIter = osgtoolData.groupsDB.find(groupName);
    bool groupExist = groupIter != osgtoolData.groupsDB.end();

    auto &group = osgtoolData.groupsDB[groupName];

    if (!groupExist)
    {
        GroupStruct newGroup;

        markersParent->addChild(newGroup.gameObjects);
        group = newGroup;
        group.groupColor = std::vector<float>({r, g, b, TRANSPARENT_WAYPOINT});
    }
    auto locationIter = group.groupLocations.find(LocationName);
    bool locationExist = locationIter != group.groupLocations.end();

    auto &location = group.groupLocations[LocationName];

    if (locationExist)
    {
        group.gameObjects->removeChild(location.imageAxis);
        markersParent->removeChild(location.imageAxis);
    }
    newLocation.imageAxis = createImageModel(osg::Vec3(newLocation.x, newLocation.y, newLocation.z), osg::Vec4(group.groupColor[0], group.groupColor[1], group.groupColor[2], group.groupColor[3]), imageWaypoint);
    markersParent->addChild(newLocation.imageAxis);

    location = newLocation;

    group.gameObjects->addChild(newLocation.imageAxis);
}

void QtOSGWidget::changeLocationName(std::string groupName, std::string oldLocationName, std::string newLocationName)
{
    updateOrAddLocationApi(groupName, osgtoolData.groupsDB[groupName].groupLocations[oldLocationName], newLocationName);

    removeLocationApi(groupName, oldLocationName);
}

void QtOSGWidget::updateOrAddLocationApi(std::string groupName, MarkerLocation newLocation, std::string LocationName)
{
    updateOrAddLocationApi(groupName, newLocation, LocationName, 0, 0, 0);
}

void QtOSGWidget::drawRoutes(const std::string &groupName)
{
    clearRoutes();

    auto groupRef = osgtoolData.groupsDB[groupName];
    auto groupColor = groupRef.groupColor;
    auto groupLocations = groupRef.groupLocations;


    int numOfLocations = groupLocations.size();
    qDebug() << "Num of locations: " << numOfLocations;
    qDebug() << groupColor[0] << groupColor[1] << groupColor[2] << groupColor[3];
    osg::Vec4 colorGroup = osg::Vec4(groupColor[0], groupColor[1], groupColor[2], groupColor[3]);


    std::map<std::string, MarkerLocation>::iterator it = groupLocations.begin();
    bool isEmptyGroup = it == groupLocations.end();
    if (isEmptyGroup)
    {
        return;
    }
    
    while (true)
    {
        auto nextItr = std::next(it);
        bool haveMoreThanOneLeft = nextItr != groupLocations.end();
        if (haveMoreThanOneLeft == false)
        {
            break;
        }

        osg::Vec3 start = getLocationApi(groupName, it->first).getPostion();
        osg::Vec3 end = getLocationApi(groupName, nextItr->first).getPostion();
        qDebug() << "Start: "<< start[0] << start[1] << start[2];
        qDebug() << "End: "<< end[0] << end[1] << end[2];
        osg::Geode *model = createAxisModel(start, end, colorGroup);
        routesParent->addChild(model);
        routesGeode.push_back(model);

        it++;
    }
}

void QtOSGWidget::clearRoutes()
{
    for (auto &route : this->routesGeode)
    {
        routesParent->removeChild(route);
    }

    this->routesGeode.clear();
}

MarkerLocation QtOSGWidget::getLocationApi(std::string groupName, std::string locationName)
{
    return osgtoolData.groupsDB[groupName].groupLocations[locationName];
}

std::string QtOSGWidget::addGroupName()
{
    osgtoolData.lastIndex++;
    std::string newName = std::to_string(osgtoolData.lastIndex);

    auto &group = osgtoolData.groupsDB[newName];

    GroupStruct newGroup;

    markersParent->addChild(newGroup.gameObjects);
    group = newGroup;
    group.groupColor = std::vector<float>({0, 0, 0, 1});

    return newName;
}

void QtOSGWidget::changeLocationNameApi(std::string groupName, std::string oldLocationName, std::string newLocationName)
{
    auto nodeHandler = osgtoolData.groupsDB[groupName].groupLocations.extract(oldLocationName);
    nodeHandler.key() = newLocationName;
    osgtoolData.groupsDB[groupName].groupLocations.insert(std::move(nodeHandler));
}

void QtOSGWidget::changeGroupNameApi(std::string oldGroupName, std::string newGroupName)
{
    auto nodeHandler = osgtoolData.groupsDB.extract(oldGroupName);
    nodeHandler.key() = newGroupName;
    osgtoolData.groupsDB.insert(std::move(nodeHandler));
}

void QtOSGWidget::removeLocationApi(std::string groupName, std::string locationName)
{
    auto markersMap = osgtoolData.groupsDB[groupName].groupLocations;
    auto locationIter = markersMap.find(locationName);
    bool locationExist = locationIter != markersMap.end();
    if (locationExist)
    {
        osgtoolData.groupsDB[groupName].gameObjects->removeChild(markersMap[locationName].imageAxis);
        osgtoolData.groupsDB[groupName].groupLocations.erase(locationName);
        markersParent->removeChild(markersMap[locationName].imageAxis);
        markersMap.erase(locationIter);
    }
}

void QtOSGWidget::removeGroupApi(std::string groupName)
{
    auto groupIt = osgtoolData.groupsDB.find(groupName);

    for(auto it = groupIt->second.groupLocations.begin();it!=groupIt->second.groupLocations.end();it++)
    {
        markersParent->removeChild(it->second.imageAxis);
    }
    
    markersParent->removeChild(groupIt->second.gameObjects);
    
    osgtoolData.groupsDB.erase(groupIt);
}

std::string QtOSGWidget::getGroupLocationName(std::string groupName)
{
    auto &group = osgtoolData.groupsDB[groupName];
    group.lastIndex++;
    std::string name = std::to_string(group.lastIndex);
    return name;
}

GroupStruct QtOSGWidget::getGroupLocation(std::string groupName)
{
    auto groupIter = osgtoolData.groupsDB.find(groupName);
    bool groupExist = groupIter != osgtoolData.groupsDB.end();
    
    auto &group = osgtoolData.groupsDB[groupName];

    if (!groupExist)
    {
        GroupStruct newGroup;

        markersParent->addChild(newGroup.gameObjects);
        group = newGroup;
        group.groupColor = std::vector<float>({0, 0, 0, 1});
    }
    return group;
}