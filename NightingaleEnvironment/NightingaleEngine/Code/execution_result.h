#pragma once
#include "defines.h"

namespace afterparty {

    struct ExecutionResult {
        string message{ "No command executed" };
        bool bValidCommand{ false };
        bool bSuccess{ false };
        
    };

}