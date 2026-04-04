#pragma once
#include "bitwise_enum.hpp"
#include <functional>

enum class MetaFlags {
	None = 0,
	ReadOnly = 1,
	ColorOnlyRGB = 2
};
ENABLE_ENUM_BITWISE_OPERATORS(MetaFlags);

struct MetaData {
	using TChangeCallback = std::function<void()>; //std::function -> non literal type
private:
	MetaFlags m_flags{ MetaFlags::None };

	std::vector<TChangeCallback> m_vCbOnChanged{};
public:

	inline MetaData() = default;
	inline MetaData(MetaFlags flags) : m_flags(flags) {}


	inline bool isFlag(MetaFlags flag) const {
		return ((m_flags & flag) == flag);
	}
	
	inline bool isReadOnly() const {
		return isFlag(MetaFlags::ReadOnly);
	}


	inline void onChangeCallback() const {
		for (TChangeCallback const& callback : m_vCbOnChanged) {
			if (callback != nullptr) {
				callback();
			}
		}
	}

	static MetaData ReadOnly() { return MetaFlags::ReadOnly; }
	static MetaData OnlyRGB() { return MetaFlags::ColorOnlyRGB; }

	static MetaData OnChange(TChangeCallback changeCallback) {
		MetaData changeMetaData{};
		changeMetaData.m_vCbOnChanged.push_back(changeCallback);
		return changeMetaData;
	}

	inline MetaData operator|(MetaData const& other) const {
		MetaData result;
		result.m_flags = other.m_flags | m_flags;
		
		result.m_vCbOnChanged = m_vCbOnChanged;
		for (TChangeCallback const& otherCallback : other.m_vCbOnChanged) {
			result.m_vCbOnChanged.push_back(otherCallback);
		}

		return result;
	}

};