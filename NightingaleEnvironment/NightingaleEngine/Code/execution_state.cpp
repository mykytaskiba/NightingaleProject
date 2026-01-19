#include "pch.h"
#include "execution_state.h"
#include "argument_impl.h"

float ExecutionState::extract_float_with_default(string const& key, float default_val) const
{
    bool bHasKey = defines.find(key) != defines.end();

    if (bHasKey) {
        string value(defines.at(key));
        ParsingResult parsingResult;
        float result = ArgumentExtractor<float>::parse(value, parsingResult);
        
        if (parsingResult.bSuccess) {
            return result;
        }
    }
    return default_val;
}
