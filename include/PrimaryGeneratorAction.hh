#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

// Uses G4GeneralParticleSource (GPS) to define radioactive ion sources.
// The actual source configuration (position, abundance weighting, nuclide)
// is done in the run.mac macro file — GPS is fully macro-controllable.
//
// The three sources (U238, Th232, K40) are defined as separate GPS sources
// and weighted by their relative abundances in crust and mantle.

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event*) override;

private:
    G4GeneralParticleSource* fParticleGun;
};

#endif
