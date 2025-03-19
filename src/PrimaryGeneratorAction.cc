#include "PrimaryGeneratorAction.hh"
#include "Parameters.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "TFile.h"
#include "TH3F.h"

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),fParticleGun(nullptr)
{

  G4int nofParticles = 1;

  fParticleGun = new G4ParticleGun(nofParticles);

  TFile* rootfile = new TFile("maps/PET_1.root");

  if (!rootfile || rootfile->IsZombie()) {
        G4Exception("PrimaryGeneratorAction::LoadActivityMap",
        "FileNotFoundError",FatalException,"Error: activity map not found");
    }

  fPET = (TH3F*)rootfile->Get("histo");

}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

PrimaryGeneratorAction::~PrimaryGeneratorAction(){ delete fParticleGun; }

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{

  // Select radionuclide
  G4int Z = 9, A = 18; // F-18
  //G4int Z = 29, A = 64; // Cu-64
  //G4int Z = 31, A = 68; // Ga-68
  //G4int Z = 38, A = 89; // Sr-89
  //G4int Z = 39, A = 90; // Y-90
  //G4int Z = 43, A = 99; // Tc-99
  //G4int Z = 47, A = 111; // Ag-111
  //G4int Z = 49, A = 111; // In-111
  //G4int Z = 53, A = 131; // I-131
  //G4int Z = 62, A = 153; // Sm-153
  //G4int Z = 71, A = 177; // Lu-177
  G4double ionCharge   = 0.*eplus;
  G4double excitEnergy = 0.*keV;
  //G4double excitEnergy = 142.6836*keV; // Tc-99m

  ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
  fParticleGun->SetParticleDefinition(ion);
  fParticleGun->SetParticleCharge(ionCharge);
  fParticleGun->SetParticleEnergy(0*keV);

  nVoxelX = Parameters::cVoxelX;
  nVoxelY = Parameters::cVoxelY;
  nVoxelZ = Parameters::cVoxelZ;
  G4double v_size = Parameters::cVoxelSize; // Voxel size

  fPET->GetRandom3(x,y,z);
  x = (x-nVoxelX*0.5+G4UniformRand())*v_size;
  y = (y-nVoxelY*0.5+G4UniformRand())*v_size;
  z = (z-nVoxelZ*0.5+G4UniformRand())*v_size;

  // Set gun position
  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));

  fParticleGun->GeneratePrimaryVertex(event);

}
