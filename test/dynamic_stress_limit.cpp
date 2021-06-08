#include <FEBoundary2.hpp>
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>
#include <memory>
#include <vector>
#include <iomanip>

TEST_CASE("dynamic_stress"){
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

	// dynamic_stress -c feasibility
    SECTION("feasibility"){
        WARN("Is feasible solution: " << feHandler.isFeasibleSolution());
    }

}
