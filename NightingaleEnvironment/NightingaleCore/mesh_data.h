#pragma once
#include <vector>
#include <string>
#include <map>

struct MeshData {

public:
	enum class PrimitiveType {
		UNSIGNED_INT,
		FLOAT,
	};

private:
	struct AttributeFormat {
		AttributeFormat(std::string label, PrimitiveType type, unsigned int count) : m_label(label), m_type(type), m_count(count) {}
		std::string m_label;
		PrimitiveType m_type;
		unsigned int m_count;

		unsigned int m_size{ 0 };
		unsigned int m_offset{ 0 };
	};

	std::vector<unsigned char> m_vData{};

	std::vector<std::string> m_vAttributeOrder{};
	std::map<std::string, AttributeFormat> m_mapAttributes{};

	unsigned int m_dataStride{ 0 };


public:

	void addAttribute(AttributeFormat const& attribute);

	void prepareVertexData(unsigned int vertexCount);
	void pushVertexData(float value);
	void pushVertexData(unsigned int value);

	unsigned int getSizeByPrimitive(PrimitiveType type) const;
	void calculateDataStride();

};