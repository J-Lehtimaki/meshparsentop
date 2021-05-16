#include <FEBoundary2.hpp>
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>
#include <memory>
#include <vector>
#include <iomanip>

TEST_CASE("files_with_unkown_line_count"){
	/*
	std::string pathPin = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust4/pin";
	std::string pathThread = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust4/thread";
	std::string pathPR = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust4/pr";
	std::string pathFea = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust4/fea";
	*/
	std::string pathPin = "C:\\SoftwareDevelopment\\cmake catch\\test\\tabularData\\robust4\\pin";
	std::string pathThread = "C:\\SoftwareDevelopment\\cmake catch\\test\\tabularData\\robust4\\thread";
	std::string pathPR = "C:\\SoftwareDevelopment\\cmake catch\\test\\tabularData\\robust4\\pr";
	std::string pathFea = "C:\\SoftwareDevelopment\\cmake catch\\test\\tabularData\\robust4\\fea";

	fe::FEBoundary feHandler("myID", pathFea, pathPin, pathThread, pathPR);

	SECTION("constructor_added_id"){
		REQUIRE("myID" == feHandler.getID());
	}

	SECTION("test_region_sizes"){
		unsigned pinSize = feHandler.getPinIntersectionSize();
		unsigned threadSize = feHandler.getThreadIntersectionSize();
		unsigned prSize = feHandler.getPrIntersectionSize();
		unsigned subtractedSize = feHandler.getSubtractRegionIntersectionSize();

		WARN("PIN size: " << pinSize << "\nTHREAD size: " <<
		threadSize << "\nPR size: " <<
		prSize << "\nSUBTRACT MESH size: " <<
		subtractedSize);
		// 207648 = lines in robust4/fea
		REQUIRE(0 == 207648 - pinSize - threadSize - prSize - subtractedSize);
	}

	SECTION("min_max_regions"){
		WARN("PIN Min: " << feHandler.getPinMinMax().first << " Max: " << 
		feHandler.getPinMinMax().second);

		WARN("THREAD Min: " << feHandler.getThreadMinMax().first << " Max: " <<
		feHandler.getPinMinMax().second);

		WARN("PR Min: " << feHandler.getPrMinMax().first << " Max: " <<
		feHandler.getPrMinMax().second);

		WARN("SUBTRACTED Min: " << feHandler.getSubMeshMinMax().first << " Max: " <<
		feHandler.getSubMeshMinMax().second);
	}

	SECTION("init_stress_distributions"){
		auto distrSub = feHandler.getSubtractRegionDistribution();
		auto distrPin = feHandler.getPinRegionDistribution();
		auto distrThread = feHandler.getThreadRegionDistribution();
		auto distrPr = feHandler.getPrRegionDistribution();
		for(auto d : distrSub){
			WARN("SUBTRACT REGION <Min, Max> (" << d.lowerBoundStress << "," << d.upperBoundStress <<
			") - Nodes in range: " << d.nOccurance);
		}
		for(auto d : distrPin){
			WARN("PIN REGION <Min, Max> (" << d.lowerBoundStress << "," << d.upperBoundStress <<
			") - Nodes in range: " << d.nOccurance);
		}
		for(auto d : distrThread){
			WARN("THREAD REGION <Min, Max> (" << d.lowerBoundStress << "," << d.upperBoundStress <<
			") - Nodes in range: " << d.nOccurance);
		}
		for(auto d : distrPr){
			WARN("PR REGION <Min, Max> (" << d.lowerBoundStress << "," << d.upperBoundStress <<
			") - Nodes in range: " << d.nOccurance);
		}
	}

    /*
	SECTION("pin_average"){
		WARN("Pin average: " << feHandler.getPinAverage() <<
		" Pa\nMin: " << feHandler.getPinMinMax().first << " Max: " << 
		feHandler.getPinMinMax().second);
	}

	SECTION("thread_average"){
		WARN("Thread average: " << feHandler.getThreadAverage() <<
		" Pa\nMin: " << feHandler.getThreadMinMax().first << " Max: " <<
		feHandler.getPinMinMax().second);
	}

	SECTION("pr_average"){
		WARN("PR average: " << feHandler.getPrAverage() <<
		" Pa\nMin: " << feHandler.getPrMinMax().first << " Max: " <<
		feHandler.getPrMinMax().second);
	}

	SECTION("subMesh_average"){
		WARN("SubMesh average: " << feHandler.getSubMeshAverage() <<
		" Pa\nMin: " << feHandler.getSubMeshMinMax().first << " Max: " <<
		feHandler.getSubMeshMinMax().second);	
	}
    */
}
