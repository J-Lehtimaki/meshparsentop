#include <FEBoundary.hpp>
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

TEST_CASE("test_name"){
	std::string name = "Jesse";
	REQUIRE(name == fe::FEBoundary(name).getID());
}

TEST_CASE("pareto_front_18_million"){
	unsigned int n = 18000000;
	REQUIRE(n == fe::FEBoundary("someName").iterateOverN(n));
}

// Test file reading with c++
// 364535 node fea results represented in the '../tabularData/fea'
TEST_CASE("fea_file_linecount"){
	unsigned lineCount = 364535;
	std::string pathFea = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/fea";
	REQUIRE(lineCount == fe::FEBoundary("someName").getLineCount(pathFea));
	REQUIRE(lineCount != fe::FEBoundary("someName").getLineCount(pathFea) - 1);
	REQUIRE(0 == fe::FEBoundary("someName").getLineCount(pathFea) - 364535);
}

TEST_CASE("constructor"){
	// 9628 lines in pin
	std::string pathPin = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/pin";
	// 16924 lines in thread
	std::string pathThread = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/thread";
	// 4667 lines in pr
	std::string pathPR = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/pr";
	// 364535 in fea
	std::string pathFea = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/fea";

	fe::FEBoundary feHandler("myID", pathFea, pathPin, pathThread, pathPR);
	REQUIRE("myID" == feHandler.getID());

	SECTION("datastructures_formed_from_files_in_constructor"){
		REQUIRE("1" == "1");
		REQUIRE(16924 == feHandler.getThreadNodes().size());		
		REQUIRE(9628 == feHandler.getPinNodes().size());
		REQUIRE(4667 == feHandler.getPRNodes().size());
		REQUIRE(364535 == feHandler.getVonMisesFullRegion().size());
	}
	SECTION("intersections_by_vector"){
		std::vector<fe::VonMisesNode> v0 = feHandler.getVonMisesFullRegion();
		std::vector<fe::CriticalNode> v1 = feHandler.getThreadNodes();
		std::vector<fe::CriticalNode> v2 = feHandler.getPinNodes();
		std::vector<fe::CriticalNode> v3 = feHandler.getPRNodes();

		REQUIRE(16924 == feHandler.intersectionCount(v1,v1));
		REQUIRE(9628 == feHandler.intersectionCount(v2,v2));
		REQUIRE(4667 == feHandler.intersectionCount(v3,v3));
		// Test that the critical regions are in vonMisesResults
		//REQUIRE(16924 == feHandler.intersectionCount(v1,v0));
		//REQUIRE(16924 == feHandler.intersectionCount(v0,v1));
		REQUIRE(9628 == feHandler.intersectionCount(v2,v0));
		REQUIRE(9628 == feHandler.intersectionCount(v0,v2));
		REQUIRE(4667 == feHandler.intersectionCount(v3,v0));
		REQUIRE(4667 == feHandler.intersectionCount(v0,v3));
		REQUIRE(feHandler.getThreadNodes().size() == feHandler.intersectionCount(v1,v0));
	}
	//SECTION("intersections_by_path"){
	//	unsigned expected = feHandler.getNodeDifferenceCount(pathFea, pathPin);
	//	CHECK(expected == feHandler.intersectionCount(pathFea, pathPin));
	//}

}

TEST_CASE("test_FEClass_with_real_FE_results"){
	// 9628 lines in pin
	std::string pathPin = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/pin";
	// 16924 lines in thread
	std::string pathThread = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/thread";
	// 4667 lines in pr
	std::string pathPR = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/pr";
	// 364535 in fea
	std::string pathFea = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/fea";

	auto feHandler = fe::FEBoundary("id");

	SECTION("absdiff_fea_pin"){
		unsigned preKnownCount = 364535 - 9628;
		CHECK(preKnownCount == feHandler.getNodeDifferenceCount(pathFea, pathPin));
		CHECK(preKnownCount == feHandler.getNodeDifferenceCount(pathPin, pathFea));
	}
	SECTION("absdiff_fea_thread"){
		unsigned preKnownCount = 364535 - 16924;
		CHECK(preKnownCount == feHandler.getNodeDifferenceCount(pathFea, pathThread));
		CHECK(preKnownCount == feHandler.getNodeDifferenceCount(pathThread, pathFea));
	}
	SECTION("absdiff_fea_pr)"){
		unsigned preKnownCount = 364535 - 4667;
		CHECK(preKnownCount == feHandler.getNodeDifferenceCount(pathFea, pathPR));
		CHECK(preKnownCount == feHandler.getNodeDifferenceCount(pathPR, pathFea));
	}
}

