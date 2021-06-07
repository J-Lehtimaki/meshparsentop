#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "structs.hpp"

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

// Modified version, suitable for feboundary -class
void write_csv(fs::path filename, const std::vector<fe::StressFrequencyDistribution>& dataset){ 
    // Create an output filestream object
    std::ofstream myFile(filename);
    // Send data to the stream
    for(int j = 0; j < dataset.size(); ++j){
        myFile << dataset.at(j).upperBoundStress << "," << dataset.at(j).nOccurance << "\n";
    }
    // Close the file
    myFile.close();
}

// Overload. Writes plain stress values for each node of a given set
void write_csv_plain_region(fs::path filename, const std::vector<std::shared_ptr<fe::FEMeshNode>>& nodes){
    std::ofstream myFile(filename);
    for(int i = 0; 0 < nodes.size(); ++i){
        myFile << nodes.at(i)->stress << "\n";
    }
    myFile.close();
}

}

#endif