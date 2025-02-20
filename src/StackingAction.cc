#include "G4Track.hh"
#include "StackingAction.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"


////////////////////////////////////////////////////////////////////////////////

StackingAction::StackingAction(){

  a = 0;

}

////////////////////////////////////////////////////////////////////////////////

StackingAction::~StackingAction(){}

////////////////////////////////////////////////////////////////////////////////

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* track)
{
  //keep primary particle
  if (track->GetParentID() == 0) return fUrgent;

  if (track->GetDefinition() == G4AntiNeutrinoE::AntiNeutrinoE())
  {
    // do not track E-Neutrinos
    return fKill;
  }

 /* if (track->GetDefinition() == G4Gamma::Gamma())
  {
    // do not track Gammas
    return fKill;
  }
  */
/*
    if (track->GetDefinition() == G4Electron::Electron() and a==0)
  {
     a = 1;
    // do not track Electrons
    return fKill;
  }*/

  return fUrgent;

}

////////////////////////////////////////////////////////////////////////////////

void StackingAction::reset(){a=0;}
