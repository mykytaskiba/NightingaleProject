#pragma once

/*
struct VisitorTemplate {
	template<typename TValue, typename... TMeta >
	void operator()(std::string const& key, TValue& value, TMeta&&... meta);
};
class VisitorAcceptor {
	template <typename TPropertyVisitor>
	void properties(TPropertyVisitor& visitor);
}
*/

struct DummyVisitor {
	template<typename TValue, typename... TMeta >
	void operator()(std::string const& key, TValue& value, TMeta&&... meta) {
	}
};


struct Meta {

	struct ReadOnly {};   // force read-only of this data, even if non-const
	struct DummyTrait {};

	template<typename TMetaValue, typename... TMeta>
	static bool has(TMeta&&... meta) {
		return (std::is_same_v<TMetaValue, std::decay_t<TMeta>> || ...);
	}

};

template <typename TObject>
concept HasProperties = requires(TObject obj, DummyVisitor visitor) {
	obj.properties(visitor);
};


/* 
this is AI Generated, does this work??
template<typename TMetaValue, typename... Meta>
TMetaValue get_or(Meta&&... meta, TMetaValue const& def) {
	TMetaValue result = def;
	((std::is_same_v<T, std::decay_t<Meta>> ? result = meta : void()), ...);
	return result;
}
*/