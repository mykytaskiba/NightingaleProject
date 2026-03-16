#include "pch.h"
#include "argument_helpers.h"
#include "argument.h"
#include "environment.h"


string ArgumentHelpers::getNextRaw(string& args)
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

string ArgumentHelpers::getNextWithDefines(string& args)
{
    string arg = getNextRaw(args);

    if (arg.size() >= 2) {
        bool bDefineSyntax = arg[0] == '$';
        if (bDefineSyntax) {
            //assert(Environment::getInstance() != nullptr);
            //Environment& environment = *Environment::getInstance();
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

std::vector<std::string> ArgumentHelpers::separateByDelimiter(std::string const& inString, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(inString);

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

