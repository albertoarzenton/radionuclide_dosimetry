#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "Analysis.hh"

class StackingAction : public G4UserStackingAction
{
    public:

        StackingAction();
        virtual ~StackingAction();

        void reset();

        virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);

    private:
        G4bool a;
};

#endif
