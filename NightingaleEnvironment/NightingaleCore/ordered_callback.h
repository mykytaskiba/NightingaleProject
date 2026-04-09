#pragma once

struct CallbackRef {
	std::multiset<PriorityFunction>::iterator m_iterator;
};

class CallbackHandler {

private:
	std::multiset<PriorityFunction> m_vFunctions;

	bool m_bExecuting{ false };
	std::vector<CallbackRef> m_deferredRemovals; //deffer removals if called mid execution


public:
	CallbackRef addCallback(PriorityFunction function);
	bool removeCallback(CallbackRef ref);
	void execute();
};

#include <functional>
#include <vector>

template <typename... TArgs>
class OrderedCallback {

	using TCallback = std::function<bool(TArgs...)>;
	using TCallbackPriority = unsigned int;
	using TCallbackID = unsigned int;

	static constexpr TCallbackID INVALID_ID = 0xffffffff;

	struct CallbackEntry {
		TCallbackID m_id;
		TCallbackPriority m_priority;
		TCallback m_func;

		// This allows std::stable_sort to order them by priority
		bool operator<(const CallbackEntry& other) const {
			return priority < other.priority;
		}
	};

	std::vector<CallbackEntry> m_vCallbacks{};
	TCallbackID m_idCounter{ 0u };
	bool m_bDirty{ true };
	bool m_bExecuting{ false };
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

		if (m_bExecuting) dasdasdsaad //ADD WHILE RUNNING LOGIC HERE

		m_vCallbacks.emplace_back({ outID, priority, std::move(callback) });
		return outID;
	}

	bool removeCallback(TCallbackID id) {
		if (id == INVALID_ID) {
			assert(false);
			return false;
		}

		if (m_bExecuting) dasdasdsaad //ADD WHILE RUNNING LOGIC HERE

		size_t numRemoved = std::erase_if(m_vCallbacks, 
			[id](CallbackEntry const& entry) {
				return entry.m_id == id;
			}
		);

		if (numRemoved > 1) {
			assert(false);
			return false;
		}

		return (numRemoved == 1);
	}

	void execute(TArgs... args) const {
		for (TCallback const& callback : m_vCallbacks) {
			//assume no nullptr here
			bool bBlocking = callback(args...);
			if (bBlocking) {
				return;
			}
		}
	}

	void operator()(TArgs... args) const {
		execute(args...);
	}

}