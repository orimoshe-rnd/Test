#include "IOutputCallbacks.h"
#include "IInputCallbacks.h"
#include "View.h"
#include "Model.h"

class Controller : public IOutputCallbacks, IInputCallbacks {
    Model* model;
    View* view;
public:
    Controller(Model* model, View* view);

    // output
     void ProjectLoaded() override;
     void ProjectUnloaded() override;
     void showedModel(std::string projectPath) override;
     void addedGroup(int index) override;
     void groupSelected(int index) override;
     void groupUnselected(int index) override;
     void removedGroup(int index) override;
     void selectedMarkers(std::vector<Marker> selectedMarkers) override;
     void deletedMarkers(std::vector<Marker> selectedMarkers) override;
     void exportedGroup(Group selectedMarkers) override;
     void exportedAll() override;
     void changedGroupColor(Color newColor) override;
     void changedMarksPostions(std::vector<Marker> selectedMarkers, Position newPosition) override;
     void showMarkOnScene(Position position) override;
     void changedMarkerPos(Marker marker, Position position) override;
     void changedToViewMode(bool enable) override;
     void changedTorouteMode(bool enable) override;

    // input
    void loadProject(std::string projectPath) override;
    void saveProject(std::string projectPath) override;
    void loadModel(std::string projectPath) override;
    void addGroup() override;
    void selectGroup(int index) override;
    void removeGroup() override;
    void addMarker(Position position) override;
    void changeMarkPos(Position newPosition) override;
    void selectMarkers(std::vector<Marker> selectedMarkers) override;
    void deleteMarkers() override;
    void exportGroup() override;
    void exportAll() override;
    void changeColor() override;
    void clickOnScene(Position position) override;
    void changeMarkerPos(Marker marker, Position newPosition) override;
    void viewMode(bool enable) override;
    void routeMode(bool enable) override;


};