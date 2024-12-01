#ifndef __USERDETECTIONSENSOR__
#define __USERDETECTIONSENSOR__

#include "devices/sensors/Sensor.hpp"

class UserDetectionSensor : public Sensor<bool> {
   public:
    bool init() override;
    bool detect() override;
};

#endif