#ifndef PARAMETERS_HH
#define PARAMETERS_HH

#include "G4SystemOfUnits.hh"

namespace Parameters {

    // CT parameters
    constexpr G4int cVoxelX = 148;
    constexpr G4int cVoxelY = 160;
    constexpr G4int cVoxelZ = 160;
    constexpr G4double cVoxelSize = 0.5*mm;

    // HU intervals
    constexpr G4int HUminLung = -3100;
    constexpr G4int HUminSoft = -200;
    constexpr G4int HUminBone = 2900;

    // PET or SPECT parameters
    constexpr G4int eVoxelX = 148;
    constexpr G4int eVoxelY = 160;
    constexpr G4int eVoxelZ = 160;
    constexpr G4double eVoxelSize = 0.5*mm;

}

#endif
