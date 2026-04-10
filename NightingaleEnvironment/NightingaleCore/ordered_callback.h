#pragma once
#include <functional>
#include <vector>
#include <algorithm>

using TCallbackPriority = unsigned int;
using TCallbackID = unsigned int;

template <typename... TArgs>
class OrderedCallback {

	using TCallback = std::function<void(TArgs...)>;

	static constexpr TCallbackID INVALID_ID = 0xffffffff;

	struct CallbackEntry {
		TCallbackID m_id;
		TCallbackPriority m_priority;
		TCallback m_func;

		CallbackEntry(TCallbackID id, TCallbackPriority priority, TCallback func) : m_id(id), m_priority(priority), m_func(std::move(func)) {}

		// This allows std::stable_sort to order them by priority
		bool operator<(const CallbackEntry& other) const {
			return m_priority < other.m_priority;
		}
	};

	TCallbackID m_idCounter{ 0u };

	std::vector<CallbackEntry> m_vCallbacks{};
	std::vector<TCallbackID> m_vDefferedRemoves{};
	std::vector<CallbackEntry> m_vDefferedAdds{};

public:
	TCallbackID addCallback(TCallbackPriority priority, TCallback callback) {
		if (callback == nullptr) {
			//why are we pushing a nullptr?
			assert(false);
			return INVALID_ID;
		}
		TCallbackID outID = m_idCounter++;
		assert(m_idCounter != INVALID_ID);

		m_vDefferedAdds.emplace_back(CallbackEntry{ outID, priority, std::move(callback) });
		return outID;
	}

	bool removeCallback(TCallbackID id) {
		if (id == INVALID_ID) {
			assert(false);
			return false;
		}

		auto itFound = std::find_if(m_vCallbacks.begin(), m_vCallbacks.end(),
			[id](CallbackEntry const& entry) {
				return entry.m_id == id;
			}
		);

		if (itFound == m_vCallbacks.end()) {
			return false;
		}

		m_vDefferedRemoves.push_back(id);
		return true;
	}

	void processDefferals() {

		if (m_vDefferedAdds.empty() && m_vDefferedRemoves.empty()) {
			return;
		}

		for (TCallbackID id : m_vDefferedRemoves) {
			size_t numRemoved = std::erase_if(m_vCallbacks,
				[id](CallbackEntry const& entry) {
					return entry.m_id == id;
				}
			);
			assert(numRemoved == 1);
		}
		m_vDefferedRemoves.clear();

		for (CallbackEntry& entry : m_vDefferedAdds) {
			m_vCallbacks.push_back(std::move(entry));
		}
		m_vDefferedAdds.clear();

		std::stable_sort(m_vCallbacks.begin(), m_vCallbacks.end());

	}

	void execute(TArgs... args) {
		processDefferals();

		for (CallbackEntry const& entry : m_vCallbacks) {
			//assume no nullptr here
			entry.m_func(args...);
		}
	}

	void operator()(TArgs... args) {
		execute(args...);
	}

};