// Per-event scoring. Resets counters at start of each event and passes totals to RunAction at the end.

#include "EventAction.hh"
#include "RunAction.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction(RunAction* runAction)
    : fRunAction(runAction),
      fGammaCount(0),
      fGammaEnergy(0.0)
{} //pointer to run action

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fGammaCount  = 0;
    fGammaEnergy = 0.0;
}//resets counters at the start of every event

void EventAction::EndOfEventAction(const G4Event*)
{
    if (fGammaCount > 0) {
        fRunAction->AddGammaCount(fGammaCount);
        fRunAction->AddGammaEnergy(fGammaEnergy);
    }
}//passes counts to runaction if gammas are found

void EventAction::AddGamma(G4double energy)
{
    fGammaCount++;
    fGammaEnergy += energy;
}
