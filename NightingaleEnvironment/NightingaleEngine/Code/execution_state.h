#pragma once
#include "defines.h"
#include  "argument_impl.h"
#include "guid.h"

class GameObject;
class RenderNode;
class ExecutionResult;

struct ExecutionState {

    string lastCommand{ "No command" };

    map<string, string> defines{};

    template<typename T> 
    T extract_or_default(string const& key, T const& default_val) const {
        bool bHasKey = defines.find(key) != defines.end();

        if (bHasKey) {
            string value(defines.at(key));
            ParsingResult parsingResult;
            T result = ArgumentExtractor<T>::parse(value, parsingResult);

            if (parsingResult.bSuccess) {
                return result;
            }
        }
        return default_val;
    }

    GUID getSelectedGUID() const;
    void setSelectedGUID(GUID const&);

    //log
    bool bCreateLog{ false };
    string log{};
    string errorLog{};

    GameObject* pGameObject;
    RenderNode* pRenderNode;

};
