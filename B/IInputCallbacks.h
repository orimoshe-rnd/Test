#include <string>
#include <vector>


 class IInputCallbacks {
public:
    // What need to enter to the logic
    virtual void loadProject(std::string projectPath) = 0;
    virtual void saveProject(std::string projectPath) = 0;
    virtual void loadModel(std::string projectPath) = 0;
    virtual void addGroup() = 0;
    virtual void selectGroup(int index) = 0;
    virtual void removeGroup() = 0;
    virtual void addMarker(Position position) = 0;
    virtual void changeMarkPos(Position newPosition) = 0;
    virtual void selectMarkers(std::vector<Marker> selectedMarkers) = 0;
    virtual void deleteMarkers() = 0;
    virtual void exportGroup() = 0;
    virtual void exportAll() = 0;
    virtual void changeColor() = 0;
    virtual void clickOnScene(Position position) = 0;
    virtual void changeMarkerPos(Marker marker, Position newPosition) = 0;
    virtual void viewMode(bool enable) = 0;
    virtual void routeMode(bool enable) = 0;
}; 