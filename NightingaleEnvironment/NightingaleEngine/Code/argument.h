#pragma once
#include "defines.h"
#include "execution_state.h"
#include "advanced_arguments.h"

namespace afterparty {

    class AfterpartyEnvironment;



    struct ArgumentExtractionResult {

        string args{}; //args for processing
        bool bSuccess{ false };
        string errorMessage{};
        string sampleUsage{};
    };

    class Argument {
    public:
        static string extractNext(string&);
        static string extractNextWithDefine(string&);

    private:

        //Argument extraction

        template<typename Arg>
        static void extractSingleArg(ArgumentExtractionResult& result, Arg& arg) {
            //Specific tempalte for this arg not implemented
            assert(false);
        }

        static void extractSingleArg(ArgumentExtractionResult& result, float& arg);
        static void extractSingleArg(ArgumentExtractionResult& result, string& arg);
        static void extractSingleArg(ArgumentExtractionResult& result, bool& arg);



        //terminal function
        static void extractNextArg(ArgumentExtractionResult& result) {
            if (result.args.size() > 0) {
                //TOO MANY ARGUMENTS
                result.bSuccess = false;
                result.errorMessage = "Too many arguments given";
            }
        }

        //extract all args using Variadic templates
        template<typename Arg, typename... Rest>
        static void extractNextArg(ArgumentExtractionResult& result, Arg& arg, Rest&... rest) {
            extractSingleArg(result, arg);
            if (!result.bSuccess) return;
            extractNextArg(result, rest...);
        }
    public:




    private:

        template<typename Arg>
        static string argStringRepresentation(Arg const&) {
            //Specific tempalte for this arg not implemented
            //assert(false);
            return "error";
        }

        static string argStringRepresentation(float const&) { return "float"; }
        static string argStringRepresentation(string const&) { return "string"; }
        static string argStringRepresentation(bool const&) { return "bool"; }


        static void buildSampleUsageForArg(string& sampleUsage) {

        }

        template<typename Arg, typename... Rest>
        static void buildSampleUsageForArg(string& sampleUsage, Arg& arg, Rest&... rest) {

            sampleUsage += " (" + argStringRepresentation(arg) + ")";
            buildSampleUsageForArg(sampleUsage, rest...);
        }

    public:

        //top level build sample usage
        template<typename... Rest>
        static void buildSampleUsage(string& sampleUsage, Rest&... rest) {
            buildSampleUsageForArg(sampleUsage, rest...);
        }

        //top level extractArgsCall
        template<typename... Rest>
        static ArgumentExtractionResult extractArgs(string args, Rest&... rest) {
            ArgumentExtractionResult result;
            result.bSuccess = true; //assume true unless false
            result.errorMessage = "";
            result.args = args;
            extractNextArg(result, rest...);

            if (!result.bSuccess) {
                //if not successful build a sample string
                result.sampleUsage = "";
                buildSampleUsage(result.sampleUsage, rest...);
            }

            return result;
        }

    };

    template<typename Arg>
    struct ArgumentTest {
        
    };

    template<>
    struct ArgumentTest<int> {
        string usage() { return "int"; };
        void get();
    };

    template <typename... Args>
    struct ArgumentList;

    //base specialization
    template<>
    struct ArgumentList<> {

    };

    template<typename Arg, typename... Rest>
    struct ArgumentList<Arg, Rest...> {

        Arg arg;
        ArgumentList<Rest...> rest;

        Arg const& get() const {
            return arg;
        }

        template<int depth>
        auto get() const {
            if constexpr (depth == 0) {
                return get();
            }
            if constexpr (depth > 0) {
                return rest.template get<(depth - 1)>();
            }
        }

        string get_usage() const {

        }

    };

}
