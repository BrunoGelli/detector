#include "Filtro_PrimaryGeneratorAction.hh"
#include "Filtro_Constantes.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4SystemOfUnits.hh"

// ============================================================================

Filtro_PrimaryGeneratorAction::Filtro_PrimaryGeneratorAction() {
    
    // default
    G4int n_particle = 1;

    traped  =   true;

    particleGun = new G4ParticleGun(n_particle);
    particleGun->SetParticleEnergy (2500000*eV);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    particleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
   
}

// ============================================================================

Filtro_PrimaryGeneratorAction::~Filtro_PrimaryGeneratorAction() {
    
  	delete particleGun;
    
}

// ============================================================================

void Filtro_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) { 
  
    G4double yran = 0.99 * 10.0 * (G4UniformRand()-0.5)*cm;
    G4double zran = 0.99 * 10.0 * (G4UniformRand()-0.5)*cm;
    // G4double yran = 0.99 * 10.0 * (G4UniformRand()-0.5)*cm;
    // G4double zran = 0.99 * 08.0 * (G4UniformRand()-0.5)*cm;

    particleGun->SetParticleTime(0.*ns);
    particleGun->SetParticlePosition(G4ThreeVector(10*cm, yran, zran));
    particleGun->SetParticleMomentumDirection(G4ThreeVector(-1, 0, 0)); 
    particleGun->SetParticleEnergy (6500000*eV);
    // Gera evento
    particleGun->GeneratePrimaryVertex(anEvent);
    
}


// ============================================================================

void Filtro_PrimaryGeneratorAction::SetOptPhotonPolar() {
    
    G4double angle = G4UniformRand() * 360.0*deg;
    SetOptPhotonPolar(angle);
    
}

// ============================================================================

void Filtro_PrimaryGeneratorAction::SetOptPhotonPolar(G4double angle) {
    
    if (particleGun->GetParticleDefinition()->GetParticleName() != "opticalphoton") {
        G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
                "the particleGun is not an opticalphoton" << G4endl;
        return;
    }

    G4ThreeVector normal (1., 0., 0.);
    G4ThreeVector kphoton = particleGun->GetParticleMomentumDirection();
    G4ThreeVector product = normal.cross(kphoton);
    G4double modul2       = product*product;

    G4ThreeVector e_perpend (0., 0., 1.);
    if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
    G4ThreeVector e_paralle    = e_perpend.cross(kphoton);

    G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
    particleGun->SetParticlePolarization(polar);
    
}

// ============================================================================

