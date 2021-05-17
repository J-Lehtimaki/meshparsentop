#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>
#include <memory>

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
	StressFrequencyDistribution(
		unsigned l,
		unsigned h,
		unsigned n) :
			lowerBoundStress(l),
			upperBoundStress(h),
			nOccurance(n){}
	unsigned lowerBoundStress;
	unsigned upperBoundStress;
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

}

#endif