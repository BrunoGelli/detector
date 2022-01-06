#include "Filtro_DetectorConstruction.hh"
#include "Filtro_Constantes.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VPVParameterisation.hh"
#include "G4PVReplica.hh"
#include "G4AutoDelete.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4SubtractionSolid.hh"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TransportationManager.hh"

#include <fstream>
using namespace std;


//================================================================================

Filtro_DetectorConstruction::Filtro_DetectorConstruction (G4double RIndex)
: G4VUserDetectorConstruction(), fCheckOverlaps(true), Refr_Index(RIndex) {
  
    // external WLS's thickness
    espessura_metal         = RIndex * 1E-6 * m * 0.5;
    // espessura_metal         = 0.2 * cm * 0.5;
    espessura_cintilador    = 1 * cm * 0.5;


    //barra
    largura  = 10.0 * cm * 0.5;
    altura   = 10.0 * cm * 0.5;


    // World
    world_x     = 100 *cm;
    world_y     = 100 *cm;
    world_z     = 100 *cm;

}

//================================================================================

Filtro_DetectorConstruction::~Filtro_DetectorConstruction (){;}

//================================================================================

G4VPhysicalVolume* Filtro_DetectorConstruction::Construct () {

    // Define materials 
    DefineMaterials();
    
    // Define volumes
    return DefineVolumes();
    
}

//================================================================================

void Filtro_DetectorConstruction::DefineMaterials() {
    
    // G4double a, z, density;
    // G4int nelements;
    G4NistManager* nistManager = G4NistManager::Instance();

    // G4Element* C  =  new G4Element ("Carbon"  , "C" , z = 6., a = 12.01  *g/mole);
    // G4Element* F  =  new G4Element ("Fluor"   , "F",  z = 9., a = 18.998 *g/mole);
    // G4Element* H  =  new G4Element ("Hydrogen"   , "H",  z = 1., a = 1 *g/mole);

    // G4Material* Teflon = new G4Material ("Teflon", density = 2.2 *g/cm3, nelements = 2);
    // Teflon->AddElement (C, 2);
    // Teflon->AddElement (F, 4);

    // G4Material* detec = new G4Material ("detec", density = 5 *g/cm3, nelements = 2);
    // detec->AddElement (C, 2);
    // detec->AddElement (F, 4);

    // G4Material* tpb = new G4Material ("tpb", density = 1.08 *g/cm3, nelements = 2);
    // tpb->AddElement (C, 28);
    // tpb->AddElement (H, 22);

    // G4Material* ptp = new G4Material ("ptp", density = 1.24 *g/cm3, nelements = 2);
    // ptp->AddElement (C, 18);
    // ptp->AddElement (H, 14);

    nistManager->FindOrBuildMaterial("G4_GLASS_PLATE");
    nistManager->FindOrBuildMaterial("G4_Galactic");
    // nistManager->FindOrBuildMaterial("G4_MAGNESIUM_FLUORIDE");

    nistManager->FindOrBuildMaterial("G4_W");


    // Print materials
    //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    
}

//================================================================================

G4VPhysicalVolume* Filtro_DetectorConstruction::DefineVolumes() {
    // Get materials
    G4Material* Vidro           = G4Material::GetMaterial("G4_GLASS_PLATE");
    // G4Material* WLS1_material   = G4Material::GetMaterial("ptp");
    // G4Material* WLS2_material   = G4Material::GetMaterial("tpb");    
    G4Material* Vacuo           = G4Material::GetMaterial("G4_Galactic");
    // G4Material* teflon          = G4Material::GetMaterial("Teflon");
    // G4Material* detec           = G4Material::GetMaterial("detec");
    // G4Material* MgF2_material   = G4Material::GetMaterial("G4_MAGNESIUM_FLUORIDE");
    G4Material* aco             = G4Material::GetMaterial("G4_W");

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//====================== World ===========-===========

    G4Box* world_S = new G4Box (MUNDO_NOME, world_x, world_y, world_z);
	
    G4LogicalVolume* world_LV =
    	new G4LogicalVolume (world_S, Vacuo, MUNDO_NOME);
	
    G4VPhysicalVolume* world_PV =
    	new G4PVPlacement (0, G4ThreeVector (), world_LV, MUNDO_NOME, 0, true, 0, fCheckOverlaps);

//==================== WLS1 - out ====================

    G4ThreeVector Pos_WLS1 = 
	G4ThreeVector (espessura_metal, 0*cm, 0*cm);

    G4Box* wls1_solid  =  new G4Box ("wls1", espessura_metal, largura, altura);

    G4LogicalVolume* wls1_logico = new G4LogicalVolume (wls1_solid, aco, "wls1_logico");

    G4VPhysicalVolume* WLS1PV = 
	new G4PVPlacement (0, Pos_WLS1, wls1_logico, "wls1_logico", world_LV, true, 0, fCheckOverlaps);

//==================== WLS2 - out ====================

    G4ThreeVector Pos_WLS2 = 
    G4ThreeVector (-espessura_cintilador, 0*cm, 0*cm);

    G4Box* wls2_solid  =  new G4Box ("wls2", espessura_cintilador, largura, altura);

    G4LogicalVolume* wls2_logico = new G4LogicalVolume (wls2_solid, Vidro, "wls2_logico");

    G4VPhysicalVolume* WLS2PV = 
    new G4PVPlacement (0, Pos_WLS2, wls2_logico, "wls2_logico", world_LV, true, 0, fCheckOverlaps);



 //    G4VPVParameterisation* chamberParam = new G4VPVParameterisation( number, -sipm_z/0.6*48*cm+0.5*sipm_z*cm,
 // 48/number*cm,0.6*cm,0.6*cm, 0.6*cm);
 //  G4VPhysicalVolume* physChamber =new G4PVParameterised( "Chamber", sipm_LV,
 // teste_LV, kZAxis, number, chamberParam);


        const G4int NUMENTRIES = 1;
        G4double Scint_PP[NUMENTRIES] = {3*eV};
        G4double Scint_FAST[NUMENTRIES] = {1};
        G4double Scint_SLOW[NUMENTRIES] = {1};

        //proprieta ottiche scint (da trovare)

        G4double rindex[NUMENTRIES] = {1.5};
        G4double rindex_V[NUMENTRIES] = {1};
        G4double absorption[NUMENTRIES] = {1*1E-6*m};

        G4MaterialPropertiesTable* Scint_MPT = new G4MaterialPropertiesTable();

        Scint_MPT->AddProperty("FASTCOMPONENT", Scint_PP, Scint_FAST, NUMENTRIES);
        Scint_MPT->AddProperty("SLOWCOMPONENT", Scint_PP, Scint_SLOW, NUMENTRIES);

        Scint_MPT->AddConstProperty("SCINTILLATIONYIELD", 100./MeV);
        Scint_MPT->AddConstProperty("RESOLUTIONSCALE", 1.);

        Scint_MPT->AddConstProperty("FASTTIMECONSTANT",    1.0*ns);
        Scint_MPT->AddConstProperty("SLOWTIMECONSTANT", 1000.0*ns);
        Scint_MPT->AddConstProperty("YIELDRATIO", 0.25);

        Scint_MPT -> AddProperty("RINDEX",Scint_PP,rindex,NUMENTRIES);
        Scint_MPT -> AddProperty("ABSLENGTH",Scint_PP,absorption,NUMENTRIES);

        Vidro->SetMaterialPropertiesTable(Scint_MPT);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Properties %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// //==================== WLS2 - in ====================
// //tpb
    
//     G4MaterialPropertiesTable* wls2_PT = new G4MaterialPropertiesTable();

//     wls2_PT->AddProperty ("WLSABSLENGTH", energy,  EJ_WLS_ABS, 101); 
//     wls2_PT->AddProperty ("WLSCOMPONENT", energy,  EJ_WLS_EM,  101); 
//     wls2_PT->AddConstProperty("WLSTIMECONSTANT", 0*ns);

//     wls2_PT->AddProperty ("RINDEX", energy, R_Index_mgf2, 101);	// as Xarapuca
    
// 	wls2_PT->AddProperty ("ABSLENGTH", 	energy, abs, 101);		


//     WLS2_material->SetMaterialPropertiesTable(wls2_PT);

// //====================== Vacuo ======================
    
    G4MaterialPropertiesTable* Vacuo_PT = new G4MaterialPropertiesTable();

    Vacuo_PT->AddProperty ("RINDEX",	Scint_PP, rindex_V,   1);
    Vacuo_PT->AddProperty ("ABSLENGTH", Scint_PP, absorption, 1);

    Vacuo->SetMaterialPropertiesTable(Vacuo_PT);


// //====================== Detector ======================

//     G4MaterialPropertiesTable* detec_PT = new G4MaterialPropertiesTable();

//     detec_PT->AddProperty ("RINDEX",    energy, R_Index_mgf2, 101);
//     detec_PT->AddProperty ("ABSLENGTH", energy, detec_abs,   101);

//     detec->SetMaterialPropertiesTable(detec_PT);

// //====================== Teflon ======================
  
//     G4MaterialPropertiesTable* teflon_PT = new G4MaterialPropertiesTable();

//     teflon_PT->AddProperty ("RINDEX",    energy, R_Index,   101);
//     teflon_PT->AddProperty ("ABSLENGTH", energy, detec_abs, 101);

//     teflon->SetMaterialPropertiesTable(teflon_PT);
   
// // sup opt entre arapuca e mundo
// //

// //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% OpticalSurface %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



// ================ Mundo -> Arapuca =================

G4MaterialPropertiesTable *Tef_prop = new G4MaterialPropertiesTable();

Tef_prop->AddProperty("REFLECTIVITY",Scint_PP,rindex_V,1);

G4OpticalSurface *Tef_OptSurf 
= new G4OpticalSurface("Tef_OptSurf");

Tef_OptSurf->SetModel(glisur);
/* dielectric_dielectric or dielectric_metal ? */
Tef_OptSurf->SetType(dielectric_metal);
Tef_OptSurf->SetFinish(ground);
Tef_OptSurf->SetPolish(0.9);

Tef_OptSurf->SetMaterialPropertiesTable(Tef_prop);


new G4LogicalBorderSurface("Arapuca_BorderSurface", WLS2PV,   WLS1PV, Tef_OptSurf);



// // ================ Dicroico -> wls1 =================
// /* */
// G4OpticalSurface *Dic_OptSur = new G4OpticalSurface("Dic_OptSur");

// //Dic_OptSur->SetModel(glisur);
// Dic_OptSur->SetType(dielectric_metal);
// //Dic_OptSur->SetFinish(polished);
// //Dic_OptSur->SetPolish(0.9);

// G4MaterialPropertiesTable *Dic_prop = new G4MaterialPropertiesTable();

// Dic_prop->AddProperty("REFLECTIVITY",energy,Dicroic_Reflectivity,101);

// Dic_OptSur->SetMaterialPropertiesTable(Dic_prop);

// new G4LogicalBorderSurface("Dicroic_to_WLS1_BorderSurface", dicroiuco_PV, WLS1PV, Dic_OptSur);

// // ================ Dicroico -> arapuca =================

// G4OpticalSurface *Dic_OptSur_new = new G4OpticalSurface("Dic_OptSur_new");

// //Dic_OptSur->SetModel(glisur);
// Dic_OptSur->SetType(dielectric_metal);
// //Dic_OptSur->SetFinish(polished);
// //Dic_OptSur->SetPolish(0.9);

// G4MaterialPropertiesTable *Dic_prop_new = new G4MaterialPropertiesTable();

// Dic_prop_new->AddProperty("REFLECTIVITY",energy,Arapuca_Reflectivity,101);

// Dic_OptSur_new->SetMaterialPropertiesTable(Dic_prop_new);

// new G4LogicalBorderSurface("Dicroic_to_ARAPUCA_BorderSurface", arapuca_PV, dicroiuco_PV, Dic_OptSur_new);

// // ================ world -> Dicroico =================
// /* */
// G4OpticalSurface *Dic_OptSur2 = new G4OpticalSurface("Dic_OptSur2");

// //Dic_OptSur->SetModel(glisur);
// Dic_OptSur2->SetType(dielectric_metal);
// //Dic_OptSur->SetFinish(polished);
// //Dic_OptSur->SetPolish(0.9);

// G4MaterialPropertiesTable *Dic_prop2 = new G4MaterialPropertiesTable();

// Dic_prop2->AddProperty("REFLECTIVITY",energy,Dicroic_Reflectivity,101);

// Dic_OptSur2->SetMaterialPropertiesTable(Dic_prop2);

// new G4LogicalBorderSurface("World_to_Dicroic_BorderSurface", world_PV,dicroiuco_PV, Dic_OptSur2);
// new G4LogicalBorderSurface("Bar_to_Dicroic_BorderSurface",   wls2_PV,dicroiuco_PV,  Dic_OptSur2);

// // ================ Barra -> Arapuca =================
// /* */
// G4OpticalSurface *Dic_OptSur3 = new G4OpticalSurface("Dic_OptSur3");

// //Dic_OptSur3->SetModel(glisur);
// Dic_OptSur3->SetType(dielectric_metal);
// //Dic_OptSur3->SetFinish(polished);
// //Dic_OptSur3->SetPolish(0.9);

// G4MaterialPropertiesTable *Dic_prop3 = new G4MaterialPropertiesTable();

// Dic_prop3->AddProperty("REFLECTIVITY", energy, Arapuca_Reflectivity, 101);

// Dic_OptSur3->SetMaterialPropertiesTable(Dic_prop3);

// new G4LogicalBorderSurface("WLS2_to_Arapuca_BorderSurface", wls2_PV, arapuca_PV, Dic_OptSur3);

// //  ================ Barra -> gaps =================
// G4OpticalSurface *Gaps_OptSurf = new G4OpticalSurface("Gaps_OptSurf");

// //Dic_OptSur3->SetModel(glisur);
// Gaps_OptSurf->SetType(dielectric_metal);
// //Dic_OptSur3->SetFinish(polished);
// //Dic_OptSur3->SetPolish(0.9);

// G4MaterialPropertiesTable *Gaps_prop = new G4MaterialPropertiesTable();

// Gaps_prop->AddProperty("REFLECTIVITY", energy, Arapuca_Reflectivity, 101);

// Gaps_OptSurf->SetMaterialPropertiesTable(Gaps_prop);

// new G4LogicalBorderSurface("WLS2_to_Arapuca_BorderSurface", wls2_PV, gaps_ph,   Gaps_OptSurf);
// new G4LogicalBorderSurface("WLS2_to_Arapuca_BorderSurface", wls2_PV, gaps_2_ph, Gaps_OptSurf);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Visualization Attributes %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // G4VisAttributes* branco = new G4VisAttributes (G4Colour (1.0, 1.0, 1.0, 0.8));
    // G4VisAttributes* cinza = new G4VisAttributes (G4Colour (1.0, 1.0, 1.0, 0.5));
    // G4VisAttributes* branco_vazado = new G4VisAttributes (G4Colour (1.0, 1.0, 1.0, 0.1));
    // G4VisAttributes* azul = new G4VisAttributes (G4Colour (0.5, 0.5, 1.0, 0.8));
    // G4VisAttributes* azul_vazado = new G4VisAttributes (G4Colour (0.5, 0.5, 1.0, 1));
    // G4VisAttributes* verde = new G4VisAttributes (G4Colour (0.5, 1, 0.5, 0.7));
    // G4VisAttributes* vermelho = new G4VisAttributes (G4Colour (1, 0, 0, 0.8));
    // G4VisAttributes* vermelho_forte = new G4VisAttributes (G4Colour (1, 0, 0, 1));
    // G4VisAttributes* transparente = new G4VisAttributes (G4Colour (1, 1, 1, 0));
    // G4VisAttributes* preto = new G4VisAttributes (G4Colour (0, 0, 0, 0.2));
    // G4VisAttributes* preto_vazado = new G4VisAttributes (G4Colour (0, 0, 0, 1));
    

    // branco->SetForceSolid (true);
    // branco_vazado->SetForceSolid (false);
    // cinza->SetForceSolid (true);
    // azul->SetForceSolid (true);
    // vermelho->SetForceSolid (true);
    // vermelho_forte->SetForceSolid (true);
    // azul_vazado->SetForceSolid (false);
    // preto->SetForceSolid(true);
    // preto_vazado->SetForceSolid(false);
    // verde->SetForceSolid(true);

    // Gaps_LV->SetVisAttributes(transparente);
    // arapuca_LV->SetVisAttributes(branco);
    // Sipm_LV->SetVisAttributes(vermelho_forte);
    // teste_LV->SetVisAttributes(transparente);
    // replicas_LV->SetVisAttributes(transparente);

    // dicroico_LV->SetVisAttributes(vermelho);
    // wls1_logico->SetVisAttributes(verde);
    // wls2_LV->SetVisAttributes(azul);

    // mgf2_logico->SetVisAttributes(transparente);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% End %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    return world_PV;
    
}

//
//================================================================================

