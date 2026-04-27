#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Types.hh"

class RunAction;

// Per-event scoring.
// Counts gammas and accumulates energy reaching the detector surface.
// The sensitive detector (SensitiveDetector.hh) fills these values.

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction* runAction);
    ~EventAction() override;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    // Called by SensitiveDetector to record a gamma hit
    void AddGamma(G4double energy);

private:
    RunAction* fRunAction;
    G4int      fGammaCount;
    G4double   fGammaEnergy;
};

#endif
