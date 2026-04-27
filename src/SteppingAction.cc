//=============================================================================
// SteppingAction.cc
//
// Records every gamma produced anywhere in the geometry at its point of
// creation. Writes to EarthGamma_output.csv in the working directory.
//
// CSV columns:
//   x(m), y(m), z(m), energy(MeV), nuclide, volume
//
// One row per gamma produced. Use Python/pandas to filter and plot.
//=============================================================================

#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TrackStatus.hh"
#include "G4VProcess.hh" //to check what physics process created gamma
#include "G4Ions.hh"
#include "G4IonTable.hh"
#include "G4RunManager.hh"

#include <sstream>

//For generating .csv file
std::mutex  SteppingAction::fFileMutex;
G4bool      SteppingAction::fHeaderWritten = false;

//For generating .csv file
SteppingAction::SteppingAction()
{
    // Each worker thread opens its own output file to avoid conflicts.
    // Files are named EarthGamma_output_t0.csv, _t1.csv etc.
    // Merge them in Python after the run.
    G4int threadID = G4Threading::G4GetThreadId();
    G4String filename = "EarthGamma_output_t" + std::to_string(threadID) + ".csv";

    fOutputFile.open(filename, std::ios::out | std::ios::app);

    // Write header only once per file
    if (fOutputFile.is_open()) {
        fOutputFile << "x_m,y_m,z_m,energy_MeV,nuclide,volume,process\n";
    }
}

SteppingAction::~SteppingAction()
{
    if (fOutputFile.is_open()) {
        fOutputFile.close();
    }
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    if (track->GetDefinition() != G4Gamma::Gamma()) return; //discard anything but gamma
    if (track->GetKineticEnergy() <= 0) return;
    G4cout << "Gamma seen at step " << track->GetCurrentStepNumber() << G4endl;

    if (track->GetCurrentStepNumber() != 1) return; //record gamma only once- at the place of creation

    // This filter gives gammas only from radioactive nuclides, no secondaries
    //const G4VProcess* creatorProcess = track->GetCreatorProcess();
    //if (!creatorProcess) return;
    //G4String processName = creatorProcess->GetProcessName();
    //if (processName != "RadioactiveDecay" && processName != "RadioactiveDecayBase") return;

    // Getp rocess name
    const G4VProcess* creatorProcess = track->GetCreatorProcess();
    G4String processName = creatorProcess ? creatorProcess->GetProcessName() : "Primary";

    G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
    G4double x = pos.x() / m;   // convert to metres
    G4double y = pos.y() / m;
    G4double z = pos.z() / m;
    //Get position

    G4double energy = track->GetKineticEnergy() / MeV;
    //get energy

    G4String volume = step->GetPreStepPoint()
                         ->GetTouchableHandle()
                         ->GetVolume()
                         ->GetName();
    //get volume name

    G4String nuclide = GetParentNuclide(track);
    //identify parent nuclide

    if (fOutputFile.is_open()) {
        fOutputFile << x           << ","
                    << y           << ","
                    << z           << ","
                    << energy      << ","
                    << nuclide     << ","
                    << volume      << ","
                    << processName << "\n";
    }//writing to file
}

G4String SteppingAction::GetParentNuclide(const G4Track* track)
{
    //get the primary particle that started this event
    const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
    if (!event) return "Unknown";
    G4PrimaryVertex* vertex = event->GetPrimaryVertex();
    if (!vertex) return "Unknown";
    G4PrimaryParticle* primary = vertex->GetPrimary();
    if (!primary) return "Unknown";

    // identify the nuclide
    const G4ParticleDefinition* primaryDef = primary->GetParticleDefinition();
    if (!primaryDef) return "Unknown";
    G4int Z = primaryDef->GetAtomicNumber();
    switch(Z) {
        case 92: return "U238";
        case 90: return "Th232";
        case 19: return "K40";
        default: return "Unknown";
    }
}
