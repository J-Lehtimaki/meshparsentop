#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <iostream>
#include <string>
#include <vector>

namespace lineparser{

class Parser{
public:
Parser(std::string delimiter, std::string dot) :
	delimiter_(delimiter), dot_(dot){}

const std::vector<std::string> separateFloats(std::string& s){
	this->splittedRow_ = {};
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter_)) != std::string::npos) {
	    token = s.substr(0, pos);
	    this->splittedRow_.push_back(token);
	    s.erase(0, pos + delimiter_.length());
	}
	splittedRow_.push_back(s);	// The last one can't be reached from while loop
	return this->splittedRow_;
	// return {"scott", "tiger", "mushroom"};
}

// Takes string "1252792.10259512059712509977"
// Outputs pair<int,int> (1252792,102596) decimal from max 6 characters
const std::pair<int,int> splitFloatStringToIntPair(std::string s){
	std::string sCopy = s;
	size_t pos = 0;
	std::string first;
	std::string second;
	while((pos = sCopy.find(dot_)) != std::string::npos){
		first = sCopy.substr(0, pos);
		sCopy.erase(0, pos + dot_.length());
	}
	second = sCopy;	// The remaining part after dot_ "21251._12412_"
	if(second.length() <= 6){
		return {std::stoi(first), std::stoi(second)};
	}else{
		return {std::stoi(first), std::stoi(second.substr(0,6))};
	}
}

// USE THIS ONLY WHEN YOU DON'T CARE ABOUT LEADING ZEROS IN THE DECIMALS
// THIS METHOD LOOSES ALL PRECISION FROM DOUBLES. "124.00010" -> (123,10)
// >>> Can be used to evaluate equality of datastructures generated from files
// >>> since all floats are converted in same fashion.
// Takes some "12512.6126126,126.1251,12525.12,125125.12512,1251.12515 <...>"
// Returns always the first 3 floats.
// Note: separator and dot have to be set correctly in this->constructor
// Note2: decimal is trimmed for leading zeros.
const std::vector<std::pair<int,int>> convertLineToCoordinate(std::string& s){
	std::vector<std::pair<int,int>> intPairs = {};
	auto splitDelimiterVec = this->separateFloats(s);
	for(auto floatString : splitDelimiterVec){
		auto intPair = this->splitFloatStringToIntPair(floatString);
		intPairs.push_back(intPair);
	}
	return {intPairs[0],intPairs[1],intPairs[2]};	// x, y, z
} 

// USE THIS ONLY WHEN YOU DON'T CARE ABOUT LEADING ZEROS IN THE DECIMALS
// THIS METHOD LOOSES ALL PRECISION FROM DOUBLES. "124.00010" -> (123,10)
// >>> Can be used to evaluate equality of datastructures generated from files
// >>> since all floats are converted in same fashion.
// Takes some "12512.6126126,126.1251,12525.12,125125.12512,1251.12515 <...>"
// Returns always the first 4 floats.
// Note: separator and dot have to be set correctly in this->constructor
// Note2: decimal is trimmed for leading zeros.
const std::vector<std::pair<int,int>> convertLineToFeaCoordinate(std::string& s){
	std::vector<std::pair<int,int>> intPairs = {};
	auto splitDelimiterVec = this->separateFloats(s);
	for(auto floatString : splitDelimiterVec){
		auto intPair = this->splitFloatStringToIntPair(floatString);
		intPairs.push_back(intPair);
	}
	return {intPairs[0],intPairs[1],intPairs[2],intPairs[3]};	// x, y, z, vm
} 


private:
	std::vector<std::string> splittedRow_;
	std::string delimiter_;
	std::string dot_;
};	// Class
}	// namespace

#endif

