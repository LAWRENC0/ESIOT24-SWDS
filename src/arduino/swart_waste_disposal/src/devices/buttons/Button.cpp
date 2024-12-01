#include "devices/buttons/Button.hpp"

Button::Button() {}

void Button::updateSyncTime(long time) { lastTimeSync = time; }

long Button::getLastSyncTime() { return lastTimeSync; }