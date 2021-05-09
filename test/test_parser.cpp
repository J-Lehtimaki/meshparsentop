#include "FEBoundary.hpp"
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

#include <vector>
#include <string>

TEST_CASE("FEBoudary"){

// FEBoundary constructor parameters
std::string pathPin =
"/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/pin";
std::string pathThread =
"/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/thread";
std::string pathPR =
"/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/pr";
std::string pathFea =
"/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/fea";

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

	SECTION("separate_floatstr_to_strvec"){
		std::string l1 =
		"-76.883763,10.575940,14.973368,-0.07688376307487,0.01057593990117,0.01497336849570";
		std::string l2 =
		"-73.586859,16.805306,13.948498,-0.07358685880899,0.01680530607700,0.01394849829376";
		std::string l3 =
		"-63.026294,21.271488,14.808376,-0.06302629411221,0.02127148769796,0.01480837631971";
		
		auto vec1 = parser.separateFloats(l1);
		auto vec2 = parser.separateFloats(l2);
		auto vec3 = parser.separateFloats(l3);

		REQUIRE(vec1 ==
			std::vector<std::string>({"-76.883763","10.575940","14.973368",
			"-0.07688376307487","0.01057593990117","0.01497336849570"}));

		REQUIRE(vec2 ==
			std::vector<std::string>({"-73.586859","16.805306","13.948498",
			"-0.07358685880899","0.01680530607700","0.01394849829376"}));

		REQUIRE(vec3 ==
			std::vector<std::string>({"-63.026294","21.271488","14.808376",
			"-0.06302629411221","0.02127148769796","0.01480837631971"}));
	}
	
	SECTION("practice_precision_conversion"){
		std::vector<std::string> svec= {
			"10000000000000000",
			"20000000000000",
			"300000",
			"4000",
			"5",
			"00000000000000006",
			"000070",
			"00008"
			};
		std::vector<int> ints = {};

		for(auto i : svec){
			if(i.length() <= 6){
				ints.push_back(std::stoi(i));
			}else{
				ints.push_back(std::stoi(i.substr(0,6)));
			}
		}

		REQUIRE(ints.size() == svec.size());
		REQUIRE(100000 == ints[0]);
		REQUIRE(200000 == ints[1]);
		REQUIRE(300000 == ints[2]);
		REQUIRE(4000 == ints[3]);
		REQUIRE(5 == ints[4]);
		REQUIRE(0 == ints[5]);
		REQUIRE(70 == ints[6]);
		REQUIRE(8 == ints[7]);
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
			REQUIRE(500000 == coord.second);
		}catch(const std::out_of_range& oor){
			WARN("Catch too large number. Message: " << oor.what());
		}
	}

	SECTION("convert_floatstring_to_intpair_vector"){
		std::string l1 =
		"-76.883763,10.575940,14.973368,-0.07688376307487,0.01057593990117,0.01497336849570";
		std::string l2 =
		"-73.586859,16.805306,13.948498,-0.07358685880899,0.01680530607700,0.01394849829376";
		std::string l3 =
		"-63.026294,21.271488,14.808376,-0.06302629411221,0.02127148769796,0.01480837631971";

		std::vector<std::pair<int,int>> coord1 = parser.convertLineToCoordinate(l1);
		std::vector<std::pair<int,int>> coord2 = parser.convertLineToCoordinate(l2);
		std::vector<std::pair<int,int>> coord3 = parser.convertLineToCoordinate(l3);

		INFO(	"c1 x: " << coord1[0].first << "_" << coord1[0].second << "\n" <<
				"c1 y: " << coord1[1].first << "_" << coord1[1].second << "\n" <<
				"c1 z: " << coord1[2].first << "_" << coord1[2].second << "\n");

		INFO(	"c2 x: " << coord2[0].first << "_" << coord2[0].second << "\n" <<
				"c2 y: " << coord2[1].first << "_" << coord2[1].second << "\n" <<
				"c2 z: " << coord2[2].first << "_" << coord2[2].second << "\n");

		INFO(	"c3 x: " << coord3[0].first << "_" << coord3[0].second << "\n" <<
				"c3 y: " << coord3[1].first << "_" << coord3[1].second << "\n" <<
				"c3 z: " << coord3[2].first << "_" << coord3[2].second << "\n");

		bool b1 =
			((coord1[0].first == -76) && (coord1[0].second == 883763) &&
			(coord1[1].first == 10) && (coord1[1].second == 575940) &&
			(coord1[2].first == 14) && (coord1[2].second == 973368));
		bool b2 =
			((coord2[0].first == -73) && (coord2[0].second == 586859) &&
			(coord2[1].first == 16) && (coord2[1].second == 805306) &&
			(coord2[2].first == 13) && (coord2[2].second == 948498));
		bool b3 =
			((coord3[0].first == -63) && (coord3[0].second == 26294) &&
			(coord3[1].first == 21) && (coord3[1].second == 271488) &&
			(coord3[2].first == 14) && (coord3[2].second == 808376));
		
		REQUIRE(true == b1);
		REQUIRE(true == b2);
		REQUIRE(true == b3);
	}

	SECTION("convert_floatstring_to_fea_intpair_vector"){
		std::string l1 =
			"-85.064448,9.745136,32.264508,236741.67187500000000";

		std::string l2 =
			"106.110722,10.489016,25.881486,671048.93750000000000";	
		
		std::vector<std::pair<int,int>> coord1 =
			parser.convertLineToFeaCoordinate(l1);
		std::vector<std::pair<int,int>> coord2 =
			parser.convertLineToFeaCoordinate(l2);

		INFO(	"c1 x: " << coord1[0].first << "_" << coord1[0].second << "\n" <<
				"c1 y: " << coord1[1].first << "_" << coord1[1].second << "\n" <<
				"c1 z: " << coord1[2].first << "_" << coord1[2].second << "\n");

		INFO(	"c2 x: " << coord2[0].first << "_" << coord2[0].second << "\n" <<
				"c2 y: " << coord2[1].first << "_" << coord2[1].second << "\n" <<
				"c2 z: " << coord2[2].first << "_" << coord2[2].second << "\n");

		bool b1 =
			((coord1[0].first == -85) && (coord1[0].second == 64448) &&
			(coord1[1].first == 9) && (coord1[1].second == 745136) &&
			(coord1[2].first == 32) && (coord1[2].second == 264508) &&
			(coord1[3].first == 236741) && (coord1[3].second == 671875));
		bool b2 =
			((coord2[0].first == 106) && (coord2[0].second == 110722) &&
			(coord2[1].first == 10) && (coord2[1].second == 489016) &&
			(coord2[2].first == 25) && (coord2[2].second == 881486) &&
			(coord2[3].first == 671048) && (coord2[3].second == 937500));
		
		REQUIRE(true == b1);
		REQUIRE(true == b2);
	}

} // TEST_CASE("Parserclass")

