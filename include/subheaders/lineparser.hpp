#ifndef LINEPARSER_H
#define LINEPARSeR_H

#include <iostream>
#include <string>
#include <vector>

namespace lineparser{

class Parser{
public:
Parser(std::string delimiter, std::string dot) :
	delimiter_(delimiter), dot_(dot){}

const std::vector<std::string> separateFloats(std::string& s){
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

// Takes string "125279.10259512059712509977"
// Outputs pair<int,int> (125279,102596) decimal from max 6 characters
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
	std::pair<int,int> floatIntPair(std::stoi(first), std::stoi(second));
	return floatIntPair;
}

const std::vector<std::pair<int,int>> convertLineToCoordinate(std::string& s){
	auto splitDelimiterVec = this->separateFloats(s);
	for(auto floatString : splitDelimiterVec){
		break;
	}
	return {{1,2},{3,4},{5,6}};
} 

private:
	std::vector<std::string> splittedRow_;
	std::string delimiter_;
	std::string dot_;
};	// Class
}	// namespace

#endif

