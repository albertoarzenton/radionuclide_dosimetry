/*
    Reading ROOT TNtuple file
    -------------------------

    #Author: A. Arzenton
    #Contact: alberto.arzenton@pd.infn.it

*/

#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <fstream>
#include <limits>

struct VoxelData {
    //double totalEnergy = 0;
    double totalDose = 0;
    std::vector<double> eventDoses;
};

void read_relSEM(const char* filename) {
    int totalEvents = 0;
    TFile *file = TFile::Open(filename, "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    TTree *tree = (TTree*)file->Get("tDose");
    if (!tree) {
        std::cerr << "Error: Cannot find TTree in file " << filename << std::endl;
        return;
    }

    int eventID, xIndex, yIndex, zIndex;
    //double energyHit
    double doseHit;

    tree->SetBranchAddress("fEventID", &eventID);
    tree->SetBranchAddress("fVoxelX", &xIndex);
    tree->SetBranchAddress("fVoxelY", &yIndex);
    tree->SetBranchAddress("fVoxelZ", &zIndex);
    //tree->SetBranchAddress("fEdep", &energyHit);
    tree->SetBranchAddress("fDose", &doseHit);

    std::map<std::tuple<int, int, int, int>, double> eventVoxelMap;
    std::map<std::tuple<int, int, int>, VoxelData> voxelMap;
    int maxX = 0, maxY = 0, maxZ = 0;

    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);
        std::tuple<int, int, int, int> key = {eventID, xIndex, yIndex, zIndex};
        eventVoxelMap[key] += doseHit;
        maxX = std::max(maxX, xIndex);
        maxY = std::max(maxY, yIndex);
        maxZ = std::max(maxZ, zIndex);
    }
    totalEvents = eventID;

    for (const auto& entry : eventVoxelMap) {
        auto [eventID, x, y, z] = entry.first;
        double totalDose = entry.second;
        auto& voxelData = voxelMap[{x, y, z}];
        voxelData.totalDose += totalDose;
        voxelData.eventDoses.push_back(totalDose);
    }

    std::ofstream outFile("relSEM_map.txt");

    for (int x = 0; x < maxX; ++x) {
        for (int y = 0; y < maxY; ++y) {
            for (int z = 0; z < maxZ; ++z) {
                auto it = voxelMap.find({x, y, z});
                double relSEM = 0;
                if (it != voxelMap.end() && it->second.totalDose > 0) {
                    double meanDose = it->second.totalDose / (totalEvents + 1);
                    double sumSquaredDiffs = 0;
                    for (double dose : it->second.eventDoses) {
                        sumSquaredDiffs += std::pow(dose - meanDose, 2);
                    }
                    double variance = std::sqrt(sumSquaredDiffs / totalEvents);
                    double sem = variance / std::sqrt(totalEvents);
                    relSEM = (meanDose > 0) ? (sem / meanDose) * 100 : 0;
                }
                outFile << relSEM << " ";
            }
            outFile << "\n";
        }
        outFile << "\n";
    }

    outFile.close();
    file->Close();
}
