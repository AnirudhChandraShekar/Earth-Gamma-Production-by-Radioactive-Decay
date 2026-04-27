// Records gamma hits at the detector surface (top of crust).
// Only gammas entering the detector volume are recorded.
// Their kinetic energy at the surface is logged for spectrum analysis.

#include "SensitiveDetector.hh"
#include "EventAction.hh"

#include "G4Step.hh" //small segment of a track 
#include "G4Track.hh" //full track 
#include "G4ParticleDefinition.hh"
#include "G4Gamma.hh" //for gamma
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

SensitiveDetector::SensitiveDetector(G4String name, EventAction* eventAction)
    : G4VSensitiveDetector(name),
      fEventAction(eventAction)
{}

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // Only record gammas
    G4Track* track = step->GetTrack();
    if (track->GetDefinition() != G4Gamma::Gamma()) return false; //filters to get gamma

    // Only record on first entry into detector volume (boundary crossing)
    if (step->GetPreStepPoint()->GetStepStatus() != fGeomBoundary) return false; //avoids counting repetition of gamma taken by many steps

    G4double energy = track->GetKineticEnergy(); //get K.E of gamma

    // Retrieve EventAction from the current event's user info
    // (EventAction pointer passed as nullptr in ConstructSDandField for now)
    // In a full implementation, retrieve via:
    // EventAction* ea = static_cast<EventAction*>(
    //     G4EventManager::GetEventManager()->GetUserEventAction());
    // ea->AddGamma(energy);

    // For now, print each gamma hit (disable for large runs)
    // G4cout << "Gamma at surface: " << energy/MeV << " MeV" << G4endl;

    // TODO: Fill a ROOT histogram here:
    // fGammaSpectrum->Fill(energy/MeV);

    return true;
}
