#pragma once
#include "defines.h"
#include  "argument_impl.h"
#include "guid.h"

class GameObject;
class RenderNode;
class ExecutionResult;

struct ExecutionState {

    std::string lastCommand{ "No command" };

    std::map<std::string, std::string> defines{};

    template<typename T> 
    T extract_or_default(std::string const& key, T const& default_val) const {
        bool bHasKey = defines.find(key) != defines.end();

        if (bHasKey) {
            std::string value(defines.at(key));
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
    std::string log{};
    std::string errorLog{};

    GameObject* pGameObject;
    RenderNode* pRenderNode;

};
