#include "PhysicsList.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    SetVerboseLevel(1);
    RegisterPhysics(new G4EmLivermorePhysics());
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList() {}

void PhysicsList::SetCuts()
{
    SetCutValue(0.1*mm, "gamma");
    SetCutValue(0.1*mm, "e-");
    SetCutValue(0.1*mm, "e+");
    SetCutValue(0.1*mm, "proton");

    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(
        10.0*keV, 10.0*GeV);
}