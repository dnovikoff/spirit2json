#ifndef JSONEASY_TEMPLATE_MAP_HPP_
#define JSONEASY_TEMPLATE_MAP_HPP_

#include <map>

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<typename K,typename V, typename C, typename A>
class Container<JsonObject, std::map<K,V,C,A> > {
	typedef std::map<K,V,C,A> MapType;
public:
	MapType data;
	typedef V ValueType;
	typedef K KeyType;

	bool insert(KeyType& key, ValueType& val) {
		auto p = std::make_pair(std::move(key), std::move(val));
		return data.insert(std::move(p)).second;
	}

	bool validate() { return true; }
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_MAP_HPP_ */
