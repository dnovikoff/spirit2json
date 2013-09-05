#ifndef JSONEASY_TEMPLATE_SET_HPP_
#define JSONEASY_TEMPLATE_SET_HPP_

namespace std {
template<typename T, typename C, typename A> class set;
template<typename T, typename C, typename A> class multiset;
} // namespace std

#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

/**
 * Support for set
 * Will fail parsing in case of multiple values
 */
template<typename T, typename C, typename A>
class Container<JsonArray, std::set<T,C,A > > {
	typedef std::set<T,C,A> SetType;
public:
	SetType data;
	typedef T ValueType;

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if( !jsonToUser(x, tmp) ) return false;
		return data.insert( std::move(tmp) ).second;
	}
};

/**
 * Support for multiset
 */
template<typename T, typename C, typename A>
class Container<JsonArray, std::multiset<T,C,A > > {
	typedef std::multiset<T,C,A> SetType;
public:
	SetType data;
	typedef T ValueType;

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if( !jsonToUser(x, tmp) ) return false;
		data.insert( std::move(tmp) );
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_SET_HPP_ */
