#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"

class EventAction;

// Sensitive detector placed at the top of the crust.
// Records every gamma that enters the detector volume,
// logging its energy for spectrum analysis.

class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector(G4String name, EventAction* eventAction);
    ~SensitiveDetector() override;

    G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;

private:
    EventAction* fEventAction;
};

#endif
