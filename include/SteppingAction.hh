#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include <fstream>
#include <mutex>

// Records every gamma produced anywhere in the geometry at its point of creation.
// Writes one line per gamma to a CSV file:
//   x(m), y(m), z(m), energy(MeV), nuclide, volume
//
// Nuclide is traced back through the decay chain to identify whether the
// gamma ultimately originated from U238, Th232, or K40.

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction();
    ~SteppingAction() override;

    void UserSteppingAction(const G4Step* step) override;

private:
    // Traces a track's ancestry to find the original radionuclide (U238/Th232/K40)
    G4String GetParentNuclide(const G4Track* track);

    // Thread-safe file writing
    std::ofstream fOutputFile;
    static std::mutex fFileMutex;
    static G4bool fHeaderWritten;
};

#endif
