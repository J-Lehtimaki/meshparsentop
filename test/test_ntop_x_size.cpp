#include <FEBoundary.hpp>
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>


TEST_CASE("files_with_unkown_line_count"){
	std::string pathPin = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust1/pin";
	std::string pathThread = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust1/thread";
	std::string pathPR = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust1/pr";
	std::string pathFea = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust1/fea";

	fe::FEBoundary feHandler("myID", pathFea, pathPin, pathThread, pathPR);
	std::vector<fe::VonMisesNode> vonMisesVec = feHandler.getVonMisesFullRegion();
	std::vector<fe::CriticalNode> threadNodeVec = feHandler.getThreadNodes();
	std::vector<fe::CriticalNode> pinNodeVec = feHandler.getPinNodes();
	std::vector<fe::CriticalNode> prNodeVec = feHandler.getPRNodes();

	SECTION("constructor_added_id"){
		REQUIRE("myID" == feHandler.getID());
	}

	SECTION("datastructures_formed_from_files_in_constructor"){
		REQUIRE(threadNodeVec.size() != 0);
		REQUIRE(pinNodeVec.size() != 0);
		REQUIRE(prNodeVec.size() != 0);
		REQUIRE(vonMisesVec.size() != 0);
		REQUIRE(threadNodeVec.size() == feHandler.getThreadNodes().size());		
		REQUIRE(pinNodeVec.size() == feHandler.getPinNodes().size());
		REQUIRE(prNodeVec.size() == feHandler.getPRNodes().size());
		REQUIRE(vonMisesVec.size() == feHandler.getVonMisesFullRegion());
	}
	//SECTION("intersections_by_vector"){
		//REQUIRE(feHandler.getThreadNodes().size() =0 feHandler.intersectionCount(
	//		threadNodeVec, threadNodeVec));
		//REQUIRE(feHandler.getThreadNodes().size() == feHandler.intersectionCount(
	//		threadNodeVec, vonMisesVec));
	//}
	
}
