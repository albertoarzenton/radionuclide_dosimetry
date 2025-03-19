#include "EventAction.hh"
#include "VoxelSD.hh"
#include "VoxelHit.hh"
#include "RunAction.hh"
#include "Analysis.hh"
#include "Parameters.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include <fstream>

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

EventAction::EventAction()
: G4UserEventAction(), fScintHCID(-1)
{}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

EventAction::EventAction(RunAction* run)
: G4UserEventAction(), fScintHCID(-1)
{
  fRunAction = run;
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

EventAction::~EventAction()
{}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

VoxelHitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{

  auto hitsCollection = static_cast<VoxelHitsCollection*>
    ( event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! hitsCollection )
  {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("B4cEventAction::GetHitsCollection()",
    "MyCode0003", FatalException, msg);
  }

  return hitsCollection;

}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
  return ;
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void EventAction::EndOfEventAction(const G4Event* event)
{

  if(fScintHCID == -1)
  {
    fScintHCID = G4SDManager::GetSDMpointer()->GetCollectionID("VoxelHitsCollection");
  }

  // Get hits collections IDs (only once)
  auto voxelHC = GetHitsCollection(fScintHCID, event);

  G4double hit_energy;
  G4double hit_dose;
  G4int hit_copy_number;

  G4int ix,iy,iz;

  nVoxelX = Parameters::cVoxelX;
  nVoxelY = Parameters::cVoxelY;
  nVoxelZ = Parameters::cVoxelZ;

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  for(size_t i = 0; i < voxelHC->GetSize(); i++)
   {
     // Retrieve hit time and deposited energy
     hit_energy      = (*voxelHC)[i]->GetEdep();
     hit_dose        = (*voxelHC)[i]->GetDose();
     hit_copy_number = (*voxelHC)[i]->GetCopyNumber();

     // Convert to 3D indices (+1 for ROOT going 1,2,3...)
     ix = hit_copy_number%nVoxelX+1;
     iy = (hit_copy_number/nVoxelX)%nVoxelY+1;
     iz = (hit_copy_number/nVoxelX)/nVoxelY+1;

     fRunAction->AddEdep(ix,iy,iz,hit_energy);
     fRunAction->AddDose(ix,iy,iz,hit_dose);

     man->FillNtupleIColumn(0, 0, event->GetEventID());
     man->FillNtupleIColumn(0, 1, ix);
     man->FillNtupleIColumn(0, 2, iy);
     man->FillNtupleIColumn(0, 3, iz);
     man->FillNtupleDColumn(0, 4, hit_energy);
     man->FillNtupleDColumn(0, 5, hit_dose);
     man->AddNtupleRow(0);
   }

  return ;
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
