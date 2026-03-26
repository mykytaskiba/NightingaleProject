#pragma once

struct MetaData {
	bool m_bReadOnly{ false };

	static constexpr MetaData ReadOnly() {
		MetaData data{};
		data.m_bReadOnly = true;
		return data;
	}
};