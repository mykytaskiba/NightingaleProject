#pragma once
#include "defines.h"

namespace afterparty {


    struct ExecutionState {

        string lastCommand{ "No command" };

        map<string, string> defines{};

        //log
        bool bCreateLog{ false };
        string log{};
        string errorLog{};


        //loader values
        float loaderScale{ 1.0f };
    };
}