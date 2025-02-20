//
//  RADIONUCLIDE_DOSIMETRY
//  ----------------------------------------------------------------------------
//
//  Version 1.0
//
//  Geant4 application for internal dosimetry studies.
//  Developed within the ISOLPHARM project (https://isolpharm.pd.infn.it/web/).
//
//  Authors: A. Arzenton, D. Serafini, A. Leso, L. Morselli, M. Lunardon,
//  A. Andrighetto, G. Russo
//
//  Contact: alberto.arzenton@pd.infn.it, giorgio-russo@cnr.it
//
//
////////////////////////////////////////////////////////////////////////////////

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "StackingAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include <stdlib.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
    // Detect interactive mode and define UI session
    G4UIExecutive * ui = 0;
    if(argc == 1)
    {
      ui = new G4UIExecutive(argc, argv);
    }

    // Choose the random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    // Initialize SEED using current time since epoch
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // Set HepRandom with time dependent seed --> Needed for Cloud Parallelization
    CLHEP::HepRandom::setTheSeed(seed);
    G4Random::setTheSeed(seed);

    // Construct the run manager
    G4RunManager* runManager = new G4RunManager();

    // Set Mandatory initialization classes
  // ---------------------------------------------------------------------------
  DetectorConstruction * det   = new DetectorConstruction();

  // Pre-Defined Physics List
  PhysicsList * phys = new PhysicsList();


  runManager->SetUserInitialization(det);
  runManager->SetUserInitialization(phys);

  // Set Mandatory action classes
  // ---------------------------------------------------------------------------
  PrimaryGeneratorAction * kin   = new PrimaryGeneratorAction();
  RunAction              * run   = new RunAction();
  EventAction            * event = new EventAction(run);
  StackingAction         * stack = new StackingAction();

  runManager->SetUserAction(kin);
  runManager->SetUserAction(run);
  runManager->SetUserAction(event);
  runManager->SetUserAction(stack);

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // get the pointer to the User Interface Manager
  G4UImanager * uiManager = G4UImanager::GetUIpointer();

  if(!ui)
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    uiManager->ApplyCommand(command+fileName);
  }
  else
  {
    uiManager->ApplyCommand("/control/execute macro/vis.mac");
    ui->SessionStart(); // To open the graphical user interface
    delete ui;
  }

  // Terminate application
  // All user action and initialization classes are freed by the runManager
  // so they should not be deleted in the main() program
  delete visManager;
  delete runManager;

  return 0;
}
