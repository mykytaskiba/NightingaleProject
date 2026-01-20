#pragma once
#include "defines.h"
#include "argument_helpers.h"
#include "special_arguments.h"
#include "ngmath.h"

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
    static float parse(string& args, ParsingResult& result)
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
};

//BOOL
template<>
struct ArgumentExtractor<bool> {
    static string usage() { return "true/false"; }
    static bool parse(string& args, ParsingResult& result) 
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
};

//ALL TEXT
template<>
struct ArgumentExtractor<AllText> {
    static string usage() { return "text..."; }
    static AllText parse(string& args, ParsingResult& result)
    {
        //Note: the loop is required to make sure that all the $defines are handled
        AllText arg;
        arg.text = "";

        arg.text += ArgumentHelpers::getNextWithDefines(args);
        while (args != "") {

            arg.text += " " + ArgumentHelpers::getNextWithDefines(args);
        }

        result.bSuccess = true;
        if (arg.text.length() == 0) {
            result.bSuccess = false;
        }

        return arg;
    }

};

//LINE
template<>
struct ArgumentExtractor<Line> {
    static string usage() { return "line"; }
    static Line parse(string& args, ParsingResult& result)
    {
        Line arg;
        arg.line = ArgumentHelpers::getNextWithDefines(args);

        result.bSuccess = true;
        if (arg.line.length() == 0) {
            result.bSuccess = false;
        }

        return arg;
    }
};

//VOID
template<>
struct ArgumentExtractor<Nothing> {
    static string usage() { return "void"; }
    static Nothing parse(string& args, ParsingResult& result)
    {

        result.bSuccess = true;
        return Nothing();
    }
};

//Vector3
template<>
struct ArgumentExtractor<Vector3> {
    static string usage() { return "3d vector"; }
    static Vector3 parse(string& args, ParsingResult& result) {

        Vector3 arg(0,0,0);
        arg[0] = ArgumentExtractor<float>::parse(args, result);
        if (!result.bSuccess) return arg;
        arg[1] = ArgumentExtractor<float>::parse(args, result);
        if (!result.bSuccess) return arg;
        arg[2] = ArgumentExtractor<float>::parse(args, result);
        if (!result.bSuccess) return arg;
        
        return arg;
    }
};




