#ifndef __WASTETEMPERATURESENSOR__
#define __WASTETEMPERATURESENSOR__

#include "devices/sensors/Sensor.hpp"

class WasteTemperatureSensor : public Sensor<float> {
   public:
    bool init() override;
    float detect() override;
};

#endif