#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/typeof/typeof.hpp>

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/parser/exception.hpp>
#include <jsoneasy/template.hpp>

#include <vector>
#include <list>
#include <stack>
#include <deque>

#include <boost/assign.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/range.hpp>

using namespace boost::assign;

template<typename R>
static void dumpRange(const R& x1, const std::string& label) {
	BOOST_AUTO(x1b, boost::begin(x1));
	BOOST_AUTO(x1e, boost::end(x1));

	std::cout << "Dumping range " << label << std::endl;
	std::cout << "Size = " << std::distance(x1b, x1e) << std::endl;
	for(BOOST_AUTO(i, boost::begin(x1)); i != boost::end(x1); ++i) {
		std::cout << *i << std::endl;
	}
	std::cout << std::endl;
}

template<typename R1, typename R2>
static bool cequals(const R1& x1, const R2& x2) {
	BOOST_AUTO(x1b, boost::begin(x1));
	BOOST_AUTO(x1e, boost::end(x1));

	BOOST_AUTO(x2b, boost::begin(x2));

	BOOST_AUTO(d1, boost::distance(x1));
	BOOST_AUTO(d2, boost::distance(x2));

	if( d1 != d2 || !std::equal(x1b, x1e, x2b) ) {
		dumpRange(x1, "left");
		dumpRange(x2, "right");
		return false;
	}
	return true;
}

template<typename T>
static bool parseTo(const std::string& input, T& data) {
	try {
		JsonEasy::Parser::Handler::Ptr h = JsonEasy::Template::createHandler(data);
		JsonEasy::Parser::StringParser sp;
		return sp.parse(input, h);
	} catch(const JsonEasy::Parser::UnexpectedException&) {}
	return false;
}

BOOST_AUTO_TEST_CASE ( vectorTest ) {
	std::vector<int> x;
	BOOST_CHECK( parseTo("[]", x) );
	BOOST_CHECK( x.empty() );

	BOOST_CHECK( parseTo("[721]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x.back(), 721);

	// old data should be removed
	BOOST_CHECK( parseTo("[123]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x.back(), 123);

	BOOST_CHECK( parseTo("[123, 321, 0, 4, 12]", x) );
	{
		std::vector<int> ethalon;
		ethalon += 123,321,0,4,12;
		BOOST_CHECK( cequals(x, ethalon) );
	}
}

BOOST_AUTO_TEST_CASE ( listTest ) {
	std::list<int> x;
	BOOST_CHECK( parseTo("[]", x) );
	BOOST_CHECK( x.empty() );

	BOOST_CHECK( parseTo("[721]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x.back(), 721);

	// old data should be removed
	BOOST_CHECK( parseTo("[123]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x.back(), 123);

	parseTo("[123, 321, 0, 4, 12]", x);
	{
		std::vector<int> ethalon;
		ethalon += 123,321,0,4,12;
		BOOST_CHECK( cequals(x, ethalon) );
	}
}

BOOST_AUTO_TEST_CASE ( setTest ) {
	std::set<int> x;
	BOOST_CHECK( parseTo("[]", x) );
	BOOST_CHECK( x.empty() );

	BOOST_CHECK( parseTo("[721]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(*x.begin(), 721);

	// old data should be removed
	BOOST_CHECK( parseTo("[123]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(*x.begin(), 123);

	parseTo("[123, 321, 0, 4, 12]", x);
	{
		std::vector<int> ethalon;
		ethalon += 0,4,12,123,321;
		BOOST_CHECK( cequals(x, ethalon) );
	}

	// multiple same values
	BOOST_CHECK( !parseTo("[1,4,1]", x) );

	//just compilation test
	std::set<int, std::greater<int> > x2;
	BOOST_CHECK( parseTo("[]", x2) );
}

BOOST_AUTO_TEST_CASE ( multisetTest ) {
	{
		std::multiset<int> x;
		BOOST_CHECK( parseTo("[123, 321, 0, 4, 12, 0, 0, 123]", x) );

		std::vector<int> ethalon;
		ethalon += 0,0,0,4,12,123,123,321;
		BOOST_CHECK( cequals(x, ethalon) );
	}
	// compilation tests
	{
		std::multiset<int, std::greater<int> > x;
		BOOST_CHECK( parseTo("[123, 321, 0, 4, 12, 0, 0, 123]", x) );
	}
}

BOOST_AUTO_TEST_CASE ( stackTest ) {
	std::stack<int> x;
	BOOST_CHECK( parseTo("[123, 321, 0, 4, 12, 0, 0, 123]", x) );
	BOOST_REQUIRE_EQUAL(x.size(), 8u);
	BOOST_REQUIRE_EQUAL(x.top(), 123);
}

BOOST_AUTO_TEST_CASE ( dequeTest ) {
	std::deque<int> x;
	BOOST_CHECK( parseTo("[123, 321, 0, 4, 12, 0, 0, 123]", x) );
	BOOST_REQUIRE_EQUAL(x.size(), 8u);
	BOOST_REQUIRE_EQUAL(x.back(), 123);
}

BOOST_AUTO_TEST_CASE ( mapTest ) {
	std::map<std::string, int> x;
	BOOST_CHECK( parseTo("{}", x) );
	BOOST_CHECK( x.empty() );
	BOOST_CHECK( parseTo("{\"hello\":1}", x) );
	BOOST_CHECK_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x["hello"], 1);
	BOOST_CHECK( parseTo("{\"hello\":1, \"world\":5}", x) );
	BOOST_CHECK_EQUAL( x.size(), 2u );
	BOOST_CHECK_EQUAL(x["hello"], 1);
	BOOST_CHECK_EQUAL(x["world"], 5);
}

BOOST_AUTO_TEST_CASE ( startTest ) {
	std::map<std::string, int> x1;
	std::vector<int> x2;
	BOOST_CHECK( !parseTo("[]", x1) );
	BOOST_CHECK( !parseTo("{}", x2) );
	BOOST_CHECK( parseTo("[]", x2) );
	BOOST_CHECK( parseTo("{}", x1) );
}

// multimap
// optional
// double
// multitype
// int
// bool
// null

//BOOST_AUTO_TEST_CASE ( mapWithNotStringKey ) {
//	std::map<int, int> x;
//	parseTo("{}", x);
//}