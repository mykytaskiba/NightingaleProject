#pragma once

#include <vector>
#include <string>

enum class LoggerStatus {
    LOG_DISABLED,
    LOG_LITE,
    LOG_FULL
};

static class Logger {
private:
    static std::vector<std::string> LOG_STACK;
    static LoggerStatus CURRENT_STATUS;

public:
    static void BeginSection(std::string const& string);

    static void Log(std::string const& string);

    static void EndSection(std::string const& string);

    static void Reset();

    static void SetLoggingStatus(LoggerStatus status);

    
};