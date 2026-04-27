// TODO: Replace with ROOT TFile/TH1D for gamma energy spectrum histogramming.
// 

#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AccumulableManager.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

RunAction::RunAction(): fGammaCount(0),fTotalGammaEnergy(0.0)
{
    G4AccumulableManager* accMgr = G4AccumulableManager::Instance();
    accMgr->RegisterAccumulable(fGammaCount);
    accMgr->RegisterAccumulable(fTotalGammaEnergy);
}

RunAction::~RunAction() {} //empty destructor

void RunAction::BeginOfRunAction(const G4Run* run)
{
    G4AccumulableManager::Instance()->Reset();
    G4cout << "\n=== EarthGamma Run " << run->GetRunID()
           << " starting ===" << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* run)
{
    G4AccumulableManager::Instance()->Merge();

    G4int    nEvents = run->GetNumberOfEvent();
    G4int    nGamma  = fGammaCount.GetValue();
    G4double eTotal  = fTotalGammaEnergy.GetValue();

    if (IsMaster()) {
        G4cout << "\n=== EarthGamma Run Summary ===" << G4endl;
        G4cout << "  Events simulated       : " << nEvents << G4endl;
        G4cout << "  Gammas reaching surface: " << nGamma  << G4endl;
        if (nGamma > 0) {
            G4cout << "  Mean gamma energy      : "
                   << (eTotal/nGamma)/MeV << " MeV" << G4endl;
        }
        G4cout << "==============================\n" << G4endl;

        // TODO: Write ROOT histogram here
        // Example (requires ROOT linkage in CMakeLists.txt):
        // TFile* f = new TFile("EarthGamma_output.root", "RECREATE");
        // fGammaSpectrum->Write();
        // f->Close();
    }
}
