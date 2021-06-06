#include "OsgMenu.h"

void OsgMenu::onCheckShowRoute(int a)
{
    this->onPressShowRoute();
}

OsgMenu::OsgMenu(std::function<void()> onPressShowRoute) : QGroupBox(tr("Osg Menu"))
{
    this->onPressShowRoute = onPressShowRoute;
    /* Create layout for widgets */
    layout = new QVBoxLayout();

    /* create widgets */
    viewModeCheck = new QCheckBox(tr("view mode"));
    showRouteCheck = new QCheckBox(tr("show route between selected markers"));
    modelNameLabel = new QLabel("model name");
    instructionText = new QPlainTextEdit();

    instructionText->setReadOnly(true);


    /* add widgets to layout */
    layout->addWidget(modelNameLabel);
    layout->addWidget(viewModeCheck);
    layout->addWidget(showRouteCheck);
    layout->addWidget(instructionText);

    this->setLayout(layout);

    connect(showRouteCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckShowRoute(int)));

}

bool OsgMenu::isOnViewMode() {
    bool isNotOnViewMode = viewModeCheck->checkState() == Qt::CheckState::Unchecked;
    return !isNotOnViewMode;
}

bool OsgMenu::isOnShowRouteMode() {
    bool isNotOnViewMode = showRouteCheck->checkState() == Qt::CheckState::Unchecked;
    return !isNotOnViewMode;
}