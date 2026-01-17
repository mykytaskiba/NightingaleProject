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