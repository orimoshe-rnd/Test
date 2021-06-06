
class IOutputCallbacks {
    public:
    // What need to change in GUI?
    virtual void ProjectLoaded() = 0;
    virtual void ProjectUnloaded() = 0;
    virtual void showedModel(std::string projectPath) = 0;
    virtual void addedGroup(int index) = 0;
    virtual void groupSelected(int index) = 0;
    virtual void groupUnselected(int index) = 0;
    virtual void removedGroup(int index) = 0;

    virtual void selectedMarkers(std::vector<Marker> selectedMarkers) = 0;
    virtual void deletedMarkers(std::vector<Marker> selectedMarkers) = 0;
    virtual void exportedGroup(Group selectedMarkers) = 0;
    virtual void exportedAll() = 0;
    virtual void changedGroupColor(Color newColor) = 0;
    virtual void changedMarksPostions(std::vector<Marker> selectedMarkers, Position newPosition) = 0;
    virtual void showMarkOnScene(Position position) = 0;
    virtual void changedMarkerPos(Marker marker, Position position) = 0;
    virtual void changedToViewMode(bool enable) = 0;
    virtual void changedTorouteMode(bool enable) = 0;
};
