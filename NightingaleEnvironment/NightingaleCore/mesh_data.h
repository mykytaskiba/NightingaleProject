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

	struct Attribute {
		Attribute() : m_label("nolabel"), m_type(PrimitiveType::FLOAT), m_count(0u) {}
		Attribute(std::string label, PrimitiveType type, unsigned int count) : m_label(label), m_type(type), m_count(count) {}
		std::string m_label;
		PrimitiveType m_type;
		unsigned int m_count;

		unsigned int m_size{ 0 };
		unsigned int m_offset{ 0 };
	};

	struct Triangle {
		Triangle(unsigned int p1, unsigned int p2, unsigned int p3) : m_p1(p1), m_p2(p2), m_p3(p3) {}
		unsigned int m_p1;
		unsigned int m_p2;
		unsigned int m_p3;
	};
private:

	std::vector<unsigned char> m_vData{};
	unsigned int m_dataStride{ 0u };
	unsigned int m_vertexCount{ 0u };

	std::vector<Attribute*> m_vAttributeOrder{};
	std::map<std::string, Attribute> m_mapAttributes{};


	std::vector<Triangle> m_vTriangles{};
	unsigned int m_triangleCount{ 0u };
	

public:

	bool hasData() const;

	void addAttribute(Attribute const& attribute);

	void prepareVertexData(unsigned int vertexCount);

	template <typename T>
	void pushVertexData(T value) {
		static_assert(std::is_trivially_copyable_v<T>);

		unsigned char const* src = reinterpret_cast<unsigned char const*>(&value);
		m_vData.insert(m_vData.end(), src, src + sizeof(T));
	}

	unsigned int getSizeByPrimitive(PrimitiveType type) const;
	void calculateDataStride();

	void prepareFaceData(unsigned int faceCount);
	void pushFace(Triangle const& triangle);

};