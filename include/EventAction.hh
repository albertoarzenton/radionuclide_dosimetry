#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "VoxelHit.hh"
#include "RunAction.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"

#include <fstream>


class EventAction : public G4UserEventAction
{

  public:

    EventAction();
    EventAction(RunAction*);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

  private:

    VoxelHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;

    // Data Member
    G4int fScintHCID;

    RunAction* fRunAction;

    G4int nVoxelX;
    G4int nVoxelY;
    G4int nVoxelZ;

};

#endif
