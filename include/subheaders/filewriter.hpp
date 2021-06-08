#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "structs.hpp"

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace writer{

void writeStressAttributeDataCSV(
        const fs::path& filename,
        const unsigned& minStress,
        const unsigned& maxStress,
        const unsigned& average){
    std::ofstream myFile(filename);
    myFile << "minStress;" << minStress << "\n";
    myFile << "maxStress;" << maxStress << "\n";
    myFile << "averageStress;" << average << "\n";
    myFile.close();
}

void writeRegionNodeStressCSV(
        fs::path filename,
        const std::vector<std::shared_ptr<fe::FEMeshNode>>& nodes){
    std::ofstream myFile(filename);
    for(int i = 0; i < nodes.size(); ++i){
        myFile << nodes.at(i)->stress << "\n";
    }
    myFile.close();
}

}

#endif
