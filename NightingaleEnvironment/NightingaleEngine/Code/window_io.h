#pragma once
#include "blocking_callback.h"
#include <filesystem>

class WindowIO {
	using TDropCallback = BlockingCallback<std::filesystem::path>;
private:
	TDropCallback m_dropCallback;

public:
	TDropCallback& dropCallback() { return m_dropCallback; }

};