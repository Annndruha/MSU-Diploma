
#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ThreeVector.hh"
#include "G4PhysicalConstants.hh"
#include "iostream"

#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4ProductionCuts.hh"
#include "G4Neutron.hh"
#include "G4Sphere.hh"


B1DetectorConstruction::B1DetectorConstruction(): G4VUserDetectorConstruction(),fScoringVolume(0)
{}

B1DetectorConstruction::~B1DetectorConstruction()
{}

void B1DetectorConstruction::SetJawAperture(G4int idJaw, G4ThreeVector &centre, G4ThreeVector halfSize, G4RotationMatrix *cRotation)
{
    G4double theta, x, y, z, dx, dy, dz;
    x=centre.getX();
    y=centre.getY();
    z=centre.getZ();

    dx=halfSize.getX();
    dy=halfSize.getY();
    dz=halfSize.getZ();

    switch (idJaw)
    {
    case 1: //idJaw1XV2100:
        theta = fabs(atan(10* cm/1000*mm));
        centre.set(z*sin(theta)+dx*cos(theta), y, z*cos(theta)-dx*sin(theta));
        cRotation->rotateY(-theta);
        halfSize.set(fabs(dx*cos(theta)+dz*sin(theta)), fabs(dy), fabs(dz*cos(theta)+dx*sin(theta)));
        break;

    case 2: //idJaw2XV2100:
        theta = fabs(atan(-10*cm/1000*mm));
        centre.set(-(z*sin(theta)+dx*cos(theta)), y, z*cos(theta)-dx*sin(theta));
        cRotation->rotateY(theta);
        halfSize.set(fabs(dx*cos(theta)+dz*sin(theta)), fabs(dy), fabs(dz*cos(theta)+dx*sin(theta)));
        break;

    case 3: //idJaw1YV2100:
        theta = fabs(atan(10*cm/1000*mm));
        centre.set(x, z*sin(theta)+dy*cos(theta), z*cos(theta)-dy*sin(theta));
        cRotation->rotateX(theta);
        halfSize.set(fabs(dx), fabs(dy*cos(theta)+dz*sin(theta)), fabs(dz*cos(theta)+dy*sin(theta)));
        break;

    case 4: //idJaw2YV2100:
        theta = fabs(atan(-10*cm/1000*mm));
        centre.set(x, -(z*sin(theta)+dy*cos(theta)), z*cos(theta)-dy*sin(theta));
        cRotation->rotateX(-theta);
        halfSize.set(fabs(dx), fabs(dy*cos(theta)+dz*sin(theta)), fabs(dz*cos(theta)+dy*sin(theta)));
        break;
    }
}

G4Material * B1DetectorConstruction::otherMaterials(const G4String materialName)
{
    G4Material * material=0;
    G4double A, Z, d;
    G4String name;

   // General elements

    A = 12.011*g/mole;
    G4Element* elC = new G4Element("Carbon","C",Z = 6.,A);

    A = 30.974*g/mole;
    G4Element* elP = new G4Element("Phosphorus","P",Z = 15.,A);

    A = 32.064*g/mole;
    G4Element* elS = new G4Element("Sulfur","S",Z = 16.,A);

    A = 55.85*g/mole;
    G4Element* elFe  = new G4Element("Iron","Fe",Z = 26.,A);

    A = 51.9961*g/mole;
    G4Element* elCr = new G4Element("Cromium","Cr", Z = 24.,A);

    A = 54.94*g/mole;
    G4Element* elMn = new G4Element("Manganese","Mn", Z = 25.,A);

    A =  58.69*g/mole;
    G4Element* elNi = new G4Element("Nickel","Ni", Z = 28.,A);

    A = 28.09*g/mole;
    G4Element* elSi = new G4Element("Silicon","Si", Z = 14.,A);

    A = 183.84*g/mole;
    G4Element* elW = new G4Element("Tungsten","W", Z = 74.,A);

    if (materialName=="steel1")
    {
        d = 7.76 *g/cm3;
        G4Material* steel1 = new G4Material("steel1", d,4);
        steel1 -> AddElement(elFe, 0.935);
        steel1 -> AddElement(elS,0.01);
        steel1 -> AddElement(elMn,0.05);
        steel1 -> AddElement(elC,0.005);
        material=steel1;
    }
    else if (materialName=="steel2")
    {
        d = 8.19*g/cm3;
        G4Material* steel2 = new G4Material("steel2", d,5);
        steel2 -> AddElement(elFe, 0.759);
        steel2 -> AddElement(elNi, 0.11);
        steel2 -> AddElement(elSi,0.01);
        steel2 -> AddElement(elCr,0.12);
        steel2 -> AddElement(elP,0.001);
        material=steel2;
    }
    else if (materialName=="steel3")
    {
        d = 8.19*g/cm3;
        G4Material* steel3 = new G4Material("steel3", d,5);
        steel3 -> AddElement(elFe, 0.69);
        steel3 -> AddElement(elNi, 0.1);
        steel3 -> AddElement(elSi,0.01);
        steel3 -> AddElement(elCr,0.18);
        steel3 -> AddElement(elMn,0.02);
        material=steel3;
    }
    else if (materialName=="EZcut")
    {
        d = 7.9*g/cm3;
        G4Material* EZcut20 = new G4Material("EZcut", d,2);
        EZcut20 -> AddElement(elFe, 0.98);
        EZcut20 -> AddElement(elMn,0.02);
        material=EZcut20;
    }
    else if (materialName=="W")
    {
        d = 15*g/cm3;
        G4Material* W = new G4Material("W", d,1);
        W -> AddElement(elW, 1.);
        material=W;
    }
    return material;
}

G4PVPlacement* ConstructXuy(
  char* name,
  G4ThreeVector sizes,
  G4ThreeVector position,
  G4Material* material,
  G4LogicalVolume* mother_volume)
{
  G4Box* solidXyu = new G4Box(name, sizes.getX(), sizes.getY(), sizes.getZ());
  G4LogicalVolume* logicXyu = new G4LogicalVolume(solidXyu, material, name);
  G4PVPlacement* physXyu = new G4PVPlacement(
    0,               // no rotation
    position,        // its position
    logicXyu,        // its logical volume
    name,            // its name
    mother_volume,   // its mother volume
    false,           // no boolean operation
    0);              // copyNo

  return physXyu;
}

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  G4double world_sizeX = 1.5 * m;   // полуразмеры мира
  G4double world_sizeY = 1.5 * m;   // полуразмеры мира
  G4double world_sizeZ = 2. * m;   // полуразмеры мира
  //
  G4double phantom_sizeX = 20 * cm; // полуразмеры фантома
  G4double phantom_sizeY = 20 * cm; // полуразмеры фантома
  G4double phantom_sizeZ = 20 * cm; // полуразмеры фантома

  G4NistManager* nist = G4NistManager::Instance();
  //
  G4Material* water = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* air   = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* Cu    = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* W     = nist->FindOrBuildMaterial("G4_W");
  G4Material* Ta     = nist->FindOrBuildMaterial("G4_Ta");
  G4Material* Fe     = nist->FindOrBuildMaterial("G4_Fe");
  G4Material* Pb     = nist->FindOrBuildMaterial("G4_Pb");
  G4Material* VC = nist->FindOrBuildMaterial("G4_Galactic");

  //
  G4Material* world_mat = VC;

  G4Box* solidWorld = new G4Box("World", world_sizeX, world_sizeY, world_sizeZ);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(
    0,               // no rotation
    G4ThreeVector(), // its position
    logicWorld,      // its logical volume
    "World",         // its name
    0,               // its mother volume
    false,           // no boolean operation
    0);              // copyNo


  /*

  auto plateCu = ConstructXuy(
    "Cu",
    G4ThreeVector(
      5*cm / 2,
      5*cm / 2,
      3 * mm / 2),
    G4ThreeVector(0,0,0*cm),
    Cu,
    logicWorld);

  auto plateW = ConstructXuy(
    "W",
    G4ThreeVector(
      5*cm / 2,
      5*cm / 2,
      1 * mm / 2),
    G4ThreeVector(0, 0, 0.*cm + 3 * mm / 2),
    W,
    logicWorld);

  auto shield = ConstructXuy(
              "shield",
              G4ThreeVector(50 *cm, 50 *cm, 0.5*mm),
              G4ThreeVector(0,0,43.1*cm),
              air,
              logicWorld);

  auto detector = ConstructXuy(
    "detector",
    G4ThreeVector(30. * cm, 30. * cm, 20. * cm),
    G4ThreeVector(0., 0., 65. * cm),
    water,
    logicWorld);

// =================== Accelerator construction =================== //


  G4RotationMatrix*  rotateMatrix=new G4RotationMatrix();
  rotateMatrix->rotateX(180.0*deg);

// --------- upper collimator ---------

  G4double innerRadiusOfTheTubeEx = 1.0*cm;
  G4double outerRadiusOfTheTubeEx = 8.*cm;
  G4double hightOfTheTubeEx = 8.0*cm;
  G4double startAngleOfTheTubeEx = 0.*deg;
  G4double spanningAngleOfTheTubeEx = 360.*deg;
  G4Tubs* UpperCollimator = new G4Tubs("UpperCollimator",
          innerRadiusOfTheTubeEx,
          outerRadiusOfTheTubeEx,
          hightOfTheTubeEx,
          startAngleOfTheTubeEx,
          spanningAngleOfTheTubeEx);
  G4LogicalVolume *UpperCollimator_log = new G4LogicalVolume(UpperCollimator, W, "UpperCollimator_log", 0, 0, 0);

  G4double UpperCollimatorPosX = 0.*cm;
  G4double UpperCollimatorPosY = 0.*cm;
  G4double UpperCollimatorPosZ = -2.*cm;
  new G4PVPlacement(0,
          G4ThreeVector(UpperCollimatorPosX, UpperCollimatorPosY, UpperCollimatorPosZ),
          "UpperCollimator",
          UpperCollimator_log,
          physWorld,
          false,
          0);


// --------- lower colimator ---------
  G4double  pRmin1 = 0.*cm;
  G4double  pRmax1 = 1*cm;
  G4double  pRmin2 = 0.*cm;
  G4double  pRmax2 = 2*cm;
  G4double  hightOfTheCone =3.2*cm;
  G4double  startAngleOfTheCone = 0.5*deg;
  G4double  spanningAngleOfTheCone = 360.*deg;

 // G4Material * Vac = G4NistManager::Instance()->FindOrBuildMaterial("G4Galactic");

  G4Cons* collim_cone = new G4Cons ("collim_cone",pRmin1,pRmax1,pRmin2,
            pRmax2,hightOfTheCone,startAngleOfTheCone,
            spanningAngleOfTheCone);
      G4LogicalVolume *collim_log = new G4LogicalVolume(collim_cone,world_mat,"collim_log",0,0,0);

 G4double innerRadiusOfTheTube = 0.*cm;
 G4double outerRadiusOfTheTube = 8.*cm;
 G4double hightOfTheTube = 3.1*cm;
 G4double startAngleOfTheTube = 0.*deg;
 G4double spanningAngleOfTheTube = 360.*deg;
 G4Tubs* tracker_tube = new G4Tubs("tracker_tube",innerRadiusOfTheTube,
         outerRadiusOfTheTube,hightOfTheTube,
         startAngleOfTheTube,spanningAngleOfTheTube);

 G4SubtractionSolid* CylMinusCone = new G4SubtractionSolid("Cyl-Cone",
              tracker_tube,collim_cone);
 G4LogicalVolume *CylMinusCone_log = new G4LogicalVolume(CylMinusCone,W,"CylminusCone_log",0,0,0);
 G4double CminusCPos_x = 0.*cm;
 G4double CminusCPos_y = 0.*cm;
 G4double CminusCPos_z = 8.3*cm;
      new G4PVPlacement(0,
              G4ThreeVector(CminusCPos_x, CminusCPos_y, CminusCPos_z),
              "CylMinusCone",
              CylMinusCone_log,
              physWorld,
              false,
              0);

 //--------- Visualization attributes -------------------------------
 G4VisAttributes* simpleTungstenWVisAtt= new G4VisAttributes(G4Colour::Magenta());
 simpleTungstenWVisAtt->SetVisibility(true);
 collim_log->SetVisAttributes(simpleTungstenWVisAtt);

 CylMinusCone_log->SetVisAttributes(simpleTungstenWVisAtt);
 UpperCollimator_log->SetVisAttributes(simpleTungstenWVisAtt);

 // ***********  REGIONS for CUTS
 G4Region *regVol;
 regVol= new G4Region("PrymCollR");
 G4ProductionCuts* cuts = new G4ProductionCuts;
 cuts->SetProductionCut(0.1*cm);
 regVol->SetProductionCuts(cuts);

 collim_log->SetRegion(regVol);
 regVol->AddRootLogicalVolume(collim_log);

 CylMinusCone_log->SetRegion(regVol);
 regVol->AddRootLogicalVolume(CylMinusCone_log);

 UpperCollimator_log->SetRegion(regVol);
 regVol->AddRootLogicalVolume(UpperCollimator_log);


 //============== vacuumWindow ==============

 G4Material *Be = G4NistManager::Instance()->FindOrBuildMaterial("G4_Be");
 G4Region *regVol1;
 G4VisAttributes* simpleAlSVisAtt;
 // Region for cuts
 regVol1= new G4Region("BeWindow");
 G4ProductionCuts* cuts1 = new G4ProductionCuts;
 cuts1->SetProductionCut(0.1*cm);
 regVol1->SetProductionCuts(cuts1);

 G4Tubs* BeWTube = new G4Tubs("BeWindowTube", 0., 36.*mm, 0.2*mm, 0.*deg, 360.*deg);
 G4LogicalVolume *BeWTubeLV = new G4LogicalVolume(BeWTube, Be, "BeWTubeLV", 0, 0, 0);
 new G4PVPlacement(0, G4ThreeVector(0.,0.,11.2*cm), "BeWTubePV", BeWTubeLV,
         physWorld, false, 0);

 simpleAlSVisAtt = new G4VisAttributes(G4Colour::Yellow());
 simpleAlSVisAtt -> SetVisibility(true);
 BeWTubeLV -> SetVisAttributes(simpleAlSVisAtt);
 BeWTubeLV -> SetRegion(regVol);
 regVol -> AddRootLogicalVolume(BeWTubeLV);

 // ============== FF ==============

 G4double z0, h0;
 G4ThreeVector centre, halSize;
 //G4Material *Cu=G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
 // Region for cuts
 G4Region *regVol2;
 regVol2= new G4Region("flatfilterR");
 G4ProductionCuts* cuts2 = new G4ProductionCuts;
 cuts2->SetProductionCut(0.5*cm);
 regVol2->SetProductionCuts(cuts2);

 G4VisAttributes* simpleAlSVisAttFF1, * simpleAlSVisAttFF2;

 // one
 z0=130.0*mm;
 h0=5.0/2.*cm;
 centre.set(0.,0.,z0);
 G4Cons *FFL1A_1Cone = new G4Cons("FFL1A_1", 0.*cm, 0.3*cm, 0.*cm, 5.*cm, h0, 0.*deg, 360.*deg);
 G4LogicalVolume *FFL1A_1LV = new G4LogicalVolume(FFL1A_1Cone, Fe, "FFL1A_1LV", 0, 0, 0);
 new G4PVPlacement(0, centre, "FFL1A_1PV", FFL1A_1LV, physWorld, false, 0);

 // two
 z0+=h0;
 h0=0.081/2.*cm;
 z0+=h0;
 centre.setZ(z0);
 z0+=h0;
 G4Tubs *FFL2_1Tube = new G4Tubs("FFL6_1", 0.*cm, 2.5*cm, h0, 0.*deg, 360.*deg);
 G4LogicalVolume *FFL2_1LV = new G4LogicalVolume(FFL2_1Tube, Ta, "FFL2_1LV", 0, 0, 0);
 new G4PVPlacement(0, centre, "FFL2_1PV", FFL2_1LV, physWorld, false, 0);

 simpleAlSVisAttFF1= new G4VisAttributes(G4Colour::Red());
 simpleAlSVisAttFF1->SetVisibility(true);
 simpleAlSVisAttFF2= new G4VisAttributes(G4Colour::Cyan());
 simpleAlSVisAttFF2->SetVisibility(true);
 FFL1A_1LV->SetVisAttributes(simpleAlSVisAttFF1);
 FFL2_1LV->SetVisAttributes(simpleAlSVisAttFF2);

 FFL1A_1LV->SetRegion(regVol2);
 FFL2_1LV->SetRegion(regVol2);

 regVol2->AddRootLogicalVolume(FFL1A_1LV);
 regVol2->AddRootLogicalVolume(FFL2_1LV);

 // ============== Ionization Chamber ===============

 G4Material *material=G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
 G4VisAttributes* simpleAlSVisAtt3;
 // Region for cuts
 G4Region *regVol3;
 regVol3= new G4Region("ionizationChamber");
 G4ProductionCuts* cuts3 = new G4ProductionCuts;
 cuts3->SetProductionCut(0.1*cm);
 regVol3->SetProductionCuts(cuts3);

 G4Tubs* ICTubeW = new G4Tubs("ionizationChamberTube", 0., 2.*2.54*10.*mm, 0.016*25.4*mm, 0.*deg, 360.*deg);
 G4Tubs* ICTubeP = new G4Tubs("ionizationChamberTube", 0., 2.*2.54*10.*mm, 0.010*25.4*mm, 0.*deg, 360.*deg);

 G4ThreeVector centre1;
 // W1
 centre1.set(0.,0.,157.*mm);
 G4LogicalVolume *PCUTubeW1LV = new G4LogicalVolume(ICTubeW, material, "ionizationChamberTubeW1LV", 0, 0, 0);
 new G4PVPlacement(0, centre1, "ionizationChamberTubeW1PV", PCUTubeW1LV, physWorld, false, 0);
 simpleAlSVisAtt3= new G4VisAttributes(G4Colour::Blue());
 simpleAlSVisAtt3->SetVisibility(true);
 PCUTubeW1LV->SetVisAttributes(simpleAlSVisAtt3);
 PCUTubeW1LV->SetRegion(regVol3);
 regVol3->AddRootLogicalVolume(PCUTubeW1LV);

 // P1
 centre1.set(0.,0.,158.*mm);
 G4LogicalVolume *PCUTubeP1LV = new G4LogicalVolume(ICTubeP, material, "ionizationChamberTubeP1LV", 0, 0, 0);
 new G4PVPlacement(0, centre1, "ionizationChamberTubeP1PV", PCUTubeP1LV, physWorld, false, 0);
 simpleAlSVisAtt3= new G4VisAttributes(G4Colour::Yellow());
 simpleAlSVisAtt3->SetVisibility(true);
 PCUTubeP1LV->SetVisAttributes(simpleAlSVisAtt3);
 PCUTubeP1LV->SetRegion(regVol3);
 regVol3->AddRootLogicalVolume(PCUTubeP1LV);

 // W2
 centre1.set(0.,0.,159.*mm);
 G4LogicalVolume *PCUTubeW2LV = new G4LogicalVolume(ICTubeW, material, "ionizationChamberTubeW2LV", 0, 0, 0);
 new G4PVPlacement(0, centre1, "ionizationChamberTubeW2PV", PCUTubeW2LV, physWorld, false, 0);
 simpleAlSVisAtt3= new G4VisAttributes(G4Colour::Blue());
 simpleAlSVisAtt3->SetVisibility(true);
 PCUTubeW2LV->SetVisAttributes(simpleAlSVisAtt3);
 PCUTubeW2LV->SetRegion(regVol3);
 regVol3->AddRootLogicalVolume(PCUTubeW2LV);

 // P2
 centre1.set(0.,0.,160.*mm);
 G4LogicalVolume *PCUTubeP2LV = new G4LogicalVolume(ICTubeP, material, "ionizationChamberTubeP2LV", 0, 0, 0);
 new G4PVPlacement(0, centre1, "ionizationChamberTubeP2PV", PCUTubeP2LV, physWorld, false, 0);
 simpleAlSVisAtt3= new G4VisAttributes(G4Colour::Yellow());
 simpleAlSVisAtt3->SetVisibility(true);
 PCUTubeP2LV->SetVisAttributes(simpleAlSVisAtt3);
 PCUTubeP2LV->SetRegion(regVol3);
 regVol3->AddRootLogicalVolume(PCUTubeP2LV);

 // W3
 centre1.set(0.,0.,161.*mm);
 G4LogicalVolume *PCUTubeW3LV = new G4LogicalVolume(ICTubeW, material, "ionizationChamberTubeW3LV", 0, 0, 0);
 new G4PVPlacement(0, centre1, "ionizationChamberTubeW3PV", PCUTubeW3LV, physWorld, false, 0);
 simpleAlSVisAtt3= new G4VisAttributes(G4Colour::Blue());
 simpleAlSVisAtt3->SetVisibility(true);
 PCUTubeW3LV->SetVisAttributes(simpleAlSVisAtt3);
 PCUTubeW3LV->SetRegion(regVol3);
 regVol3->AddRootLogicalVolume(PCUTubeW3LV);

 // P3
 centre1.set(0.,0.,162.*mm);
 G4LogicalVolume *PCUTubeP3LV = new G4LogicalVolume(ICTubeP, material, "ionizationChamberTubeP3LV", 0, 0, 0);
 new G4PVPlacement(0, centre1, "ionizationChamberTubeP3PV", PCUTubeP3LV, physWorld, false, 0);
 simpleAlSVisAtt3= new G4VisAttributes(G4Colour::Yellow());
 simpleAlSVisAtt3->SetVisibility(true);
 PCUTubeP3LV->SetVisAttributes(simpleAlSVisAtt3);
 PCUTubeP3LV->SetRegion(regVol3);
 regVol3->AddRootLogicalVolume(PCUTubeP3LV);

 // ============== miror ===============

 G4Material *MYLAR = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");
 G4VisAttributes* simpleAlSVisAtt4;
 // Region for cuts
 G4Region *regVol4;
 regVol4 = new G4Region("Mirror");
 G4ProductionCuts* cuts4 = new G4ProductionCuts;
 cuts4 -> SetProductionCut(0.1*cm);
 regVol4 -> SetProductionCuts(cuts4);

 G4Tubs* MirrorTube = new G4Tubs("MirrorTube", 0., 63.*mm, .5*mm, 0.*deg, 360.*deg);
 G4LogicalVolume *MirrorTubeLV = new G4LogicalVolume(MirrorTube, MYLAR, "MirrorTubeLV", 0, 0, 0);
 G4RotationMatrix *cRotation = new G4RotationMatrix();
 cRotation -> rotateY(12.0*deg);
 new G4PVPlacement(cRotation, G4ThreeVector(0., 0., 175.*mm), "MirrorTubePV", MirrorTubeLV,physWorld, false, 0);

 simpleAlSVisAtt4 = new G4VisAttributes(G4Colour::Green());
 simpleAlSVisAtt4 -> SetVisibility(true);
 MirrorTubeLV -> SetVisAttributes(simpleAlSVisAtt4);
 MirrorTubeLV -> SetRegion(regVol4);
 regVol4 -> AddRootLogicalVolume(MirrorTubeLV);





 //================ Jaws =================

 G4double jaw1XAperture, jaw2XAperture, jaw1YAperture, jaw2YAperture, isoCentre;

 jaw1XAperture = -100*mm;
 jaw1YAperture = 100*mm;
 jaw2XAperture = -100*mm;
 jaw2YAperture = 100*mm;
 isoCentre = 500*mm;

 G4Material *steel1=otherMaterials("steel1");
 G4Box *box;
 G4LogicalVolume *logVol;
 G4String name="Jaws1X";
 G4ThreeVector halfSize;
 G4RotationMatrix *cRotation1=new G4RotationMatrix();


 centre.set(0.,0.,(320.+80./2.)*mm);
 halfSize.set(45.*mm, 93.*mm, 78./2.*mm);
 box = new G4Box(name+"Box", 45.*mm, 93.*mm, 78./2.*mm);
 logVol = new G4LogicalVolume(box, steel1, name+"LV", 0, 0, 0);
 SetJawAperture(1, centre, halfSize, cRotation1);
   G4PVPlacement *  phVol1X= new G4PVPlacement(cRotation1, centre, logVol,name+"PV",  logicWorld, false, 0);

 // Region for cuts

 regVol= new G4Region(name+"R");

 cuts->SetProductionCut(2.*cm);
 regVol->SetProductionCuts(cuts);
 logVol->SetRegion(regVol);
 regVol->AddRootLogicalVolume(logVol);

 // Visibility
 simpleAlSVisAtt= new G4VisAttributes(G4Colour::Blue());
 simpleAlSVisAtt->SetVisibility(true);
// 	simpleAlSVisAtt->SetForceSolid(true);
 logVol->SetVisAttributes(simpleAlSVisAtt);


 name="Jaws2X";

 G4RotationMatrix *cRotation2=new G4RotationMatrix();



 centre.set(0.,0.,(320.+80./2.)*mm);
 halfSize.set(45.*mm, 93.*mm, 78./2.*mm);
 box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
 logVol = new G4LogicalVolume(box, steel1, name+"LV", 0, 0, 0);
 SetJawAperture(2, centre, halfSize, cRotation2);
   G4PVPlacement *  phVol2X= new G4PVPlacement(cRotation2, centre, logVol,name+"PV",  logicWorld, false, 0);

 // Region for cuts

 regVol= new G4Region(name+"R");

 cuts->SetProductionCut(2.*cm);
 regVol->SetProductionCuts(cuts);
 logVol->SetRegion(regVol);
 regVol->AddRootLogicalVolume(logVol);

 // Visibility
 simpleAlSVisAtt= new G4VisAttributes(G4Colour::Cyan());
 simpleAlSVisAtt->SetVisibility(true);
// 	simpleAlSVisAtt->SetForceSolid(true);
 logVol->SetVisAttributes(simpleAlSVisAtt);



 name="Jaws1Y";


 G4RotationMatrix *cRotation3=new G4RotationMatrix();




 centre.set(0.,0.,(230.+80./2.)*mm);
 halfSize.set(93.*mm, 35.*mm, 78./2.*mm);
 box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
 logVol = new G4LogicalVolume(box, steel1, name+"LV", 0, 0, 0);
 SetJawAperture(3, centre, halfSize, cRotation3);
 G4PVPlacement * phVol1Y= new G4PVPlacement(cRotation3, centre, logVol,name+"PV",  logicWorld, false, 0);

 // Region for cuts

 regVol= new G4Region(name+"R");

 cuts->SetProductionCut(2.*cm);
 regVol->SetProductionCuts(cuts);
 logVol->SetRegion(regVol);
 regVol->AddRootLogicalVolume(logVol);

 // Visibility
 simpleAlSVisAtt= new G4VisAttributes(G4Colour::Red());
 simpleAlSVisAtt->SetVisibility(true);
// 	simpleAlSVisAtt->SetForceSolid(true);
 logVol->SetVisAttributes(simpleAlSVisAtt);


 name="Jaws2Y";

 G4RotationMatrix *cRotation4=new G4RotationMatrix();


 centre.set(0.,0.,(230.+80./2.)*mm);
 halfSize.set(93.*mm, 35.*mm, 78./2.*mm);
 box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
 logVol = new G4LogicalVolume(box, steel1, name+"LV", 0, 0, 0);
 SetJawAperture(4, centre, halfSize, cRotation4);
  G4PVPlacement *   phVol2Y= new G4PVPlacement(cRotation4, centre, logVol,name+"PV",  logicWorld, false, 0);

 // Region for cuts


 regVol= new G4Region(name+"R");

 cuts->SetProductionCut(2.*cm);
 regVol->SetProductionCuts(cuts);
 logVol->SetRegion(regVol);
 regVol->AddRootLogicalVolume(logVol);

 // Visibility
 simpleAlSVisAtt= new G4VisAttributes(G4Colour::Magenta());
 simpleAlSVisAtt->SetVisibility(true);
// 	simpleAlSVisAtt->SetForceSolid(true);
 logVol->SetVisAttributes(simpleAlSVisAtt);

 */
 //===================================================================================

 //G4Material* W = nist->FindOrBuildMaterial("G4_W");

 G4bool checkOverlaps = true;

 G4ThreeVector pos1 = G4ThreeVector(0, 0, 2./2.*mm);

 G4Box* targetW =
   new G4Box("targetW",
   20./2*mm, 20./2*mm, 2./2.*mm);

 G4LogicalVolume* targetWL =
   new G4LogicalVolume(targetW,         //its solid
                       W,               //its material
                       "targetW");      //its name

 new G4PVPlacement(0,                       //no rotation
                   pos1,                    //at position
                   targetWL,             //its logical volume
                   "targetW",                //its name
                   logicWorld,                //its mother  volume
                   false,                   //no boolean operation
                   0,                       //copy number
                   checkOverlaps);          //overlaps checking

 //
 // targetCu
 //
 //G4Material* Cu = nist->FindOrBuildMaterial("G4_Cu");
 G4ThreeVector pos2 = G4ThreeVector(0, 0, 4.5*mm);

 G4Box* targetCu =
   new G4Box("targetCu",
   20./2*mm, 20./2*mm, 5./2*mm);

 G4LogicalVolume* targetCuL =
   new G4LogicalVolume(targetCu,         //its solid
                       Cu,          //its material
                       "targetCu");           //its name

 new G4PVPlacement(0,                       //no rotation
                   pos2,                    //at position
                   targetCuL,             //its logical volume
                   "targetCu",                //its name
                   logicWorld,                //its mother  volume
                   false,                   //no boolean operation
                   0,                       //copy number
                   checkOverlaps);          //overlaps checking


//vacuum window
 G4Material* Be = nist->FindOrBuildMaterial("G4_Be");
   G4Tubs* window = new G4Tubs("Window", 0., 50.*mm, 0.12*mm, 0.*deg, 360.*deg);
   G4LogicalVolume* windowl = new G4LogicalVolume (window, Be, "windowl");
   new G4PVPlacement(0, G4ThreeVector(0,0,80*mm), windowl, "windowp", logicWorld, false, 0, checkOverlaps);

//upper part primary collimator
G4ThreeVector posPC = G4ThreeVector(0, 0, 0);
G4Tubs* PCUW = new G4Tubs("PCUW", 10.*mm, 40.*mm, 20.*mm, 0.*deg, 360.*deg);
G4LogicalVolume* PCUWL =
 new G4LogicalVolume(PCUW,         //its solid
                     W,          //its material
                     "PCUW");  // Trapezoid shape


new G4PVPlacement(0,                       //no rotation
                 posPC,                    //at position
                 PCUWL,             //its logical volume
                 "PCUW",                //its name
                 logicWorld,                //its mother  volume
                 false,                   //no boolean operation
                 0,                       //copy number
                 checkOverlaps);


//lower part primary collimator
posPC = G4ThreeVector(0, 0, 50.*mm);
G4Tubs* PCLW = new G4Tubs("PCLW", 0, 40.*mm, 60./2*mm, 0.*deg, 360.*deg);
G4LogicalVolume* PCLWL = new G4LogicalVolume(PCLW,         //its solid
                                       W,          //its material
                                       "PCLW");  // Trapezoid shape


new G4PVPlacement(0,                       //no rotation
                 posPC,                    //at position
                 PCLWL,             //its logical volume
                 "PCLW",                //its name
                 logicWorld,                //its mother  volume
                 false,                   //no boolean operation
                 0,                       //copy number
                 checkOverlaps);

G4Material* Vacuum = nist->FindOrBuildMaterial("G4_Galactic");
G4Cons* InPCLW = new G4Cons ("InPCLW", 0., 10*mm, 0., (76.*std::tan(14.*deg))*mm, 30.*mm,  0.*deg, 360.*deg);
G4LogicalVolume* InPCLWL = new G4LogicalVolume(InPCLW,         //its solid
                                       Vacuum,          //its material
                                       "InPCLW");


 new G4PVPlacement(0,                       //no rotation
                   posPC,                    //at position
                   InPCLWL,             //its logical volume
                   "InPCLW",                //its name
                   logicWorld,                //its mother  volume
                   false,                   //no boolean operation
                   0,                       //copy number
                   checkOverlaps);



//flattering filter
G4RotationMatrix * nRot = new G4RotationMatrix;
nRot->rotateY(180*deg);

G4Cons * ff1 = new G4Cons ("ff1",0.,20.*mm,0.,50.*mm,2.01*mm,0.*deg,360.*deg);
G4LogicalVolume * ff1l = new G4LogicalVolume (ff1, Fe, "Conelog1");
new G4PVPlacement (nRot,
		   G4ThreeVector(0.,0.,101.005*mm),
		   ff1l,
		   "ff1p",
		   logicWorld,
		   false,
		   0,
		   checkOverlaps);

G4Cons * ff2 = new G4Cons ("ff2",0.,14.*mm,0.,20.*mm,5.21*mm,0.*deg,360.*deg);
G4LogicalVolume * ff2l = new G4LogicalVolume (ff2, Fe, "Conelog1");
new G4PVPlacement (nRot,
                   G4ThreeVector(0.,0.,104.615*mm),
                   ff2l,
                   "ff2p",
                   logicWorld,
                   false,
                   0,
                   checkOverlaps);

G4Cons * ff3 = new G4Cons ("ff3",0.,10.*mm,0.,14.*mm,2.48*mm,0.*deg,360.*deg);
G4LogicalVolume * ff3l = new G4LogicalVolume (ff3, Fe, "Conelog3");
new G4PVPlacement (nRot,
                   G4ThreeVector(0.,0.,108.46*mm),
                   ff3l,
                   "ff3p",
                   logicWorld,
                   false,
                   0,
                   checkOverlaps);

G4Cons * ff4 = new G4Cons ("ff4",0.,6.*mm,0.,10.*mm,1.7*mm,0.*deg,360.*deg);
G4LogicalVolume * ff4l = new G4LogicalVolume (ff4, Fe, "Conelog1");
new G4PVPlacement (nRot,
                   G4ThreeVector(0.,0.,110.55*mm),
                   ff4l,
                   "ff4p",
                   logicWorld,
                   false,
                   0,
                   checkOverlaps);

G4Cons * ff5 = new G4Cons ("ff5",0.,3.*mm,0.,6.*mm,0.7*mm,0.*deg,360.*deg);
G4LogicalVolume * ff5l = new G4LogicalVolume (ff5, Fe, "Conelog1");
new G4PVPlacement (nRot,
                   G4ThreeVector(0.,0.,111.75*mm),
                   ff5l,
                   "ff5p",
                   logicWorld,
                   false,
                   0,
                   checkOverlaps);

G4Cons * ff6 = new G4Cons ("ff6",0.,0.*mm,0.,3.*mm,0.2*mm,0.*deg,360.*deg);
G4LogicalVolume * ff6l = new G4LogicalVolume (ff6, Fe, "Conelog1");
new G4PVPlacement (nRot,
                   G4ThreeVector(0.,0.,112.2*mm),
                   ff6l,
                   "ff6p",
                   logicWorld,
                   false,
                   0,
                   checkOverlaps);
/*
G4double xRadius[21] = {0.0000000001*25.4*mm, 0.040*25.4*mm, 0.078*25.4*mm,
   0.105*25.4*mm, 0.131*25.4*mm, 0.160*25.4*mm, 0.205*25.4*mm, 0.248*25.4*mm, 0.343*25.4*mm, 0.436*25.4*mm, 0.531*25.4*mm,
   0.628*25.4*mm, 0.727*25.4*mm, 0.829*25.4*mm, 0.880*25.4*mm, 0.932*25.4*mm, 0.983*25.4*mm, 1.037*25.4*mm, 1.250*25.4*mm,
   1.350*25.4*mm, 1.5*25.4*mm};
G4double yHeight[21] = {0.744*25.4*mm, 0.718*25.4*mm, 0.682*25.4*mm, 0.653*25.4*mm,
   0.622*25.4*mm, 0.593*25.4*mm, 0.547*25.4*mm, 0.506*25.4*mm, 0.427*25.4*mm, 0.354*25.4*mm, 0.287*25.4*mm, 0.225*25.4*mm,
   0.168*25.4*mm, 0.119*25.4*mm, 0.090*25.4*mm, 0.067*25.4*mm, 0.047*25.4*mm, 0.030*25.4*mm, 0.030*25.4*mm, 0.125*25.4*mm,
   0.125*25.4*mm};

         G4double halfheigth;
         int i;
           G4double distanceLast = 105.2774*mm;
         G4String iName;
         char a[10];

for (i=0; i<18; i++)
{
 halfheigth = (yHeight[i]-yHeight[i+1])/2.;
               iName = (G4String)a;
 if (i==17)
 {halfheigth = (yHeight[i])/2;}
G4ThreeVector centre = G4ThreeVector (0,0, -25*mm + distanceLast+halfheigth);
distanceLast+=halfheigth*2;
G4Cons* cone = new G4Cons("Cone"+iName, 0., xRadius[i] , 0., xRadius[i+1] , halfheigth, 0.*deg, 360.*deg);
G4LogicalVolume* logvol = new G4LogicalVolume(cone, Cu, "ConeL"+iName);
new G4PVPlacement(0,
                 centre,
                 logvol,
                 "ConeP"+iName,
                 logicWorld,
                 false,
                 0,
                 checkOverlaps);

};
halfheigth = yHeight[18]/2.;
G4ThreeVector centre = G4ThreeVector (0,0, -25*mm + distanceLast+halfheigth);
G4Cons* cone1 = new G4Cons ("Cone18", 0., xRadius[20], 0., xRadius[20], halfheigth, 0.*deg, 360.0*deg);
G4LogicalVolume* logvol1 = new G4LogicalVolume(cone1, Cu, "ConeL18");
new G4PVPlacement(0,
                 centre,
                 logvol1,
                 "ConeP18",
                 logicWorld,
                 false,
                 0,
                 checkOverlaps);

 halfheigth = (yHeight[19]-yHeight[18])/2.;
 centre = G4ThreeVector (0,0,-25*mm + distanceLast-halfheigth);
 G4Cons* cone2 = new G4Cons ("Cone19", xRadius[19], xRadius[20], xRadius[18], xRadius[20], halfheigth, 0.*deg, 360.0*deg);
 G4LogicalVolume* logvol2 = new G4LogicalVolume(cone2, Cu, "ConeL19");
 new G4PVPlacement(0,
                  centre,
                  logvol2,
                  "ConeP19",
                  logicWorld,
                  false,
                  0,
                  checkOverlaps);


//сердечник
 //G4Material* Ta = nist->FindOrBuildMaterial("G4_Ta");
 G4Cons* coneIn = new G4Cons ("ConeIn", 0., 0., 0., (0.476*25.4)*mm, (0.258*25.4)*mm, 0.*deg, 360.0*deg);
 G4LogicalVolume* logcon = new G4LogicalVolume (coneIn, Ta, "ConeInL");
   centre = G4ThreeVector (0,0,90*mm);
 new G4PVPlacement(0,
                  centre,
                  logcon,
                  "ConePIn",
                  logicWorld,
                  false,
                  0,
                  checkOverlaps);
*/

G4ThreeVector centre;
//ionization chamber

 G4Material* KAPTON = nist->FindOrBuildMaterial("G4_KAPTON");
   G4Tubs* ICTubeW = new G4Tubs("ionizationChamberTube", 0., 3.75*2.54*10.*mm, 0.005*25.4*mm, 0.*deg, 360.*deg);
   G4Tubs* ICTubeP = new G4Tubs("ionizationChamberTube", 0., 3.75*2.54*10.*mm, 0.002*25.4*mm, 0.*deg, 360.*deg);


   // W1
   centre= G4ThreeVector(0.,0.,103*mm);
   G4LogicalVolume *PCUTubeW1LV = new G4LogicalVolume(ICTubeW, KAPTON, "ionizationChamberTubeW1LV");
   new G4PVPlacement(0, centre, PCUTubeW1LV,"ionizationChamberTubeW1PV", logicWorld, false, 0, checkOverlaps);
   // P1
   centre= G4ThreeVector(0.,0.,105.38*mm);
   G4LogicalVolume *PCUTubeP1LV = new G4LogicalVolume(ICTubeP, KAPTON, "ionizationChamberTubeP1LV");
   new G4PVPlacement(0, centre, PCUTubeP1LV, "ionizationChamberTubeP1PV", logicWorld, false, 0, checkOverlaps);
   // W2
   centre= G4ThreeVector(0.,0.,110.15*mm);
   G4LogicalVolume *PCUTubeW2LV = new G4LogicalVolume(ICTubeW, KAPTON, "ionizationChamberTubeW2LV");
   new G4PVPlacement(0, centre, PCUTubeW2LV, "ionizationChamberTubeW2PV", logicWorld, false, 0, checkOverlaps);

   // P2
   centre= G4ThreeVector(0.,0.,107.77*mm);
   G4LogicalVolume *PCUTubeP2LV = new G4LogicalVolume(ICTubeP, KAPTON, "ionizationChamberTubeP2LV");
   new G4PVPlacement(0, centre, PCUTubeP2LV, "ionizationChamberTubeP2PV", logicWorld, false, 0, checkOverlaps);

   // W3
   centre= G4ThreeVector(0.,0.,117.3*mm);
   G4LogicalVolume *PCUTubeW3LV = new G4LogicalVolume(ICTubeW, KAPTON, "ionizationChamberTubeW3LV");
   new G4PVPlacement(0, centre, PCUTubeW3LV, "ionizationChamberTubeW3PV", logicWorld, false, 0, checkOverlaps);

   // P3
   centre= G4ThreeVector(0.,0.,112.53*mm);
   G4LogicalVolume *PCUTubeP3LV = new G4LogicalVolume(ICTubeP, KAPTON, "ionizationChamberTubeP3LV");
   new G4PVPlacement(0, centre, PCUTubeP3LV, "ionizationChamberTubeP3PV", logicWorld, false, 0, checkOverlaps);

   // P4
   centre= G4ThreeVector(0.,0.,114.92*mm);
   G4LogicalVolume *PCUTubeP4LV = new G4LogicalVolume(ICTubeP, KAPTON, "ionizationChamberTubeP4LV");
   new G4PVPlacement(0, centre, PCUTubeP4LV, "ionizationChamberTubeP4PV", logicWorld, false, 0, checkOverlaps);


//mirror
 G4Material* MYLAR = nist->FindOrBuildMaterial("G4_MYLAR");
 G4Tubs* mirror = new G4Tubs("mirror", 0., 86.*mm, 1.0*mm, 0.*deg, 360.0*deg);
 G4LogicalVolume *mirrorl = new G4LogicalVolume(mirror, MYLAR, "mirrorl");
 G4RotationMatrix* pr = new 	G4RotationMatrix();
   pr -> rotateY(35.0*deg);
 new G4PVPlacement(pr,
                   G4ThreeVector(0., 0., (117.3+5+49.33)*mm),
                   mirrorl,
                   "mirrorp",
                   logicWorld,
                   false,
                   0,
                   checkOverlaps);


//jaws
//1x
   G4double theta;
 G4double jaw1XAperture = 5*cm;
   G4double jaw2XAperture = -5*cm;
     G4double jaw1YAperture = 5*cm;
       G4double jaw2YAperture = -5*cm;
G4RotationMatrix* cRotation1 = new G4RotationMatrix();
G4RotationMatrix* cRotation2 = new G4RotationMatrix();
G4RotationMatrix* cRotation3 = new G4RotationMatrix();
G4RotationMatrix* cRotation4 = new G4RotationMatrix();
G4double x, y, z, dx, dy; //, dz, top;
G4ThreeVector halfSize, centre1;
//	G4double beta, R;


centre= G4ThreeVector(50.*mm, 0,(117.3+5+2*49.33+25+78/2)*mm);
halfSize= G4ThreeVector(50.*mm, 90.*mm, 78./2.*mm);

x=centre.getX();
y=centre.getY();
z=centre.getZ();
//	top=z-78./2.;
dx=halfSize.getX();
dy=halfSize.getY();

G4Box* box1 = new G4Box("jaw1box", 50.*mm, 90.*mm, 78./2.*mm);
G4LogicalVolume *boxl1 = new G4LogicalVolume (box1, W, "jaw1boxl");



theta=fabs(atan(jaw1XAperture/1000*mm));
       centre1.set(z*sin(theta)+dx*cos(theta), y, z*cos(theta)-dx*sin(theta));
cRotation1->rotateY(-theta);
new G4PVPlacement (cRotation1, centre1, boxl1, "jaw1boxp", logicWorld, false,0, checkOverlaps);

//2x
centre= G4ThreeVector(-50.*mm,0,(117.3+5+2*49.33+25+78/2)*mm);
G4Box* box2 = new G4Box("jaw1box", 50.*mm, 90.*mm, 78./2.*mm);

   halfSize= G4ThreeVector(50.*mm, 90.*mm, 78./2.*mm);

 x=centre.getX();
 y=centre.getY();
 z=centre.getZ();
 //	top=z-78./2.;
 dx=halfSize.getX();
 dy=halfSize.getY();

G4LogicalVolume *boxl2 = new G4LogicalVolume (box2, W, "jaw2boxl");
theta=fabs(atan(jaw2XAperture/1000*mm));
       centre1.set(-(z*sin(theta)+dx*cos(theta)), y, z*cos(theta)-dx*sin(theta));
cRotation2->rotateY(theta);
new G4PVPlacement (cRotation2, centre1, boxl2, "jaw2boxp", logicWorld, false,0, checkOverlaps);

//3y
centre= G4ThreeVector(0, 50.*mm,(117.3+5+2*49.33+25+78/2+90)*mm);
G4Box* box3 = new G4Box("jaw3box",90.*mm, 50.*mm, 78./2.*mm);

   halfSize= G4ThreeVector(90.*mm, 50.*mm, 78./2.*mm);

 x=centre.getX();
 y=centre.getY();
 z=centre.getZ();
 //	top=z-78./2.;
 dx=halfSize.getX();
 dy=halfSize.getY();

G4LogicalVolume *boxl3 = new G4LogicalVolume (box3, W, "jaw3boxl");
theta=fabs(atan(jaw1YAperture/1000*mm));
       centre1.set(x, z*sin(theta)+dy*cos(theta), z*cos(theta)-dy*sin(theta));
cRotation3->rotateX(theta);
new G4PVPlacement (cRotation3, centre1, boxl3, "jaw3boxp", logicWorld, false,0, checkOverlaps);

//4y
centre= G4ThreeVector(0, -50.*mm,( 117.3+5+2*49.33+25+78/2+90)*mm);
G4Box* box4 = new G4Box("jaw4box",90.*mm, 50.*mm, 78./2.*mm);
   halfSize= G4ThreeVector(90.*mm, 50.*mm, 78./2.*mm);

 x=centre.getX();
 y=centre.getY();
 z=centre.getZ();
 //	top=z-78./2.;
 dx=halfSize.getX();
 dy=halfSize.getY();

G4LogicalVolume *boxl4 = new G4LogicalVolume (box4, W, "jaw4boxl");
theta=fabs(atan(jaw2YAperture/1000*mm));
       centre1.set(x, -(z*sin(theta)+dy*cos(theta)), z*cos(theta)-dy*sin(theta));
cRotation4->rotateX(-theta);
new G4PVPlacement (cRotation4, centre1, boxl4, "jaw3boxp", logicWorld, false,0, checkOverlaps);


G4RotationMatrix* cRot = new G4RotationMatrix();
cRot ->rotateX(180*deg);
 G4Sphere* hSphere = new G4Sphere("hSphere",
                                  10.*mm,
                                  40*mm,
                                  0*deg,
                                  360*deg,
                                  0*deg,
                                  90*deg);
 G4LogicalVolume* loghSphere = new G4LogicalVolume(hSphere,
                                                  W,
                                                  "loghSphere",
                                                  0,
                                                  0,
                                                  0,
                                                  true);
 new G4PVPlacement(cRot,                       //no rotation
                   G4ThreeVector(0, 0, -20.*mm),                    //at position
                   loghSphere,             //its logical volume
                   "hSphere",                //its name
                   logicWorld,                //its mother  volume
                   false,                   //no boolean operation
                   0,                       //copy number
                   checkOverlaps);


 G4Sphere* Sphere = new G4Sphere("Sphere", 40.*mm, 110.*mm, 0*deg, 360.*deg, 0*deg, (180-20)*deg);
 G4LogicalVolume* logSphere = new G4LogicalVolume(Sphere,
                                                  Fe,
                                                  "logSphere",
                                                  0,
                                                  0,
                                                  0,
                                                  true);



 new G4PVPlacement(cRot,                       //no rotation
                   G4ThreeVector(0, 0, -20.*mm),                    //at position
                   logSphere,             //its logical volume
                   "Sphere",                //its name
                   logicWorld,                //its mother  volume
                   false,                   //no boolean operation
                   0,                       //copy number
                   checkOverlaps);



 G4Sphere* nSphere = new G4Sphere("nSphere", 111.*mm, 170.*mm, 0*deg, 360.*deg, 0*deg, (180-40)*deg);
 G4LogicalVolume* lognSphere = new G4LogicalVolume(nSphere,
                                                  Pb,
                                                  "lognSphere",
                                                  0,
                                                  0,
                                                  0,
                                                  true);



 new G4PVPlacement(cRot,                       //no rotation
                   G4ThreeVector(0, 0, -20.*mm),                    //at position
                   lognSphere,             //its logical volume
                   "nSphere",                //its name
                   logicWorld,                //its mother  volume
                   false,                   //no boolean operation
                   0,                       //copy number
                   checkOverlaps);


/*G4Box* phantom = new G4Box ("phantom", 20.*cm, 20.*cm, 5*mm);
G4LogicalVolume *phantoml = new G4LogicalVolume (phantom, world_mat, "phantoml");
new G4PVPlacement (0, G4ThreeVector(0,0,(1000+5)*mm), phantoml, "phantomp", logicWorld, false, 0, checkOverlaps);
fScoringVolume = phantoml;*/

 G4Box* phantom = new G4Box ("Phantom",
                            20.*cm,
                            20.*cm,
                            15.*cm);

 G4LogicalVolume* phantoml = new G4LogicalVolume (phantom,
                                              water,
                                              "phantoml");
 new G4PVPlacement (0,
                    G4ThreeVector(0,0,105.*cm),
                    phantoml,
                    "phantomp",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);

 fScoringVolume = phantoml;







 //====================================================================================




 /*G4Sphere * sph = new G4Sphere("sphere",
                               100.*cm,
                               100.1 *cm,
                               0.*deg,
                               360.*deg,
                               0.*deg,
                               90.*deg);
 G4LogicalVolume *sph_log = new G4LogicalVolume(sph, air, "sph_log", 0, 0, 0);
 new G4PVPlacement(0,
         G4ThreeVector(0, 0, 0),
         "sph",
         sph_log,
         physWorld,
         false,
         0);
*/
 /*

 regVol = new G4Region("MLCR");
 std::vector <G4double> vec_leavesA, vec_leavesB;

 f



 G4ProductionCuts* cuts8 = new G4ProductionCuts;
 cuts -> SetProductionCut(1.0*cm);
 regVol -> SetProductionCuts(cuts);

 G4ThreeVector boxSize;
 G4ThreeVector centreStart;
 centreStart.set(0.,0.,(330.+600.)/2.*mm);
 boxSize.set(6./2.*mm, 180./2.*mm, 50./2.*mm);

 // single leaf
 G4Box* boxLeaf = new G4Box("LeafBox", boxSize.getX(), boxSize.getY(), boxSize.getZ());

 G4LogicalVolume *leafLVA = new G4LogicalVolume(boxLeaf, Fe, "leafSolidALV", 0, 0, 0);
 G4LogicalVolume *leafLVB = new G4LogicalVolume(boxLeaf, Fe, "leafSolidBLV", 0, 0, 0);

 simpleAlSVisAtt = new G4VisAttributes(G4Colour::Cyan());
 simpleAlSVisAtt -> SetVisibility(true);
 leafLVA -> SetVisAttributes(simpleAlSVisAtt);
 leafLVA -> SetRegion(regVol);
 regVol -> AddRootLogicalVolume(leafLVA);

 simpleAlSVisAtt = new G4VisAttributes(G4Colour::Green());
 simpleAlSVisAtt -> SetVisibility(true);
 leafLVB -> SetVisAttributes(simpleAlSVisAtt);
 leafLVB -> SetRegion(regVol);
 regVol -> AddRootLogicalVolume(leafLVB);

 int i;
 int j=0;
 G4String PVname;
 //G4ThreeVector centre;
 int nhalfLeaves = (int)(vec_leavesA.size()/2.);
 centre = centreStart + G4ThreeVector(-nhalfLeaves*boxSize.getX(), 0.,0.);
 for (i = 1; i < (int)vec_leavesA.size(); i++)
 {
     G4String str;
     char appo[12];
     sprintf(appo,"%d",i);
     str = appo;
     PVname = "leafA"+str;
     centre.setX(centre.getX()+boxSize.getX()*2.);
     centre.setY(-boxSize.getY()-vec_leavesA[i]);
     new G4PVPlacement(0, centre, PVname, leafLVA, physWorld, false, i);
     j++;
 }
 nhalfLeaves = (int)(vec_leavesB.size()/2.);
 centre = centreStart+G4ThreeVector(-nhalfLeaves*boxSize.getX(), 0.,0.);
 for (i = 1; i < (int)vec_leavesB.size(); i++)
 {
     G4String str;
     char appo[12];
     sprintf(appo,"%d",i);
     str = appo;
     PVname = "leafB"+str;
     centre.setX(centre.getX()+boxSize.getX()*2.);
     centre.setY(+boxSize.getY()+vec_leavesB[i]);
     new G4PVPlacement(0, centre, PVname, leafLVB, physWorld, false, i);
     j++;
}
*/
  return physWorld;
}