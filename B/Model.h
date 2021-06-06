#include "IOutputCallbacks.h"

class Model {
    IOutputCallbacks* outputCallbacks;

    public:
        void setOutputCallbacks(IOutputCallbacks* outputCallbacks);
};