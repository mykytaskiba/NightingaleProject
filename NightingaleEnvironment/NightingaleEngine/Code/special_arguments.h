#pragma once
#include "defines.h"
#include "key_enum.h"
#include <set>

class ParsingResult;
class GameObject;

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

static void s_evaluateTriBool(bool& bValue, TriBool triBool) {
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

struct KeySequence {
	std::set<Key> m_sequence;
};

struct SelectedGameObject {
	GameObject* m_pGameObject{ nullptr };

	GameObject* operator*() const {
		return m_pGameObject;
	}

	//retrieve from state
	bool retrieve(ParsingResult& result);
};