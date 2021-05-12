#include <FEBoundary.hpp>
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>
#include <memory>
#include <vector>
#include <iomanip>

const std::vector<int> INTS = {10,9,8,7,6,5,4,3,2,1,0};
float GLOBAL_RESULT = 0.0;
using SubVector = std::shared_ptr<std::vector<int>>;

std::vector<int> SUBSET_VECTOR = {};

// Calculates average recursively. Helps to manage that sum(vec_i) does not
// exceed integer range limit
double myRecursiveAverage(const std::vector<int>& vec){
	if(vec.size() == 1){
		return static_cast<double>(vec[0]);
	}
	if(vec.size() == 2){
		return (
			static_cast<double>(vec[0]) +
			static_cast<double>(vec[1])
			) / 2;
	}
	unsigned mid = vec.size() / 2;
	
	std::vector<int> firstHalf;
	std::copy(vec.begin(), std::next(vec.begin(), mid),
		std::back_inserter(firstHalf));
		
	std::vector<int> secondHalf;
	std::copy(std::next(std::next(vec.begin(), mid)), vec.end(),
		std::back_inserter(secondHalf));

	return(
		myRecursiveAverage(firstHalf) +
		myRecursiveAverage(secondHalf)
		) / 2;
}

const std::vector<int> getSubVec(const std::vector<int>& vec,
		unsigned l1, unsigned l2){
	auto iter1 = std::next(vec.begin(), l1);
	auto iter2 = std::next(vec.begin(), l2);
	std::vector<int> dest;
	std::copy(iter1, iter2, std::back_inserter(dest));
	return dest;
}

using VmItPair = std::pair<
		std::shared_ptr<fe::VonMisesNode>,
		std::shared_ptr<fe::VonMisesNode>
	>;

void printRegionStressData(fe::FEBoundary& feHandler, const std::string& id,
		std::vector<std::shared_ptr<fe::VonMisesNode>>& regionVec){
	std::vector<int> endDistanceIndex = {};
	for(unsigned i=1; i<=10; i++){
		endDistanceIndex.push_back(-10*i);
	} 
	for(auto i : endDistanceIndex){
		std::vector<std::shared_ptr<fe::VonMisesNode>> temp;
		std::copy(
			std::prev(regionVec.end(), i),
			regionVec.end(),
			std::back_inserter(temp)
		);
		WARN("Stress " << id << feHandler.regionStressAverage(temp) << " Pa");
	}
}

std::string GLOBAL_MESSAGE = "";
void hello(){
	GLOBAL_MESSAGE = "Hello, modified GLOBAL_MESSAGE";
}

TEST_CASE("files_with_unkown_line_count", "[light]"){
	std::string pathPin = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust4/pin";
	std::string pathThread = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust4/thread";
	std::string pathPR = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust4/pr";
	std::string pathFea = "/home/janne/Ohjelmistokehitys/C++/nTop/FEAparse/test/tabularData/robust4/fea";

	fe::FEBoundary feHandler("myID", pathFea, pathPin, pathThread, pathPR);
	std::vector<fe::VonMisesNode> vonMisesVec = feHandler.getVonMisesFullRegion();
	std::vector<fe::CriticalNode> threadNodeVec = feHandler.getThreadNodes();
	std::vector<fe::CriticalNode> pinNodeVec = feHandler.getPinNodes();
	std::vector<fe::CriticalNode> prNodeVec = feHandler.getPRNodes();

	SECTION("constructor_added_id"){
		REQUIRE("myID" == feHandler.getID());
	}

	SECTION("init_boundary_regions_vm"){
		feHandler.initAllBoundaryRegions();
		auto pinRegionVM = feHandler.getPinVonMises();
		auto threadRegionVM = feHandler.getThreadVonMises();
		auto prRegionVM = feHandler.getPrVonMises();

		printRegionStressData(feHandler, "PIN", pinRegionVM);
		printRegionStressData(feHandler, "THEARD", threadRegionVM);
		printRegionStressData(feHandler, "PR", prRegionVM);
	}

	SECTION("get_sub_vector"){
		auto result = getSubVec(INTS, 2, 5);
		for(auto value : result){
			WARN(value);
		}
	}

	SECTION("try_recursive"){
		WARN("My average: " << myRecursiveAverage(INTS));
		hello();
		WARN(GLOBAL_MESSAGE);
	}

}

