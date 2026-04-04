#pragma once
#include <filesystem>

struct FileSource {
	FileSource(std::filesystem::path path) : m_path(path) {
	}

	std::filesystem::path m_path;

private:
};