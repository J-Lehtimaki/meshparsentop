#include <FEBoundary2.hpp>
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>
#include <memory>
#include <vector>
#include <iomanip>

TEST_CASE("files_with_unkown_line_count"){
	std::string pathPin = "C:\\SoftwareDevelopment\\cmake catch\\test\\tabularData\\robust4\\pin";
	std::string pathThread = "C:\\SoftwareDevelopment\\cmake catch\\test\\tabularData\\robust4\\thread";
	std::string pathPR = "C:\\SoftwareDevelopment\\cmake catch\\test\\tabularData\\robust4\\pr";
	std::string pathFea = "C:\\SoftwareDevelopment\\cmake catch\\test\\tabularData\\robust4\\fea.csv";

    // Export paths for feasible solutions
    std::string pathExportSubtractRegion = "C:\\SoftwareDevelopment\\cmake catch\\test\\export\\subtracedExport.csv";
    std::string pathExportRegionData = "C:\\SoftwareDevelopment\\cmake catch\\test\\export\\regionData.csv";

	fe::FEBoundary feHandler(
        200000000,      // 100 MPa design limit stress for testing
        pathFea,
        pathPin,
        pathThread,
        pathPR,
        pathExportSubtractRegion,
        pathExportRegionData
    );

	SECTION("initialized_stress_distributions"){
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

    SECTION("is_feasible_100MPa"){
        WARN("Is feasible solution: " << feHandler.isFeasibleSolution());
    }

}
