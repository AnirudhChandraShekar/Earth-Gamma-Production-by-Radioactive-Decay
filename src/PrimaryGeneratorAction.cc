//   /gps/particle ion
//   /gps/ion Z A 0        (Z=atomic number, A=mass number)
// e.g. U238: /gps/ion 92 238 0
//      Th232: /gps/ion 90 232 0
//      K40:   /gps/ion 19 40 0
#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    fParticleGun = new G4GeneralParticleSource();
}//Creates a particle generator

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}// for memory purposes

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    fParticleGun->GeneratePrimaryVertex(event);
}//tells GeneralParticleSource to fire a particle
