#include "kernel/LoggerService.hpp"
#include "kernel/MsgService.hpp"

LoggerService Logger;

void LoggerService::log(const String& msg) { MsgService.sendMsg(logger_prefix + msg); }