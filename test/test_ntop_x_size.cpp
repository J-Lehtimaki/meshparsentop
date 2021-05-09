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
	
	SECTION("end_values_vectors"){
		auto t = threadNodeVec[4516];
		auto pr = prNodeVec[4379];
		auto pin = pinNodeVec[11926];
		auto vm = vonMisesVec[186190];

		REQUIRE(107 == t.x.first);
		REQUIRE(961297 == t.x.second);
		REQUIRE(18 == t.y.first);
		REQUIRE(581288 == t.y.second);
		REQUIRE(18 == t.z.first);
		REQUIRE(123316 == t.z.second);
		
		REQUIRE(-57 == pr.x.first);
		REQUIRE(59456 == pr.x.second);
		REQUIRE(10 == pr.y.first);
		REQUIRE(135730 == pr.y.second);
		REQUIRE(14 == pr.z.first);
		REQUIRE(615310 == pr.z.second);
		
		REQUIRE(22 == pin.x.first);
		REQUIRE(461180 == pin.x.second);
		REQUIRE(24 == pin.y.first);
		REQUIRE(528321 == pin.y.second);
		REQUIRE(0 == pin.z.first);
		REQUIRE(692834 == pin.z.second);

		REQUIRE(76 == vm.coord.x.first);
		REQUIRE(241873 == vm.coord.x.second);
		REQUIRE(20 == vm.coord.y.first);
		REQUIRE(528365 == vm.coord.y.second);
		REQUIRE(40 == vm.coord.z.first);
		REQUIRE(49028 == vm.coord.z.second);
		REQUIRE(19395580 == vm.stress);
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

	SECTION("intersections_thread_fea"){
		unsigned intersections = feHandler.intersectionCount(
			threadNodeVec, vonMisesVec);
		
		REQUIRE(4517 == threadNodeVec.size());
	}

	SECTION("intersections_pin_fea"){
		unsigned intersections = feHandler.intersectionCount(
			pinNodeVec, vonMisesVec);
		REQUIRE(11927 == pinNodeVec.size());
	}

	SECTION("intersections_pr_fea"){
		unsigned intersections = feHandler.intersectionCount(
			prNodeVec, vonMisesVec);
		REQUIRE(4380 == prNodeVec.size());
	}

	SECTION("sort_fe_mesh"){
		auto vmPair = feHandler.sortFullMeshByStress();
		WARN(	"Minimum vMises: " << vmPair.first.stress << '\n' <<
				" Coords:" << '\n' <<
				"    xyz: (" <<
				vmPair.first.coord.x.first << ", " <<
				vmPair.first.coord.y.first << ", " <<
				vmPair.first.coord.z.first << ")" << '\n' << 
				"Maximum vMises: " << vmPair.second.stress << '\n' <<
				"  Coords:" << '\n' <<
				"    xyz: (" <<
				vmPair.second.coord.x.first << ", " <<
				vmPair.second.coord.y.first << ", " <<
				vmPair.second.coord.z.first << ")" << '\n'
			);
	}

}
