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

private:
	std::vector<std::string> splittedRow_;
	std::string delimiter_;
	std::string dot_;
};	// Class
}	// namespace
