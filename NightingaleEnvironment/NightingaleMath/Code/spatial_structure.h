#pragma once
#include <functional>

/*
template <typename TEntity, typename TBounds>
concept HasBounds = requires (TEntity entity) {
	{ entity.getBounds() } -> std::convertible_to<TBounds>;
};*/

/*
template <typename TBounds>
concept CanIntersect = requires (TBounds bounds, TBounds other) {
	{ bounds.intersects(other) } -> std::same_as<bool>;
};*/

template <typename TEntity>
class SpatialStructure {
public:
	virtual void insert(TEntity*) = 0;
	virtual bool remove(TEntity*) = 0;

	using PairFunction = std::function<void(TEntity&, TEntity&)>;
	virtual void queryPairs(PairFunction const&) = 0;
	virtual void queryRay() {};
	virtual void queryVolume() {};


};