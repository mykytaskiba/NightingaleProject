#pragma once
#include "bitwise_enum.hpp"

enum class MetaFlags {
	None = 0,
	ReadOnly = 1
};
ENABLE_ENUM_BITWISE_OPERATORS(MetaFlags);

struct MetaData {

public:
	MetaFlags m_flags{ MetaFlags::None };
	
	bool isReadOnly() const {
		return ((m_flags & MetaFlags::ReadOnly) == MetaFlags::ReadOnly);
	}

	static constexpr MetaFlags ReadOnly{ MetaFlags::ReadOnly };

	inline constexpr MetaData() {}
	//Flag only constructor 
	inline constexpr MetaData(MetaFlags flags) : m_flags(flags) {}
};