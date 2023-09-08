#ifndef COPTER_COM__
#define COPTER_COM__
#include <Arduino.h>
#include "Copter.h"
class Component {
public:
    Component(Copter *copter);    
    void log(String msg);
protected:
    Copter *copter;
};
#endif
