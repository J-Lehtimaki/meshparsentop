#include "FEBoundary.hpp"
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

#include <vector>
#include <string>

TEST_CASE("FEBoudary"){
// FEBoundary constructor parameters
std::string pathPin =
"/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust1/pin";
std::string pathThread =
"/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust1/thread";
std::string pathPR =
"/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust1/pr";
std::string pathFea =
"/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust1/fea";
// Parameters for test case sections
fe::FEBoundary feHandler("myID", pathFea, pathPin, pathThread, pathPR);
std::string s("scott,mas,mushroom");

	SECTION("stack_oflow_example"){
		std::vector<std::string> vec = feHandler.parseFloatsDelimitedString(std::ref(s));
		REQUIRE(vec[0] == "scott");
		REQUIRE(vec[2] == "mushroom");
		REQUIRE(vec.size() == 3);
	}
} // TEST_CASE("FEBoundary")

TEST_CASE("Parserclass"){
	lineparser::Parser parser(",",".");
	std::string s("scott,mas,mushroom");

	SECTION("stack_oflow_example"){
		auto vec = parser.separateFloats(std::ref(s));
		REQUIRE(vec[0] == "scott");
		REQUIRE(vec[2] == "mushroom");
		REQUIRE(vec.size() == 3);
	}
	SECTION("convert_floatString_to_int_pair"){
		std::string floatString("123.4567");
		std::pair<int,int> coord = parser.splitFloatStringToIntPair(floatString);
		REQUIRE(123 == coord.first);
		REQUIRE(4567 == coord.second);

		floatString = "321.000005";
		coord = parser.splitFloatStringToIntPair(floatString);
		REQUIRE(321 == coord.first);
		REQUIRE(5 == coord.second);

		floatString = "91.50000000000000";
		try {
			coord = parser.splitFloatStringToIntPair(floatString);
			REQUIRE(91 == coord.first);
			REQUIRE(50000000000000 == coord.second);
		}catch(const std::out_of_range& oor){
			WARN("Catch too large number. Message: " << oor.what());
		}
	}
} // TEST_CASE("Parserclass")

