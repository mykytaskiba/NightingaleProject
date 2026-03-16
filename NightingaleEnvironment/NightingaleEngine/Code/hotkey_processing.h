#pragma once
#include <set>
#include "key_enum.h"
#include <functional>

struct Hotkey {
	std::set<Key> m_sequence{};
	std::function<void()> m_callback{};
};

class HotkeyManager {

private:
	std::vector<Hotkey> m_vHotkeys;
	
	bool hotkeyPressed(Hotkey const& hotkey) const;

public:

	
	void processHotkeys();
	void addHotkey(Hotkey hotkey);
	void clearHotkey(std::set<Key> const& sequence);
};