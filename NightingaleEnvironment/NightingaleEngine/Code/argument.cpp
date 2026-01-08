#include "pch.h"
#include "argument.h"
#include "environment.h"

using namespace afterparty;

string Argument::extractNext(string& args)
{
    string result;
    size_t pos = args.find(' ');
    if (pos == string::npos) {
        result = args;
        args = "";
        return result;
    }
    result = args.substr(0, pos); 
    args = args.substr(pos + 1);
    return result;
}

string Argument::extractNextWithDefine(string& args)
{
    string arg = extractNext(args);

    ArgumentList<int, int, int, float> test;
    int aaa = sizeof(test);

    test.arg = 1;
    test.rest.arg = 2.0f;
    test.rest.rest.arg = 3;
    test.rest.rest.rest.arg = 5.0f;

    test.get();
    test.rest.get();
    int i1 = test.get<0>();
    int i2 = test.get<1>();
    int i3 = test.get<2>();
    float f1 = test.get<3>();

    
    if (arg.size() >= 2) {
        bool bDefineSyntax = arg[0] == '$';
        if (bDefineSyntax) {
            assert(AfterpartyEnvironment::getInstance() != nullptr);
            AfterpartyEnvironment& environment = *AfterpartyEnvironment::getInstance();
            //TO DO: make a define class?
            //map<string, string>& defines = environment.m_.defines;
            //bool bDefineExists = defines.find(arg.substr(1)) != defines.end();
            //if (bDefineExists) {
            //    arg = defines[arg.substr(1)];
            //}
        }
    }

    return arg;
}

void afterparty::Argument::extractSingleArg(ArgumentExtractionResult& result, float& arg) {
    string valueStr = extractNextWithDefine(result.args);
    try {
        arg = std::stof(valueStr);
    }
    catch (std::invalid_argument const& e) {
        result.errorMessage = valueStr + " is not a value for float";
        result.bSuccess = false;
        arg = 0.0f;
    }
    catch (std::out_of_range const& e) {
        result.errorMessage = valueStr + " is not a value for float";
        result.bSuccess = false;
        arg = 0.0f;
    }
}

void Argument::extractSingleArg(ArgumentExtractionResult& result, string& arg) {
    //Note: the loop is required to make sure that all the $defines are handled
    arg = "";

    arg += extractNextWithDefine(result.args);
    while (result.args != "") {

        arg += " " + extractNextWithDefine(result.args);
    }
}

void Argument::extractSingleArg(ArgumentExtractionResult& result, bool& arg)
{
    string valueStr = extractNextWithDefine(result.args);

    if (valueStr == "true") {
        arg = true;
        return;
    }
    if (valueStr == "false") {
        arg = false;
        return;
    }

    result.bSuccess = false;
    result.errorMessage = valueStr + " is not a value for bool type";
}


