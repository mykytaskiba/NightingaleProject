#pragma once
#include "defines.h"
#include "argument_helpers.h"

struct ParsingResult {
    bool bSuccess{ false };
    string errorMessage{};
};

template<typename Arg>
struct ArgumentExtractor {
    //must have
    //
    //static string usage() { return "arg identifier" }
    //static Arg parse(string& args, ArgumentExtractionResult& result);
};

//Begin argument templates

//INT
template<>
struct ArgumentExtractor<int> {
    static string usage() { return "int"; }
};

//FLOAT
template<>
struct ArgumentExtractor<float> {
    static string usage() { return "float"; }
    static float parse(string& args, ParsingResult& result);
};

//STRING
template<>
struct ArgumentExtractor<string> {
    static string usage() { return "text"; }
    static string parse(string& args, ParsingResult& result);
};

//BOOL
template<>
struct ArgumentExtractor<bool> {
    static string usage() { return "true/false"; }
    static bool parse(string& args, ParsingResult& result);
};



//begin all function implementations

inline float ArgumentExtractor<float>::parse(string& args, ParsingResult& result)
{
    float arg;
    string valueStr = ArgumentHelpers::getNextWithDefines(args);
    try {
        result.bSuccess = true;
        return std::stof(valueStr);
    }
    catch (std::invalid_argument const& e) {
        result.errorMessage = valueStr + " is not a value for float";
        result.bSuccess = false;
        return 0.0f;
    }
    catch (std::out_of_range const& e) {
        result.errorMessage = valueStr + " is not a value for float";
        result.bSuccess = false;
        return 0.0f;
    }
    
}

inline string ArgumentExtractor<string>::parse(string& args, ParsingResult& result)
{
    //Note: the loop is required to make sure that all the $defines are handled
    string arg = "";

    arg += ArgumentHelpers::getNextWithDefines(args);
    while (args != "") {

        arg += " " + ArgumentHelpers::getNextWithDefines(args);
    }

    result.bSuccess = true;
    return arg;
}

inline bool ArgumentExtractor<bool>::parse(string& args, ParsingResult& result)
{
    string valueStr = ArgumentHelpers::getNextWithDefines(args);

    if (valueStr == "true") {
        result.bSuccess = true;
        return true;
    }
    if (valueStr == "false") {
        result.bSuccess = true;
        return false;
    }

    result.bSuccess = false;
    result.errorMessage = valueStr + " is not a value for bool type";
    return false;
}