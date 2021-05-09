#include <FEBoundary.hpp>
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

TEST_CASE("critical_node", "[crit]"){
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

	SECTION("cn_constr"){
		// Default constructor
		fe::CriticalNode a = fe::CriticalNode();
		REQUIRE(a.x.first == 0); REQUIRE(a.x.second == 0);
		REQUIRE(a.y.first == 0); REQUIRE(a.y.second == 0);
		REQUIRE(a.z.first == 0); REQUIRE(a.z.second == 0);

		// Constructor with specified coordinates
		fe::CriticalNode b = fe::CriticalNode(
			std::pair<int,int>(11,12),
			std::pair<int,int>(21,22),
			std::pair<int,int>(31,32)
		);
		REQUIRE(b.x.first == 11); REQUIRE(b.x.second == 12);
		REQUIRE(b.y.first == 21); REQUIRE(b.y.second == 22);
		REQUIRE(b.z.first == 31); REQUIRE(b.z.second == 32);
		
		SECTION("compare"){
			REQUIRE(a == a); REQUIRE(a != b); REQUIRE(b == b);
		}

		SECTION("compare2"){
			bool bo1(a == a);
			REQUIRE(true == bo1);
		}
	}
}

TEST_CASE("vmnode", "[light]"){
	std::string pathPin = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/pin";
	std::string pathThread = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/thread";
	std::string pathPR = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/pr";
	std::string pathFea = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust3/fea";

	SECTION("intersections_by_vector"){
	}

}

