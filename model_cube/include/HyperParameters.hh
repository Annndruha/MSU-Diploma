#ifndef __HYPERPARAMETERS_HH__
#define __HYPERPARAMETERS_HH__

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

// == == == == == == == == == == == == == == == == == == == == == == ==
// Конфигурация мира.
//
constexpr G4double world_len = 20 * CLHEP::cm;
G4Material* world_material =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");


// == == == == == == == == == == == == == == == ==
// Параметры облучаемого объекта
//
constexpr G4double box_len = 10*CLHEP::cm;
G4Material* box_material =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");


#endif