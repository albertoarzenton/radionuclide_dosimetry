#include "VoxelSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

VoxelSD::VoxelSD(
  const G4String& name,
  const G4String& hitsCollectionName)
:
G4VSensitiveDetector(name),
fHitsCollection(nullptr)
{
  collectionName.insert(hitsCollectionName);
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

VoxelSD::~VoxelSD(){}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void VoxelSD::Initialize(G4HCofThisEvent* hce)
{

  // Create hits collection

  fHitsCollection =
    new VoxelHitsCollection(SensitiveDetectorName, collectionName[0]);

  auto hcID =
    G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

  hce->AddHitsCollection(hcID, fHitsCollection);

}

G4bool VoxelSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{

  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4TouchableHistory* touchable  = (G4TouchableHistory*)(preStepPoint->GetTouchable());

  // Custom code per hit processing
  // Get energy deposition in each detector
  auto edep = step->GetTotalEnergyDeposit(); //MeV
  auto mass = touchable->GetVolume()->GetLogicalVolume()->GetMass()/CLHEP::gram;
  auto dose = edep/mass;

  auto name = touchable->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
  if (name == "G4_AIR") dose = 0; // To neglect dose deposition in air

  if(edep == 0.) return true;

  // Get Current Voxel Copy number
  G4int copyNo = touchable->GetCopyNumber(0);

  VoxelHit* hit = new VoxelHit();

  // Setting up Hit
  hit->SetEdep(edep);
  hit->SetDose(dose);
  hit->SetCopyNumber(copyNo);

  fHitsCollection->insert(hit);

  return true;

}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void VoxelSD::EndOfEvent(G4HCofThisEvent*)
{ return ; }
