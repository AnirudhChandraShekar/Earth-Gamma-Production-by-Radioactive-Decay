// Main code 
// Simulates gamma production from radioactive decay of U238, Th232, and K40 in the Earth's crust and mantle, including full decay chains and secondary EM cascades (pair production, annihilation, Compton scattering).

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
// Declaration of the 3 main classes

#include "G4RunManagerFactory.hh" //Run manager
#include "G4UImanager.hh" //handles commands
#include "G4UIExecutive.hh" // opens interactive session
#include "G4VisExecutive.hh" //handles visualisation window

int main(int argc, char** argv) //argc- # of command line arguments, argv- list of them 
{
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    } //if no arguments(argc==1), creates interactive UI session; if marco file is passed, no session since batch mode does not need a UI window

    auto* runManager = G4RunManagerFactory::CreateRunManager(
        G4RunManagerType::Default);
    runManager->SetNumberOfThreads(1);

    // Creates the run manager (MT by default, falls back to sequential)
    
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();
    // Sets up visualisation 
        
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    //Pointer to the UI manager
        
    if (ui) {
        // Interactive: load visualisation and interactive settings
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    } //If interactive mode, execute init_vis.mac
    else {
        // Batch: execute the macro passed as argument
        G4String command  = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } //If batch mode, execute batch.mac, no UI window

    delete visManager;
    delete runManager;
    return 0;
    // Clean up memory and exit
}
