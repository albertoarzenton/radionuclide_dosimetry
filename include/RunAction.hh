#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <string>

#include "TFile.h"
#include "TH3F.h"

#include "G4AnalysisManager.hh"
#include "g4root_defs.hh"
#include "G4UImanager.hh"

class G4Run;

class RunAction : public G4UserRunAction
{

  public:

    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddEdep(G4int x, G4int y, G4int z, G4double edep);
    void AddDose(G4int x, G4int y, G4int z, G4double dose);

  private:

    TFile * rFile;
    TH3F * hEdep;
    TH3F * hDose;

    G4int nVoxelX;
    G4int nVoxelY;
    G4int nVoxelZ;

};

#endif
