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

namespace fs = std::filesystem;

namespace fe{

// nTopology's way to export lattice node coordinates
// xwhole,xdeci,ywhole,ydeci,zwhole,zdeci
struct CriticalNode{
	std::pair<int,int> x;
	std::pair<int,int> y;
	std::pair<int,int> z;
	bool operator==(CriticalNode const & rhs) const {
		return
			(this->x.first == rhs.x.first) && (this->x.second == rhs.x.second) &&
			(this->y.first == rhs.y.first) && (this->y.second == rhs.y.second) &&
			(this->z.first == rhs.z.first) && (this->z.second == rhs.z.second);
	}
};
struct VonMisesNode{
	std::pair<int,int> x;
	std::pair<int,int> y;
	std::pair<int,int> z;
	int stress;
};
inline bool operator==(const CriticalNode& lhs, const VonMisesNode& rhs){
	return
		(lhs.x.first == rhs.x.first) && (lhs.x.second == rhs.x.second) &&
		(lhs.y.first == rhs.y.first) && (lhs.y.second == rhs.y.second) &&
		(lhs.z.first == rhs.z.first) && (lhs.z.second == rhs.z.second);	
};
inline bool operator==(const VonMisesNode& lhs, const CriticalNode& rhs){
	return
		(lhs.x.first == rhs.x.first) && (lhs.x.second == rhs.x.second) &&
		(lhs.y.first == rhs.y.first) && (lhs.y.second == rhs.y.second) &&
		(lhs.z.first == rhs.z.first) && (lhs.z.second == rhs.z.second);	
};



using regionPathLink = std::pair<fs::path, std::vector<CriticalNode>*>;

class FEBoundary{
public:
	FEBoundary(
		std::string id) :
			id_(id),
			parser_(lineparser::Parser(",",".")){}
	FEBoundary(
		std::string id,
		std::string FEfullPath,
		std::string pinPath,
		std::string threadPath,
		std::string PRPath) :
			id_(id),
			FEMeshFull_(fs::path(FEfullPath)),
			nodesPin_(fs::path(pinPath)),
			nodesValveThread_(fs::path(threadPath)),
			nodesPR_(fs::path(PRPath)),
			parser_(lineparser::Parser(",",".")){
	// TODO: Replace with reading paths from folders in future
	// TODO: Rework member variables
	this->initCriticalRegions({
		{this->nodesPin_, &this->nodeCoordsPin_},
		{this->nodesValveThread_, &this->nodeCoordsThread_},
		{this->nodesPR_, &this->nodeCoordsPR_}
		});
	this->initVonMisesResults();
	}

	std::string getID(){return id_;}
	const std::vector<VonMisesNode>& getVonMisesFullRegion(){return this->vonMisesNodes_;}
	const std::vector<CriticalNode>& getPinNodes(){return this->nodeCoordsPin_;}
	const std::vector<CriticalNode>& getThreadNodes(){return this->nodeCoordsThread_;}
	const std::vector<CriticalNode>& getPRNodes(){return this->nodeCoordsPR_;}
	
	// TODO: Shift these to external file
	int getSubfolderConstant(){return vm::SUBCONST;}
	unsigned int iterateOverN(unsigned int n){
		unsigned int result = 0;
		for (unsigned int i=0; i<n; i++) {
			result++;
		}
		return result;
	}
	unsigned getLineCount(const std::string path){
		std::ifstream infile(path);		// Open path for reading
		unsigned infileLineCount = 0;		// Record line count here
		std::string line;				// Manipulate this with std::getline
		while(std::getline(infile, line)){
			infileLineCount++;
		}
		infile.close();
		return infileLineCount;
	}

	const std::vector<std::string> parseFloatsDelimitedString(std::string& s){
		return this->parser_.separateFloats(s);
	}

	 // TODO:: All the way until here

	// Returns the absolute value difference of line count with two files.
	// Param 1 & 2, paths to files to be compared
	unsigned getNodeDifferenceCount(const std::string firstPath,
									  const std::string secondPath){
		unsigned firstNodeCount = this->getLineCount(firstPath);
		unsigned secondNodeCount = this->getLineCount(secondPath);
		return std::abs(static_cast<int>(firstNodeCount - secondNodeCount));
	}
	
	// Checks if critical regions intersect. Returns count of intersections.
	unsigned intersectionCount(	const std::vector<CriticalNode>& vec1,
								const std::vector<CriticalNode>& vec2){
		std::vector<CriticalNode> intersections = {};
		for(auto n1 : vec1){
			for(auto n2 : vec2){
				if(n1 == n2){
					intersections.push_back(n1);
				};
			}
		}
		return intersections.size();
	}
	// Returns count how many critical region nodes intersect with
	// VonMisesCoordinates (FEMeshFull)
	unsigned intersectionCount( const std::vector<CriticalNode>& vec1,
								const std::vector<VonMisesNode>& vec2){
		std::vector<CriticalNode> intersections = {};
		for(auto n1 : vec1){
			for(auto n2 : vec2){
				if(n1 == n2){
					intersections.push_back(n1);
				}
			}
		}
		return intersections.size();
	}
	// Returns count how many critical region nodes intersect with
	// VonMisesCoordinates (FEMeshFull)
	unsigned intersectionCount( const std::vector<VonMisesNode>& vec1,
								const std::vector<CriticalNode>& vec2){
		std::vector<VonMisesNode> intersections = {};
		for(auto n1 : vec1){
			for(auto n2 : vec2){
				if(n1 == n2){
					intersections.push_back(n1);
				}
			}
		}
		return intersections.size();
	}

	unsigned intersectionCount(std::string firstPath, std::string secondPath){
		return 123;
	}
private:
	lineparser::Parser parser_;
	
	// Multigoal optimization case ID
	std::string id_;

	// Full FE region vonMises result filepath
	fs::path FEMeshFull_;

	// Critical region nodes filepaths
	fs::path nodesPin_;
	fs::path nodesValveThread_;
	fs::path nodesPR_;

	// vonMises results
	std::vector<VonMisesNode> vonMisesNodes_;
	
	// Node coordinates for critical regions
	std::vector<CriticalNode> nodeCoordsPin_;
	std::vector<CriticalNode> nodeCoordsThread_;
	std::vector<CriticalNode> nodeCoordsPR_;

	// Takes a set of paths and references to CriticalNode-vector and then
	// pushes all the values from file to the datastructure that was passed
	void initCriticalRegions(std::vector<regionPathLink> criticalRegions){
		for(auto pair : criticalRegions){
			std::ifstream infile(pair.first);
			std::string line;				// Manipulate this with std::getline
			while(std::getline(infile, line)){
				std::istringstream iss(line);
				int x1, x2, y1, y2, z1, z2;		// Actual coordinates
				int xD1, xD2, yD1, yD2, zD1, zD2;	// Unknown nTopdata in line
				char deci, delim;
				pair.second->push_back({
					std::pair(static_cast<int>(x1), static_cast<int>(x2)),
					std::pair(static_cast<int>(y1), static_cast<int>(y2)),
					std::pair(static_cast<int>(z1), static_cast<int>(z2))
					});
			}
			infile.close();
		}
	}

	void initVonMisesResults(){
		std::ifstream infile(this->FEMeshFull_);
		std::string line;
		while(std::getline(infile, line)){
			std::istringstream iss(line);
			int x1, x2, y1, y2, z1, z2, vm1, vm2;
			char deci, delim;
			if(!(iss >>
					x1 >> deci >> x2 >> delim >>
					y1 >> deci >> y2 >> delim >>
					z1 >> deci >> z2 >> delim >>
					vm1 >> deci >> vm2) &&
						(deci == '.') && (delim == ',')){break;}
			this->vonMisesNodes_.push_back({
				std::pair(static_cast<int>(x1), static_cast<int>(x2)),
				std::pair(static_cast<int>(y1), static_cast<int>(y2)),
				std::pair(static_cast<int>(z1), static_cast<int>(z2)),
				static_cast<int>(vm1)
				});
			}
		infile.close();
	}

};

}
#endif
