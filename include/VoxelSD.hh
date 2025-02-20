#ifndef VoxelSD_h
#define VoxelSD_h 1

#include <vector>
#include "G4VSensitiveDetector.hh"
#include "VoxelHit.hh"


class G4Step;
class G4HCofThisEvent;

class VoxelSD : public G4VSensitiveDetector
{
  public:

    VoxelSD(const G4String& name,
                   const G4String& hitsCollectionName);
    virtual ~VoxelSD();

    // Methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:

    VoxelHitsCollection* fHitsCollection;
};

#endif
