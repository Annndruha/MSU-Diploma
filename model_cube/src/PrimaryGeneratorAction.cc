#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

// GPS gun ================================================

PrimaryGeneratorAction::PrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction(), fGPS(0)
{
  fGPS = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fGPS;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  fGPS->GeneratePrimaryVertex(anEvent);
}


// Custom gun =============================================

/* PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
  fParticleGun = new G4ParticleGun();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  static int i = 0;
  const int n = 6;

  G4double r = 20 * cm;
  double sini = std::sin(i/6.0 * 2 * M_PI);
  double cosi = std::cos(i/6.0 * 2 * M_PI);
  G4double dx = (G4UniformRand() - 0.5) * cm;
  G4double dy = (G4UniformRand() - 0.5) * cm;
  G4double dz = (G4UniformRand() - 0.5) * cm;
  G4double pos = 0 * CLHEP::cm;

  fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("neutron")); 
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sini, 0, cosi));
  fParticleGun->SetParticlePosition(G4ThreeVector(dx -r*sini, 0 + dy, pos + dz -r*cosi));


  fParticleGun->GeneratePrimaryVertex(anEvent);
  i = (i + 1) % n;
} */


// CSV gun =============================================

/* PrimaryGeneratorAction::PrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
  fParticleGun = new G4ParticleGun();
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
  fParticleGun->SetParticleEnergy(250 * MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  static int i = 0;
  const int n = 6;

  G4double r = 20 * cm;
  double sini = std::sin(i/6.0 * 2 * M_PI);
  double cosi = std::cos(i/6.0 * 2 * M_PI);
  G4double dx = (G4UniformRand() - 0.5) * cm;
  G4double dy = (G4UniformRand() - 0.5) * cm;
  G4double dz = (G4UniformRand() - 0.5) * cm;
  G4double pos = 0 * CLHEP::cm;

  fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("neutron")); 
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sini, 0, cosi));
  fParticleGun->SetParticlePosition(G4ThreeVector(dx -r*sini, 0 + dy, pos + dz -r*cosi));


  fParticleGun->GeneratePrimaryVertex(anEvent);
  i = (i + 1) % n;
}
reader("csv_particle_source/results_nt_All particles_t0.csv", 12); */