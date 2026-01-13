#pragma once
#include "defines.h"
#include "execution_state.h"
#include "advanced_arguments.h"
#include "argument_impl.h"


class ScriptingEnvironment;





template <typename... Args>
struct ArgumentList;

//base specialization
template<>
struct ArgumentList<> {

	void get_usage_recur(string& result) const {
	}
	void parse_recur(string& args, ParsingResult& result) {

	}
};

template<typename Arg, typename... Rest>
struct ArgumentList<Arg, Rest...> {

private:
	Arg arg;
	ArgumentList<Rest...> rest;

	Arg const& get() const {
		return arg;
	}

	void get_usage_recur(string& result) const {
		result += " (" + ArgumentExtractor<Arg>::usage() + ")";
		rest.get_usage_recur(result);
	}

	void parse_recur(string& args, ParsingResult& result) {
		arg = ArgumentExtractor<Arg>::parse(args, result);
		if (result.bSuccess) {
			rest.parse_recur(args, result);
		}
	}

public:


	template<uint depth>
	auto get() const {
		if constexpr (depth == 0) {
			return get();
		}
		if constexpr (depth > 0) {
			return rest.template get<(depth - 1)>();
		}
	}

	string get_usage() const {
		string result = "";
		get_usage_recur(result);
		return result;
	}

	ParsingResult parse(string& args) {
		ParsingResult result;

		parse_recur(args, result);

		return result;

	}


};


/*

	/*

	class Argument {
	public:
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
	*/