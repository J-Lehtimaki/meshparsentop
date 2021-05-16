#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <utility> // std::pair

namespace fs = std::filesystem;

namespace writer{

// Make a CSV file with one or more columns of integer values
// Each column of data is represented by the pair <column name, column data>
//   as std::pair<std::string, std::vector<int>>
// The dataset is represented as a vector of these columns
// Note that all columns should be the same size
void write_csv(fs::path filename, std::vector<std::pair<std::string, std::vector<int>>> dataset){ 
    // Create an output filestream object
    std::ofstream myFile(filename);
    
    // Send column names to the stream
    for(int j = 0; j < dataset.size(); ++j){
        myFile << dataset.at(j).first;
        if(j != dataset.size() - 1) myFile << ","; // No comma at end of line
    }
    myFile << "\n";
    // Send data to the stream
    for(int i = 0; i < dataset.at(0).second.size(); ++i){
        for(int j = 0; j < dataset.size(); ++j){
            myFile << dataset.at(j).second.at(i);
            if(j != dataset.size() - 1) myFile << ","; // No comma at end of line
        }
        myFile << "\n";
    }
    // Close the file
    myFile.close();
}
}

#endif
