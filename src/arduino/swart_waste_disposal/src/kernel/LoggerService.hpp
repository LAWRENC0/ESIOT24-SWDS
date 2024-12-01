#ifndef __LOGGER__
#define __LOGGER__

#include "Arduino.h"

class LoggerService {
   public:
    const String logger_prefix = "lo:";
    void log(const String& msg);
};

extern LoggerService Logger;

#endif