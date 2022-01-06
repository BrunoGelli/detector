#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"

#include "Filtro_DetectorConstruction.hh"
#include "Filtro_ActionInitialization.hh"
#include "Filtro_PhysicsList.hh"

#include "G4VisExecutive.hh"

#include "G4UIExecutive.hh"

// ============================================================================

int main(int argc,char** argv) {

// Seed the random number generator manually
//    G4long myseed = 345354;
    G4long myseed = 3453540;
    CLHEP::HepRandom::setTheSeed(myseed);

// ============================================================================
// Run manager

    G4double RIndex = 1;
    if (argc==2) {
        RIndex = atof(argv [1]);
    }
    if (argc==3) {
        RIndex = atof(argv [2]);
    }

    G4MTRunManager* runManager = new G4MTRunManager;
    runManager ->SetNumberOfThreads(12);
    runManager ->SetVerboseLevel(0);


    Filtro_DetectorConstruction* detConstruction = new Filtro_DetectorConstruction(RIndex);
    runManager->SetUserInitialization(detConstruction);

    runManager->SetUserInitialization(new Filtro_PhysicsList);

    Filtro_ActionInitialization* actionInitialization
    = new Filtro_ActionInitialization(detConstruction);

    runManager->SetUserInitialization(actionInitialization);



// visualization manager
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

// Initialize G4 kernel
    runManager->Initialize();

// ============================================================================
// User Interface

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

// Define UI session for interactive mode
    if (argc < 3) 
    {
        G4UIExecutive * ui = new G4UIExecutive(argc,argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    } 
    else if (argc == 3) 
    {

        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);

    }

// ============================================================================
// Delete
    delete visManager;
    delete runManager;

    return 0;
}

// ============================================================================
