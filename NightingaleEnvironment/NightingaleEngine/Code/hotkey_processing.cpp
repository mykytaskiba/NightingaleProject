#include "pch.h"
#include "hotkey_processing.h"
#include "input.h"


bool HotkeyManager::hotkeyPressed(Hotkey const& hotkey) const
{
	bool allKeysDown = true;
	bool anyKeyClicked = false;
	for (Key key : hotkey.m_sequence) {
		allKeysDown &= Input::KeyDown(key);
		anyKeyClicked |= Input::KeyClicked(key);
	}
	return allKeysDown && anyKeyClicked;
}

void HotkeyManager::processHotkeys()
{
	Hotkey const* pExecutedHotkey{ nullptr };

	for (Hotkey const& hotkey : m_vHotkeys) {
		if (hotkeyPressed(hotkey)) {
			if (pExecutedHotkey == nullptr || pExecutedHotkey->m_sequence.size() < hotkey.m_sequence.size()) {
				pExecutedHotkey = &hotkey;
			}
		}
	}

	if (pExecutedHotkey != nullptr) {
		(pExecutedHotkey->m_callback)();
	}
}

void HotkeyManager::addHotkey(Hotkey addedHotkey)
{
	clearHotkey(addedHotkey.m_sequence);
	m_vHotkeys.push_back(addedHotkey);
}

void HotkeyManager::clearHotkey(std::set<Key> const& sequence)
{
	for (auto it = m_vHotkeys.begin(); it != m_vHotkeys.end(); ++it) {
		if ((*it).m_sequence == sequence) {
			m_vHotkeys.erase(it);
			return;
		}
	}
}
