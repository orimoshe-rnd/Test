#include "IInputCallbacks.h"

class View {
    IInputCallbacks* inputCallbacks;

    public:
        void setInputCallback(IInputCallbacks* inputCallbacks);
};