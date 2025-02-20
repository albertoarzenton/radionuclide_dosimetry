#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class TH3F;
class G4ParticleDefinition;


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* event);

    // Set methods
    void SetRandomFlag(G4bool value);

  private:

    G4ParticleGun* fParticleGun; // G4 particle gun
    TH3F* fPET;
    G4double x,y,z;
    G4ParticleDefinition* ion;

    G4int nVoxelX;
    G4int nVoxelY;
    G4int nVoxelZ;
};

#endif
