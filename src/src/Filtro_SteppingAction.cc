#include "Filtro_SteppingAction.hh"
#include "Filtro_Constantes.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

#include "G4SystemOfUnits.hh"

/* Alteracao 5 5 17 */
#include "G4ios.hh"
#include "G4GeometryTolerance.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4PhysicalConstants.hh"
// */

// Arquivo txt de out put 
#include <fstream>
using namespace std;
//ofstream OutPut_Step(TXT_FILE);

// Conversao de radianos para graus
#define RAD_TO_DEG 180/pi
//================================================================================

Filtro_SteppingAction::Filtro_SteppingAction(
                        Filtro_EventAction *eventAction, Filtro_RunAction* runAction, Filtro_PrimaryGeneratorAction* primaryAction)
    : G4UserSteppingAction (), fEventAction (eventAction), fRunAction (runAction), fPriAction (primaryAction) {
    
    fEventNumber = -1;

}

Filtro_SteppingAction::~Filtro_SteppingAction() { ; }

//================================================================================

void Filtro_SteppingAction::UserSteppingAction (const G4Step* aStep) {
        
    fEventNumber = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    //============================================================================
    G4double step_x = aStep->GetTrack()->GetPosition().x();
    G4double step_y = aStep->GetTrack()->GetPosition().y();
    G4double step_z = aStep->GetTrack()->GetPosition().z();

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // Fill ntuple
  

    //============================================================================

    // Step
    G4Track* track = aStep->GetTrack();
    G4String parti=track->GetDefinition()->GetParticleName();
    G4String material=track->GetMaterial()->GetName();
    
    //PreStep Info
    G4StepPoint * aPrePoint = aStep->GetPreStepPoint();
    G4VPhysicalVolume * aPrePV = aPrePoint->GetPhysicalVolume();
    G4String PreVolName = "";
    if (aPrePV) PreVolName = aPrePV->GetName();
    
    //PostStep Info
    G4StepPoint * aPostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume * aPostPV = aPostPoint->GetPhysicalVolume();
    G4String PostVolName = "";
    if (aPostPV) PostVolName= aPostPV->GetName();
    // energy
	// From the track you can obtain the pointer to the dynamic particle:
	const G4DynamicParticle* dynParticle = track -> GetDynamicParticle();
	// The dynamic particle class contains e.g. the kinetic energy after the step:
	G4double kinEnergy = dynParticle -> GetKineticEnergy();
    

    //============================================================================
	// here we start the tests.

	// if (PostVolName == "wls2_logico" && parti == "gamma")
	// {
	// 	// fRunAction->nOfDetections_Total ++;
	// 	// fRunAction->nOfDetections_ara ++;
	// 	// fRunAction->TOF_Detections_Total+=track->GetGlobalTime()*ns;
	// 	// analysisManager->FillH1(1, track->GetGlobalTime()*ns);
	// 	// cout << " nao foi preso " << endl;
 // 		analysisManager->FillNtupleIColumn(1,0,fEventNumber);
 //    	analysisManager->FillNtupleDColumn(1,1,step_x/cm);
 //    	analysisManager->FillNtupleDColumn(1,2,step_y/cm);
 //    	analysisManager->FillNtupleDColumn(1,3,step_z/cm);
 //    	analysisManager->FillNtupleDColumn(1,4,track->GetGlobalTime()/ns);
 //    	analysisManager->FillNtupleDColumn(1,5,kinEnergy/eV);
 //    	analysisManager->FillNtupleDColumn(1,6,1);
 //    	analysisManager->AddNtupleRow(1);
	// 	track->SetTrackStatus(fStopAndKill);
	// }

	
	if (PostVolName == "wls2_logico" && parti == "opticalphoton")
	{
		fRunAction->nOfDetections_Total ++;
		fRunAction->nOfDetections_ara ++;
		fRunAction->TOF_Detections_Total+=track->GetGlobalTime()*ns;
		// analysisManager->FillH1(1, track->GetGlobalTime()*ns);
		// cout << " nao foi preso " << endl;
 		analysisManager->FillNtupleIColumn(1,0,fEventNumber);
    	analysisManager->FillNtupleDColumn(1,1,step_x/cm);
    	analysisManager->FillNtupleDColumn(1,2,step_y/cm);
    	analysisManager->FillNtupleDColumn(1,3,step_z/cm);
    	analysisManager->FillNtupleDColumn(1,4,track->GetGlobalTime()/ns);
    	analysisManager->FillNtupleDColumn(1,5,kinEnergy/eV);
    	analysisManager->FillNtupleDColumn(1,6,2);
    	analysisManager->AddNtupleRow(1);
		track->SetTrackStatus(fStopAndKill);
	}

	// if (PostVolName == "wls2_logico" && parti == "e+")
	// {
	// 	fRunAction->nOfDetections_Total ++;
	// 	fRunAction->nOfDetections_ara ++;
	// 	fRunAction->TOF_Detections_Total+=track->GetGlobalTime()*ns;
	// 	// analysisManager->FillH1(1, track->GetGlobalTime()*ns);
	// 	// cout << " nao foi preso " << endl;
 // 		analysisManager->FillNtupleIColumn(1,0,fEventNumber);
 //    	analysisManager->FillNtupleDColumn(1,1,step_x/cm);
 //    	analysisManager->FillNtupleDColumn(1,2,step_y/cm);
 //    	analysisManager->FillNtupleDColumn(1,3,step_z/cm);
 //    	analysisManager->FillNtupleDColumn(1,4,track->GetGlobalTime()/ns);
 //    	analysisManager->FillNtupleDColumn(1,5,kinEnergy/eV);
 //    	analysisManager->FillNtupleDColumn(1,6,3);
 //    	analysisManager->AddNtupleRow(1);
	// 	track->SetTrackStatus(fStopAndKill);
	// }


	// if (PreVolName == "wls1_logico" && PostVolName == "dicroico_LV")
	// {
	// 	fRunAction -> 	nOfDetections_ara++;	
	// 	// track->SetTrackStatus(fStopAndKill);
	// }
	// if (PreVolName == BARRA_NOME && PostVolName != BARRA_NOME && kinEnergy < 3.2*eV)
	// {
	// 	fRunAction -> 	nOfDetections_dir++;	
	// 	track->SetTrackStatus(fStopAndKill);
	// }
	// // now we can check if the particle has touched the detectors. If it is not trapped, we count as an ARAPUCA event.

//     if (fPriAction -> traped == false && (PostVolName == SIPM_NOME)) 
// 	{
// 		fRunAction->nOfDetections_Total ++;
// 		fRunAction->nOfDetections_ara ++;
// 		fRunAction->TOF_Detections_Total+=track->GetGlobalTime()*ns;
// 		// analysisManager->FillH1(1, track->GetGlobalTime()*ns);
// 		// cout << " nao foi preso " << endl;
//  		analysisManager->FillNtupleIColumn(1,0,fEventNumber);
//     	analysisManager->FillNtupleDColumn(1,1,step_x/cm);
//     	analysisManager->FillNtupleDColumn(1,2,step_y/cm);
//     	analysisManager->FillNtupleDColumn(1,3,step_z/cm);
//     	analysisManager->FillNtupleDColumn(1,4,track->GetGlobalTime()/ns);
//     	analysisManager->AddNtupleRow(1);
// 		track->SetTrackStatus(fStopAndKill);
// 	}

// 	//  If it is trapped, we count as an BAR event. 

// 	if(fPriAction -> traped == true && kinEnergy < 3.*eV && (PostVolName == SIPM_NOME))
// 	{
// 		fRunAction->nOfDetections_Total ++;
// 		fRunAction->nOfDetections_bar ++;
// 		fRunAction->TOF_Detections_Total+=track->GetGlobalTime()*ns;
// 		analysisManager->FillH1(1, track->GetGlobalTime()*ns);
// 		// cout << " foi preso " << endl;
// 		analysisManager->FillNtupleIColumn(1,0,fEventNumber);
//     	analysisManager->FillNtupleDColumn(1,1,step_x/cm);
//     	analysisManager->FillNtupleDColumn(1,2,step_y/cm);
//     	analysisManager->FillNtupleDColumn(1,3,step_z/cm);
//     	analysisManager->FillNtupleDColumn(1,4,track->GetGlobalTime()/ns);
//     	analysisManager->AddNtupleRow(1);

// 		track->SetTrackStatus(fStopAndKill);

// 	}

// 	if (kinEnergy > 3.*eV && (PostVolName == SIPM_NOME))
// 	{
// 		fRunAction ->	nOfDetections_Total ++;
// 		fRunAction -> 	nOfDetections_dir	++;	
// 		fRunAction ->	TOF_Detections_Total+=track->GetGlobalTime()*ns;
// 		// analysisManager->FillH1(1, track->GetGlobalTime()*ns);

// 		// analysisManager->FillNtupleIColumn(1,0,fEventNumber);
//   //   	analysisManager->FillNtupleDColumn(1,1,step_x/cm);
//   //   	analysisManager->FillNtupleDColumn(1,2,step_y/cm);
//   //   	analysisManager->FillNtupleDColumn(1,3,step_z/cm);
//   //   	analysisManager->FillNtupleDColumn(1,4,track->GetGlobalTime()/ns);
//   //   	analysisManager->AddNtupleRow(1);

// 		track->SetTrackStatus(fStopAndKill);
// 	}
// cout << fPriAction -> traped << " " << material << " " << PreVolName << " "  << PostVolName << " " << kinEnergy << " " << fEventNumber << endl;

}

//================================================================================
