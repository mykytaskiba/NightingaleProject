#pragma once
#include "defines.h"

struct Line {
	string line;
	
	string& operator*() {
		return line;
	}

	string const& operator*() const {
		return line;
	}
};

struct AllText {
	string text;
	string& operator*() {
		return text;
	}

	string const& operator*() const {
		return text;
	}
};

struct Path {
	string path;
	string& operator*() {
		return path;
	}

	string const& operator*() const {
		return path;
	}
};

struct Nothing {

};

//A tri bool, is a bool (true/false) or a toggle
enum class TriBool {
	TRUE,
	FALSE,
	TOGGLE
};

static void EvaluateTriBool(bool& bValue, TriBool triBool) {
	if (triBool == TriBool::TRUE) {
		bValue = true;
	}
	if (triBool == TriBool::FALSE) {
		bValue = false;
	}
	if (triBool == TriBool::TOGGLE) {
		bValue = !bValue;
	}
}