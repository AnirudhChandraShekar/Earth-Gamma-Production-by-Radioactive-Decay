#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"

class G4Run;

// Manages output across the run.
// Accumulates total gamma counts and writes a summary at end of run.
// For full analysis, replace with ROOT TFile output.

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    // Called by EventAction to accumulate gamma counts
    void AddGammaCount(G4int n) { fGammaCount += n; }
    void AddGammaEnergy(G4double e) { fTotalGammaEnergy += e; }

private:
    G4Accumulable<G4int>    fGammaCount;
    G4Accumulable<G4double> fTotalGammaEnergy;
};

#endif
