#include "RunAction.hh"
#include "Parameters.hh"

#include "TFile.h"
#include "TH3F.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <string>

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

RunAction::RunAction()
: G4UserRunAction()
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->CreateNtuple("tDose", "Voxel Absorbed Dose");
  man->CreateNtupleIColumn(0, "fEventID");
  man->CreateNtupleIColumn(0, "fVoxelX");
  man->CreateNtupleIColumn(0, "fVoxelY");
  man->CreateNtupleIColumn(0, "fVoxelZ");
  man->CreateNtupleDColumn(0, "fEdep");
  man->CreateNtupleDColumn(0, "fDose");
  man->SetActivation(true);
  //UImanager->ApplyCommand("/analysis/ntuple/setActivation " + std::to_string(0) + " False");
  man->FinishNtuple(0);

  nVoxelX = Parameters::cVoxelX;
  nVoxelY = Parameters::cVoxelY;
  nVoxelZ = Parameters::cVoxelZ;

  hEdep = new TH3F("hEdep","Deposited Energy",nVoxelX,0,nVoxelX,nVoxelY,0,nVoxelY,nVoxelZ,0,nVoxelZ);
  hDose = new TH3F("hDose","Absorbed Dose",nVoxelX,0,nVoxelX,nVoxelY,0,nVoxelY,nVoxelZ,0,nVoxelZ);
}


// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

RunAction::~RunAction()
{}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void RunAction::BeginOfRunAction(const G4Run* )
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->OpenFile("dose_tuple.root");
  rFile = new TFile("dose_map.root","RECREATE");

  return ;
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void RunAction::EndOfRunAction(const G4Run* )
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();

  hEdep->Write();
  hDose->Write();
  rFile->Close();

  return ;
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void RunAction::AddEdep(G4int x, G4int y, G4int z, G4double edep)
{

  auto current_value = hEdep->GetBinContent(x,y,z);

  hEdep->SetBinContent(x,y,z,current_value+edep);

  return ;

}

void RunAction::AddDose(G4int x, G4int y, G4int z, G4double dose)
{

  auto current_value = hDose->GetBinContent(x,y,z);

  hDose->SetBinContent(x,y,z,current_value+dose);

  return ;

}
