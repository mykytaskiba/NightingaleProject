#pragma once
#include "defines.h"

class GUID {

private:
	static  constexpr uint GUID_INVALID_VALUE{ 0xffffffffu };
	uint m_guidValue{ GUID_INVALID_VALUE };
public:

	static GUID Generate();
	static GUID From(uint);

	bool isInvalid() const;
	std::string string() const;

	bool operator==(GUID const& other) const;
};