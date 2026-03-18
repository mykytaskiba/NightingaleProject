#include "pch.h"
#include "guid.h"

GUID GUID::Generate()
{
	static uint generatorValue{ 0u };
	GUID result;
	result.m_guidValue = generatorValue++;
	return result;
}

GUID GUID::From(uint value)
{
	GUID result;
	result.m_guidValue = value;
	return result;
}

bool GUID::isInvalid() const
{
	return m_guidValue == GUID_INVALID_VALUE;
}

std::string GUID::string() const
{
	return std::to_string(m_guidValue);
}

bool GUID::operator==(GUID const& other) const
{
	return m_guidValue != GUID_INVALID_VALUE && m_guidValue == other.m_guidValue;
}
