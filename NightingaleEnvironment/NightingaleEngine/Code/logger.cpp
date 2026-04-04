#include "pch.h"
#include "logger.h"


std::vector<std::string> Logger::LOG_STACK = std::vector<std::string>();
LoggerStatus Logger::CURRENT_STATUS = LoggerStatus::LOG_DISABLED;

void Logger::BeginSection(std::string const& string)
{
    if (CURRENT_STATUS == LoggerStatus::LOG_DISABLED) {
        return;
    }
    LOG_STACK.push_back(std::string{});
}

void Logger::Log(std::string const& string)
{
    if (CURRENT_STATUS == LoggerStatus::LOG_DISABLED) {
        return;
    }

    std::cout << string << std::endl;
}

void Logger::EndSection(std::string const& string)
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

