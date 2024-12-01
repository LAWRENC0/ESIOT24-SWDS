#ifndef __SENSOR__
#define __SENSOR__

template <typename T>
class Sensor {
   public:
    virtual bool init() = 0;
    virtual T detect() = 0;
};

#endif