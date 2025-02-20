#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    // Constructor & Destructor
    DetectorConstruction();
    virtual ~DetectorConstruction();

    // Mandatory class
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

  private:

    void DefineMaterials();
    G4VPhysicalVolume* ConstructDetector();

    // Materials
    G4Material* fWorldMaterial;
    G4Material* fVoxelMaterial;

    G4Material* fAir;
    G4Material* fLung;
    G4Material* fSoft;
    G4Material* fCBone;
    G4Material* fBone;
    // G4Material* fAdipose;
    // G4Material* fBrain;
    // G4Material* fBlood;
    // G4Material* fMuscle;

    // Logical volumes
    G4LogicalVolume* fLogicalWorld;

    // Physical Volumes
    G4VPhysicalVolume* PhysicsWorld;

    // Voxelized Phantom Parameters
    G4double halfVoxelSize;

    G4int nVoxelX;
    G4int nVoxelY;
    G4int nVoxelZ;

};

#endif
