#ifndef __WASTELEVELSENSOR__
#define __WASTELEVELSENSOR__

#include "devices/sensors/Sensor.hpp"

class WasteLevelSensor : public Sensor<float> {
   public:
    bool init() override;
    float detect() override;

   private:
    const float temperature = 20;
    const float vs = 331.45 + 0.62 * temperature;
};

#endif