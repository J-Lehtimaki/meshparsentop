#ifndef FEAPARSER_H
#define FEAPARSER_H

#include "subheaders/vonMisesNtopFileReader.hpp"
#include "subheaders/lineparser.hpp"
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

// nTopology's way to export lattice node coordinates
// xwhole,xdeci,ywhole,ydeci,zwhole,zdeci
struct Coordinate{
    Coordinate() :  x(std::pair<int,int>(0,0)),
                    y(std::pair<int,int>(0,0)),
                    z(std::pair<int,int>(0,0)){}
    Coordinate(	    std::pair<int,int> X,
                    std::pair<int,int> Y,
                    std::pair<int,int> Z	) :	x(X), y(Y), z(Z){}
    std::pair<int,int> x;
    std::pair<int,int> y;
    std::pair<int,int> z;
    bool operator==(Coordinate const & rhs) const {
        return
            (this->x.first == rhs.x.first) && (this->x.second == rhs.x.second) &&
            (this->y.first == rhs.y.first) && (this->y.second == rhs.y.second) &&
            (this->z.first == rhs.z.first) && (this->z.second == rhs.z.second);
    }
    bool operator!=(Coordinate const & rhs) const {
        return
            (this->x.first != rhs.x.first) || (this->x.second != rhs.x.second) ||
            (this->y.first != rhs.y.first) || (this->y.second != rhs.y.second) ||
            (this->z.first != rhs.z.first) || (this->z.second != rhs.z.second);
    }
    bool operator<(Coordinate const & rhs) const {
        return std::tie(
            x.first, x.second,
            y.first, y.second,
            z.first, z.second) <
        std::tie(
            rhs.x.first, rhs.x.second,
            rhs.y.first, rhs.y.second,
            rhs.z.first, rhs.z.second);
    }
    bool operator<(std::shared_ptr<Coordinate> const & rhs) const {
        return std::tie(
            x.first, x.second,
            y.first, y.second,
            z.first, z.second) <
        std::tie(
            rhs->x.first, rhs->x.second,
            rhs->y.first, rhs->y.second,
            rhs->z.first, rhs->z.second);
    }
};

struct FEMeshNode{
	FEMeshNode() :	coord(Coordinate()), stress(0){}
	FEMeshNode(Coordinate pCoord, unsigned int pStress) :
		coord(pCoord), stress(pStress){}
	Coordinate coord;
	unsigned int stress;
};

struct StressFrequencyDistribution{
	double lowerBoundStress;
	double upperBoundStress;
	unsigned nOccurance;
};

struct RegionStressData{
	RegionStressData() :
		maxStress(0),
		minStress(0),
		average(0),
		stressFrequencyDistribution({}),
		correspondingFEnodes({}){}
	double maxStress;
	double minStress;
	double average;
	std::vector<StressFrequencyDistribution> stressFrequencyDistribution;
	std::vector<std::shared_ptr<FEMeshNode>> correspondingFEnodes;
};

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
        /*
		this->initAllBoundaryRegionsVM();
		this->initAllBoundaryAveragesVM();
		this->sortAllBoundaryRegions();
		this->initBoundaryMinMaxStresses();
        */
		// Subtract pin, thread, PR from main mesh
		
		//this->subtractRegionFromSubtractMesh(this->nodeCoordsPin_);
		//this->subtractRegionFromSubtractMesh(this->nodeCoordsThread_);
		//this->subtractRegionFromSubtractMesh(this->nodeCoordsPR_);
	 	//this->finalizeSubtractedRegionVM();
		 
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

    /*
	const std::vector<FEMeshNode>& getVonMisesFullRegion(){return this->feNodes_;}
	const std::vector<Coordinate>& getPinNodes(){return this->nodeCoordsPin_;}
	const std::vector<Coordinate>& getThreadNodes(){return this->nodeCoordsThread_;}
	const std::vector<Coordinate>& getPRNodes(){return this->nodeCoordsPR_;}

	// Getters from boundary structs
	const double getPinAverage(){return this->pinRegionStress_.average;}
	const double getThreadAverage(){return this->threadRegionStress_.average;}
	const double getPrAverage(){return this->PrRegionStress_.average;}
	const double getSubMeshAverage(){return this->subtractedMesh_.average;}
	*/
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

	void initRegionsStresses(){

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

};	// class

}	// namespace
#endif