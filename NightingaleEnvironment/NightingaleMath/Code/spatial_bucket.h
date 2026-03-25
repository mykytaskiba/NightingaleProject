#pragma once
#include "spatial_structure.h"
#include <vector>

template <typename TEntity, typename TBounds>
class SpatialBucket : public SpatialStructure<TEntity, TBounds> {
private:
	std::vector<TEntity*> m_vEntities{};
public:
	void insert(TEntity* pEntity) override {
		assert(pEntity != nullptr);
		m_vEntities.push_back(pEntity);
	};
	bool remove(TEntity* pEntity) override {
		assert(pEntity != nullptr);
		return std::erase(m_vEntities, pEntity);
	};

	void queryPairs(SpatialStructure<TEntity, TBounds>::PairFunction const& function) override {
		for (auto it_outer = m_vEntities.begin(); it_outer != m_vEntities.end(); ++it_outer) {
			for (auto it_inner = std::next(it_outer); it_inner != m_vEntities.end(); ++it_inner) {
				function(*it_outer, *it_inner);
			}
		}
	}
	

};