#ifndef Filtro_DetectorConstruction_h
#define Filtro_DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

class Filtro_DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    Filtro_DetectorConstruction(G4double RIndex);
    virtual ~Filtro_DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // World
    G4double world_x, world_y, world_z;
    
    // barra
    G4double altura, largura, espessura_cintilador, espessura_metal;

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

    G4double Refr_Index;

    G4double mgf2_esp;
};

#endif /*Filtro_DetectorConstruction_h*/
