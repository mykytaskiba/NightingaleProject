#include "pch.h"
#include "logger.h"


vector<string> Logger::LOG_STACK = vector<string>();
LoggerStatus Logger::CURRENT_STATUS = LoggerStatus::LOG_DISABLED;

void Logger::BeginSection(string const& string)
{
    if (CURRENT_STATUS == LoggerStatus::LOG_DISABLED) {
        return;
    }
    LOG_STACK.push_back(string);
}

void Logger::Log(string const& string)
{
    if (CURRENT_STATUS == LoggerStatus::LOG_DISABLED) {
        return;
    }

    std::cout << string << std::endl;
}

void Logger::EndSection(string const& string)
{
}

void Logger::Reset()
{
    
}

void Logger::SetLoggingStatus(LoggerStatus status)
{
    CURRENT_STATUS = status;
    //if (CURRENT_STATUS != )
}

