#ifndef FEAPARSER_H
#define FEAPARSER_H

#include "subheaders/vonMisesNtopFileReader.hpp"
#include "subheaders/lineparser.hpp"
#include "subheaders/constants.hpp"
#include "subheaders/filewriter.hpp"
#include "subheaders/structs.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <cmath>

namespace fs = std::filesystem;

namespace fe{

using regionPathLink = std::pair<fs::path, std::vector<Coordinate>*>;

class FEBoundary{
public:
	FEBoundary(
		std::string id,
		std::string FEfullPath,
		std::string pinPath,
		std::string threadPath,
		std::string PRPath) :
			id_(id),
			meshPath_(fs::path(FEfullPath)),
			pinPath_(fs::path(pinPath)),
			threadPath_(fs::path(threadPath)),
			prPath_(fs::path(PRPath)),
			parser_(lineparser::Parser(",",".")),
			subtractedMesh_(RegionStressData()),
			pinRegionStress_(RegionStressData()),
			threadRegionStress_(RegionStressData()),
			PrRegionStress_(RegionStressData()){

		this->initSubRegionCoordinates({
			{this->pinPath_, &this->nodeCoordsPin_},
			{this->threadPath_, &this->nodeCoordsThread_},
			{this->prPath_, &this->nodeCoordsPR_}
		});
		this->initFEMesh();
		this->initRegionIntersections();
		this->sortAllRegions();
		this->setRegionAverages();
		this->initAllRegionStressDistributions();
		this->exportStressDistributions();
	}

	std::string getID(){return id_;}

	const unsigned getPinIntersectionSize(){
		return static_cast<unsigned>(this->pinRegionStress_.correspondingFEnodes.size());
	}
	const unsigned getThreadIntersectionSize(){
		return static_cast<unsigned>(this->threadRegionStress_.correspondingFEnodes.size());
	}
	const unsigned getPrIntersectionSize(){
		return static_cast<unsigned>(this->PrRegionStress_.correspondingFEnodes.size());
	}
	const unsigned getSubtractRegionIntersectionSize(){
		return static_cast<unsigned>(this->subtractedMesh_.correspondingFEnodes.size());
	}
	const std::vector<StressFrequencyDistribution> getSubtractRegionDistribution(){
		return this->subtractedMesh_.stressFrequencyDistribution;
	}
	const std::vector<StressFrequencyDistribution> getPinRegionDistribution(){
		return this->pinRegionStress_.stressFrequencyDistribution;
	}
	const std::vector<StressFrequencyDistribution> getThreadRegionDistribution(){
		return this->threadRegionStress_.stressFrequencyDistribution;
	}
	const std::vector<StressFrequencyDistribution> getPrRegionDistribution(){
		return this->PrRegionStress_.stressFrequencyDistribution;
	}
	const double getPinAverage(){
		return myRecursiveAverage(this->pinRegionStress_.correspondingFEnodes);
	}
	const double getThreadAverage(){
		return myRecursiveAverage(this->threadRegionStress_.correspondingFEnodes);
	}
	const double getPrAverage(){
		return myRecursiveAverage(this->PrRegionStress_.correspondingFEnodes);
	}
	const double getSubMeshAverage(){
		return myRecursiveAverage(this->subtractedMesh_.correspondingFEnodes);
	}

	const std::pair<double,double> getPinMinMax(){
		return std::make_pair(
			this->pinRegionStress_.minStress,
			this->pinRegionStress_.maxStress
		);
	}
	const std::pair<double,double> getThreadMinMax(){
		return std::make_pair(
			this->threadRegionStress_.minStress,
			this->threadRegionStress_.maxStress
		);
	}
	const std::pair<double,double> getPrMinMax(){
		return std::make_pair(
			this->PrRegionStress_.minStress,
			this->PrRegionStress_.maxStress
		);
	}
	const std::pair<double,double> getSubMeshMinMax(){
		return std::make_pair(
			this->subtractedMesh_.minStress,
			this->subtractedMesh_.maxStress
		);
	}
	bool initAllRegionStressDistributions(){
		this->initRegionStressDistributions(this->subtractedMesh_);
		this->initRegionStressDistributions(this->pinRegionStress_);
		this->initRegionStressDistributions(this->threadRegionStress_);
		this->initRegionStressDistributions(this->PrRegionStress_);
		if(this->subtractedMesh_.stressFrequencyDistribution.size() == 0 ||
			this->pinRegionStress_.stressFrequencyDistribution.size() == 0 ||
			this->threadRegionStress_.stressFrequencyDistribution.size() == 0 ||
			this->PrRegionStress_.stressFrequencyDistribution.size() == 0){
				return false;
		}
		return true;
	}

private:
	lineparser::Parser parser_;

	std::string id_;		// Multigoal optimization case ID

	fs::path meshPath_;		// FE results
	fs::path pinPath_;		// extracted lattice node coordinates
	fs::path threadPath_;	// extracted lattice node coordinates
	fs::path prPath_;		// extracted lattice node coordinates

	std::vector<FEMeshNode> feNodes_;		// Main container for saving
	std::map<Coordinate, std::shared_ptr<FEMeshNode>> mapFeNodes_;	// Container for searches
	std::map<Coordinate, std::shared_ptr<FEMeshNode>> subtractHelperFEnodeContainer_;

	// Node coordinates for critical regions
	std::vector<Coordinate> nodeCoordsPin_;
	std::vector<Coordinate> nodeCoordsThread_;
	std::vector<Coordinate> nodeCoordsPR_;

	// Region info to be saved to file
	RegionStressData subtractedMesh_;
	RegionStressData pinRegionStress_;
	RegionStressData threadRegionStress_;
	RegionStressData PrRegionStress_;

	/* MEMBER FUNCTIONS PRIVATE */

	void initRegionStressDistributions(RegionStressData& region){
		// PIN
		// Find the range iterators where nodes will be counter to freqDistr
		for(auto limits : feConstants::FREQUENCY_DISTRIBUTION_LIMITS){
			// temp variable for counting the occurances of nodes in range
			unsigned nCount = 0;
			auto lowIt = std::find_if(
				region.correspondingFEnodes.begin(),
				region.correspondingFEnodes.end(),
				[&](std::shared_ptr<FEMeshNode>& a){
				return a->stress > limits.first;
			});
			auto highIt = std::find_if(
				region.correspondingFEnodes.begin(),
				region.correspondingFEnodes.end(),
				[&](std::shared_ptr<FEMeshNode>& a){
				return a->stress > limits.second;
			});
			if(lowIt!=region.correspondingFEnodes.end() && lowIt!=highIt){
				// Iterate over region and count the occaurances
				while(lowIt != region.correspondingFEnodes.end()){
					if((*lowIt)->stress > limits.second){break;}
					nCount++;
					lowIt++;
				}
			}
			// Save results
			region.stressFrequencyDistribution.push_back(
				StressFrequencyDistribution(limits.first, limits.second, nCount)
			);
		}
	}

	void initRegionIntersections(){
		// PIN
		for(auto c : this->nodeCoordsPin_){
			auto search = this->subtractHelperFEnodeContainer_.find(c);
			if(search != this->subtractHelperFEnodeContainer_.end()){
				this->pinRegionStress_.correspondingFEnodes.push_back(
					this->subtractHelperFEnodeContainer_.at(c)
				);
				this->subtractHelperFEnodeContainer_.erase(search);
			}
		}
		// THREAD
		for(auto c : this->nodeCoordsThread_){
			auto search = this->subtractHelperFEnodeContainer_.find(c);
			if(search != this->subtractHelperFEnodeContainer_.end()){
				this->threadRegionStress_.correspondingFEnodes.push_back(
					this->subtractHelperFEnodeContainer_.at(c)
				);
				this->subtractHelperFEnodeContainer_.erase(search);
			}
		}
		// PR
		for(auto c : this->nodeCoordsPR_){
			auto search = this->subtractHelperFEnodeContainer_.find(c);
			if(search != this->subtractHelperFEnodeContainer_.end()){
				this->PrRegionStress_.correspondingFEnodes.push_back(
					this->subtractHelperFEnodeContainer_.at(c)
				);
				this->subtractHelperFEnodeContainer_.erase(search);
			}
		}
		// Save helper map to subtracted stress region data, and then delete it
		// in order to save memory
		for(auto pair : this->subtractHelperFEnodeContainer_){
			this->subtractedMesh_.correspondingFEnodes.push_back(pair.second);
		}
		this->subtractHelperFEnodeContainer_.clear();
	}


	// Takes a set of paths and references to Coordinate-vector and then
	// pushes all the values from file to the datastructure that was passed
	void initSubRegionCoordinates(std::vector<regionPathLink> criticalRegions){
		for(auto pair : criticalRegions){
			std::ifstream infile(pair.first);
			std::string line;				// Manipulate this with std::getline
			while(std::getline(infile, line)){
				std::vector<std::pair<int,int>> coord;
				try{
					coord = this->parser_.convertLineToCoordinate(line);
				}catch(const std::out_of_range& oor){
					std::cerr << oor.what() << std::endl;
				}
				Coordinate Coordinate = {
					coord[0], coord[1], coord[2]
				};
				pair.second->push_back(Coordinate);
			}
			infile.close();
		}
	}

	void initFEMesh(){
		std::ifstream infile(this->meshPath_);
		std::string line;
		while(std::getline(infile, line)){
			std::string copyLine = line;
			std::vector<std::string> sVec;
			std::vector<std::pair<int,int>> coord;
			unsigned int vmStress;
			try{
				sVec = this->parser_.separateDelimiter(line);
				coord = this->parser_.extractCoordinatesFromStrVec(sVec);
			}catch(const std::out_of_range& oor){
				std::cerr << oor.what() << std::endl;
			}
			try{
				vmStress = this->parser_.getlineVonMisesStress(copyLine);
				FEMeshNode feNode = {
					{coord[0], coord[1], coord[2]}, vmStress	//x,y,z,stress
				};
				this->feNodes_.push_back(feNode);
				this->mapFeNodes_.insert({{feNode.coord},{std::make_shared<FEMeshNode>(feNode)}});
			}catch(std::out_of_range& oor){
				std::cerr << oor.what() << std::endl;
			}
		}
		infile.close();

		this->subtractHelperFEnodeContainer_ = this->mapFeNodes_;
		
	}

	void sortAllRegions(){
		// THREAD sort
		std::sort(std::begin(this->threadRegionStress_.correspondingFEnodes),
			std::end(this->threadRegionStress_.correspondingFEnodes),
				[](std::shared_ptr<FEMeshNode> a,
				std::shared_ptr<FEMeshNode> b){
				return a->stress < b->stress;
		});		// PIN sort
		std::sort(std::begin(this->pinRegionStress_.correspondingFEnodes),
			std::end(this->pinRegionStress_.correspondingFEnodes),
				[](std::shared_ptr<FEMeshNode> a,
				std::shared_ptr<FEMeshNode> b){
				return a->stress < b->stress;
		});		// PR sort
		std::sort(std::begin(this->PrRegionStress_.correspondingFEnodes),
			std::end(this->PrRegionStress_.correspondingFEnodes),
				[](std::shared_ptr<FEMeshNode> a,
				std::shared_ptr<FEMeshNode> b){
				return a->stress < b->stress;
		});		// SUBTRACTED MESH sort
		std::sort(std::begin(this->subtractedMesh_.correspondingFEnodes),
			std::end(this->subtractedMesh_.correspondingFEnodes),
				[](std::shared_ptr<FEMeshNode> a,
				std::shared_ptr<FEMeshNode> b){
				return a->stress < b->stress;
		});
	}

	void setRegionAverages(){
		// PIN
		size_t sPin = this->pinRegionStress_.correspondingFEnodes.size();
		this->pinRegionStress_.maxStress = this->pinRegionStress_.correspondingFEnodes[sPin-1]->stress;
		this->pinRegionStress_.minStress = this->pinRegionStress_.correspondingFEnodes[0]->stress;
		// THREAD
		size_t sThread = this->threadRegionStress_.correspondingFEnodes.size();
		this->threadRegionStress_.maxStress = this->threadRegionStress_.correspondingFEnodes[sThread-1]->stress;
		this->threadRegionStress_.minStress = this->threadRegionStress_.correspondingFEnodes[0]->stress;
		// PR
		size_t sPr = this->PrRegionStress_.correspondingFEnodes.size();
		this->PrRegionStress_.maxStress = this->PrRegionStress_.correspondingFEnodes[sPr-1]->stress;
		this->PrRegionStress_.minStress = this->PrRegionStress_.correspondingFEnodes[0]->stress;
		// SUBTRACT REGION
		size_t sSubstracted = this->subtractedMesh_.correspondingFEnodes.size();
		this->subtractedMesh_.maxStress = this->subtractedMesh_.correspondingFEnodes[sSubstracted-1]->stress;
		this->subtractedMesh_.minStress = this->subtractedMesh_.correspondingFEnodes[0]->stress;
	}

	// Calculates average recursively. Helps to manage that sum(vec_i) does not
	// exceed integer range limit
	double myRecursiveAverage(const std::vector<std::shared_ptr<FEMeshNode>>& vec){
		if(vec.size() == 1){
			return static_cast<double>(vec[0]->stress);
		}
		if(vec.size() == 2){
			return (
				static_cast<double>(vec[0]->stress) +
				static_cast<double>(vec[1]->stress)
				) / 2;
		}
		unsigned mid = vec.size() / 2;
		
		std::vector<std::shared_ptr<FEMeshNode>> firstHalf;
		std::copy(vec.begin(), std::next(vec.begin(), mid),
			std::back_inserter(firstHalf));
			
		std::vector<std::shared_ptr<FEMeshNode>> secondHalf;
		std::copy(std::next(std::next(vec.begin(), mid)), vec.end(),
			std::back_inserter(secondHalf));
	
		return(
			myRecursiveAverage(firstHalf) +
			myRecursiveAverage(secondHalf)
			) / 2;
	}

	void exportStressDistributions(){
		writer::write_csv("C:\\SoftwareDevelopment\\cmake catch\\test\\export\\subtracedExport.csv",
			subtractedMesh_.stressFrequencyDistribution);
		writer::write_csv("C:\\SoftwareDevelopment\\cmake catch\\test\\export\\threadExport.csv",
			threadRegionStress_.stressFrequencyDistribution);
		writer::write_csv("C:\\SoftwareDevelopment\\cmake catch\\test\\export\\pinExport.csv",
			pinRegionStress_.stressFrequencyDistribution);
		writer::write_csv("C:\\SoftwareDevelopment\\cmake catch\\test\\export\\prExport.csv",
			PrRegionStress_.stressFrequencyDistribution);

		// .stressFrequencyDistribution
		//RegionStressData subtractedMesh_;
		//RegionStressData pinRegionStress_;
		//RegionStressData threadRegionStress_;
		//RegionStressData PrRegionStress_;
	}

};	// class

}	// namespace
#endif
