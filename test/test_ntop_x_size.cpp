#include <FEBoundary.hpp>
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

TEST_CASE("files_with_unkown_line_count", "[light]"){
	std::string pathPin = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/pin";
	std::string pathThread = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/thread";
	std::string pathPR = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/pr";
	std::string pathFea = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/fea";

	fe::FEBoundary feHandler("myID", pathFea, pathPin, pathThread, pathPR);
	std::vector<fe::VonMisesNode> vonMisesVec = feHandler.getVonMisesFullRegion();
	std::vector<fe::CriticalNode> threadNodeVec = feHandler.getThreadNodes();
	std::vector<fe::CriticalNode> pinNodeVec = feHandler.getPinNodes();
	std::vector<fe::CriticalNode> prNodeVec = feHandler.getPRNodes();

	SECTION("constructor_added_id"){
		REQUIRE("myID" == feHandler.getID());
	}

	SECTION("datastructures_formed_from_files_in_constructor"){
		REQUIRE(4517 == threadNodeVec.size());		
		REQUIRE(11927 == pinNodeVec.size());
		REQUIRE(4380 == prNodeVec.size());
		REQUIRE(186191 == vonMisesVec.size());
	}
}

TEST_CASE("O2", "[heavy]"){
	std::string pathPin = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/pin";
	std::string pathThread = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/thread";
	std::string pathPR = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/pr";
	std::string pathFea = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/fea";

	fe::FEBoundary feHandler("myID", pathFea, pathPin, pathThread, pathPR);
	std::vector<fe::VonMisesNode> vonMisesVec = feHandler.getVonMisesFullRegion();
	std::vector<fe::CriticalNode> threadNodeVec = feHandler.getThreadNodes();
	std::vector<fe::CriticalNode> pinNodeVec = feHandler.getPinNodes();
	std::vector<fe::CriticalNode> prNodeVec = feHandler.getPRNodes();
	
	SECTION("intersections_by_vector"){
		REQUIRE(feHandler.getThreadNodes().size() == feHandler.intersectionCount(
			threadNodeVec, threadNodeVec));
		REQUIRE(feHandler.getThreadNodes().size() == feHandler.intersectionCount(
			threadNodeVec, vonMisesVec));
	}

}

