#pragma once
#include <string>


    struct ExecutionResult {
        std::string message{ "No command executed" };
        bool bValidCommand{ false };
        bool bSuccess{ false };
        
    };
