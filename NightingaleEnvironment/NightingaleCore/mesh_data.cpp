#include "mesh_data.h"
#include "nightingale_assert.h"
#include <algorithm>

bool MeshData::hasData() const {
	return (m_vData.size() != 0 || m_vTriangles.size() != 0 || m_vAttributeOrder.size() != 0);
}

void MeshData::addAttribute(Attribute const& attribute)
{
	if (attribute.m_label.empty()) {
		assert(false); //cannot have an empty label for an attribute 	
		return;
	}
	if (m_mapAttributes.find(attribute.m_label) != m_mapAttributes.end()) {
		assert(false); //attribute already exists
		return;
	}

	m_mapAttributes[attribute.m_label] = attribute;
	m_vAttributeOrder.push_back(&m_mapAttributes[attribute.m_label]);
	
	calculateDataStride();

	if (!m_vData.empty()) {
		assert(false); //we are changing the data before adding all the desired attributes!!
		m_vData.clear(); //could we rearrange the existing data? Yes -> but it would be a lot of work
	}
}

void MeshData::prepareVertexData(unsigned int vertexCount)
{
	if (m_dataStride == 0 || m_vAttributeOrder.empty() || m_mapAttributes.empty() || m_vertexCount != 0) {
		assert(false); //we must have at least one attribute
		return;
	}
	m_vData.reserve(vertexCount* m_dataStride);
	m_vertexCount = vertexCount;
}

unsigned int MeshData::getSizeByPrimitive(PrimitiveType type) const
{
	switch (type)
	{
	case MeshData::PrimitiveType::UNSIGNED_INT:
		return sizeof(unsigned int);
	case MeshData::PrimitiveType::FLOAT:
		return sizeof(float);
	default:
		return 0;
	}
}

void MeshData::calculateDataStride()
{
	//NOTE: PADDING MIGHT NEED TO BE CONSIDERED, NOT IN CURRENT STATE THO
	unsigned int offset{ 0u };
	for (Attribute* pAttribute : m_vAttributeOrder) {

		if (pAttribute == nullptr) {
			assert(false); //uh oh, never happens
			return;
		}

		Attribute& attribute = *pAttribute;

		attribute.m_offset = offset;

		unsigned int typeSize = getSizeByPrimitive(attribute.m_type);
		attribute.m_size = typeSize* attribute.m_count;

		offset += attribute.m_size;
	}

	m_dataStride = offset;
}


void MeshData::prepareFaceData(unsigned int triangleCount) {

	if (!m_vTriangles.empty() || m_triangleCount != 0) {
		assert(false); //we already have triangles
		return;
	}
	m_vTriangles.reserve(triangleCount);
	m_triangleCount = triangleCount;
}


void MeshData::pushFace(Triangle const& triangle) {
	
	if (m_vTriangles.size() >= m_triangleCount) {
		assert(false); //we already have all our faces
		return;
	}
	m_vTriangles.push_back(triangle);
}
