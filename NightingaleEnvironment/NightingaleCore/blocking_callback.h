#pragma once
#include <functional>
#include <list>

template <typename... TArgs>
class BlockingCallback {

	using TCallback = std::function<bool(TArgs...)>;

	std::vector<TCallback> m_vCallback;

public:
	void addCallbackFront(TCallback callback) {
		if (callback == nullptr) {
			//why are we pushing a nullptr?
			assert(false);
			return;
		}
		m_vCallback.insert(m_vCallback.begin(), std::move(callback));
	}

	void addCallbackBack(TCallback callback) {
		if (callback == nullptr) {
			//why are we pushing a nullptr?
			assert(false);
			return;
		}
		m_vCallback.push_back(std::move(callback));
	}

	void execute(TArgs... args) const {
		for (TCallback const& callback : m_vCallback) {
			//assume no nullptr here
			bool bBlocking = callback(args...);
			if (bBlocking) {
				return;
			}
		}
	}

	void operator()(TArgs... args) const {
		execute(args..);
	}

};