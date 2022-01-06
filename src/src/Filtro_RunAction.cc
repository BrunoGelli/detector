#include "G4Timer.hh"

#include "Filtro_RunAction.hh"
#include "Filtro_Constantes.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "g4root.hh"

#include <fstream>
using namespace std;

//================================================================================

Filtro_RunAction::Filtro_RunAction() {
    
    timer = new G4Timer;
    EventNumber=-40;
    nOfReflections_Total = 0;
    nOfDetections_Total = 0;
    TOF_Detections_Total = 0;
    nOfDetections_bar = 0;
    nOfDetections_ara = 0;
    nOfDetections_dir = 0;
}

//================================================================================

Filtro_RunAction::~Filtro_RunAction() {
    
    delete timer;
  
}

//================================================================================

void Filtro_RunAction::BeginOfRunAction(const G4Run* aRun) {

    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    G4cout << "Using " << analysisManager->GetType() << " analysis manager." << G4endl;
    timer->Start();

    analysisManager->OpenFile("OutPut");
    analysisManager->SetVerboseLevel(0);


    // Indica o id do primeiro histograma criado -- default = 0
    analysisManager->SetFirstHistoId(1);
    // Cria histogramas    -- (nome, titulo, nbins, xmin, xmax)
    analysisManager->CreateH1 ("0","Time of arrival",5000,0.,10);


    // Indica o id da primeira ntuple criada -- default = 0
    analysisManager->SetFirstNtupleId(1);
    //Declara ntuples
    analysisManager->CreateNtuple("ntuple", "data");
    analysisManager->CreateNtupleIColumn("evt");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleDColumn("t");
    analysisManager->CreateNtupleDColumn("e");
    analysisManager->CreateNtupleDColumn("particle");
    analysisManager->FinishNtuple();

}

//================================================================================

void Filtro_RunAction::EndOfRunAction(const G4Run* aRun) {
    G4cout << "End of run." << G4endl;
    //G4cout << "Number of total reflections [Run] = " << nOfReflections_Total << G4endl;
    timer->Stop();
    G4cout << "Eficiency = " << (G4double)nOfDetections_Total / aRun->GetNumberOfEvent() << G4endl;
    G4cout << "Eficiency_DIR= " << (G4double)nOfDetections_dir / aRun->GetNumberOfEvent() << G4endl;
    G4cout << "Eficiency_BAR= " << (G4double)nOfDetections_bar / aRun->GetNumberOfEvent() << G4endl;
    G4cout << "Eficiency_ARAPUCA = " << (G4double)nOfDetections_ara / aRun->GetNumberOfEvent() << G4endl;
    G4cout << "Average Time of flight = " 
   	   << (G4double)TOF_Detections_Total / (G4double)nOfDetections_Total 
	   << " ns" << G4endl;
    G4cout << "Number of event = " << aRun->GetNumberOfEvent() << G4endl;


    if (PRINT_FILE == 1) {
	std::ofstream OutPut;

	OutPut.open(TXT_FILE, std::ios::app);
        //OutPut<<"Eficiency: " <<(G4double)nOfDetections_Total / aRun->GetNumberOfEvent()<<endl<<endl;
        OutPut<< setprecision(4) << fixed << (G4double)nOfDetections_Total / aRun->GetNumberOfEvent()<<endl;
	OutPut.close();
    }

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    delete G4AnalysisManager::Instance();
    
}

//================================================================================
