#pragma once

#include <vector>
#include "defines.h"

enum class LoggerStatus {
    LOG_DISABLED,
    LOG_LITE,
    LOG_FULL
};

static class Logger {
private:
    static vector<string> LOG_STACK;
    static LoggerStatus CURRENT_STATUS;

public:
    static void BeginSection(string const& string);

    static void Log(string const& string);

    static void EndSection(string const& string);

    static void Reset();

    static void SetLoggingStatus(LoggerStatus status);

    
};