#include "DetectorConstruction.hh"
#include "VoxelSD.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVParameterised.hh"
#include "G4PhantomParameterisation.hh"
#include "G4VisAttributes.hh"

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

DetectorConstruction::~DetectorConstruction()
{ }

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void DetectorConstruction::DefineMaterials()
{
  // Material Database
  G4NistManager* nistManager = G4NistManager::Instance();

  // Setting up World
  fWorldMaterial = nistManager->FindOrBuildMaterial("G4_AIR");

  // Setting up default voxels
  fVoxelMaterial = nistManager->FindOrBuildMaterial("G4_AIR");

  // Define Elements (for lung)
  G4double a;
  G4double z;
  G4double density;
  G4String name;
  G4String symbol;
  a = 1.01*g/mole;
  G4Element* elH  = new G4Element(name="Hydrogen", symbol="H", z=1., a);
  a = 12.01*g/mole;
  G4Element* elC  = new G4Element(name="Carbon", symbol="C", z=6., a);
  a = 14.01*g/mole;
  G4Element* elN  = new G4Element(name="Nitrogen", symbol="N", z=7., a);
  a = 16.00*g/mole;
  G4Element* elO  = new G4Element(name="Oxygen", symbol="O", z=8., a);
  a = 22.99*g/mole;
  G4Element* elNa = new G4Element(name="Sodium", symbol="Na", z=11., a);
  a = 30.97*g/mole;
  G4Element* elP = new G4Element(name="Phosphorus", symbol="P", z=15., a);
  a = 32.07*g/mole;
  G4Element* elS = new G4Element(name="Sulfur", symbol="S", z=16., a);
  a = 35.45*g/mole;
  G4Element* elCl = new G4Element(name="Chlorine", symbol="Cl", z=17., a);
  a = 39.10*g/mole;
  G4Element* elK = new G4Element(name="Potassium", symbol="K", z=19., a);

  // Define lung
  G4int ncomponents;
  G4double fractionmass;
  density = 0.3*g/cm3;
  fLung = new G4Material(name="LUNG", density, ncomponents=9);
  fLung->AddElement(elH, fractionmass=0.103);
  fLung->AddElement(elC, fractionmass=0.105);
  fLung->AddElement(elN, fractionmass=0.031);
  fLung->AddElement(elO, fractionmass=0.749);
  fLung->AddElement(elNa, fractionmass=0.002);
  fLung->AddElement(elP, fractionmass=0.002);
  fLung->AddElement(elS, fractionmass=0.003);
  fLung->AddElement(elCl, fractionmass=0.003);
  fLung->AddElement(elK, fractionmass=0.002);

  // List of Materials
  fAir = nistManager->FindOrBuildMaterial("G4_AIR");
  fSoft = nistManager->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP");
  fCBone = nistManager->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  //fAdipose = nistManager->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");
  //fBrain = nistManager->FindOrBuildMaterial("G4_BRAIN_ICRP");
  //fBlood = nistManager->FindOrBuildMaterial("G4_BLOOD_ICRP");
  //fMuscle = nistManager->FindOrBuildMaterial("G4_MUSCLE_SKELETAL_ICRP");

  // Define bone (compact + spongeous)
  density = 1.4*g/cm3;
  fBone = new G4Material(name="BONE", density, ncomponents=2);
  fBone->AddMaterial(fCBone, fractionmass=0.5);
  fBone->AddMaterial(fSoft, fractionmass=0.5);

  return ;

}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{

  G4bool fCheckOverlaps = true;

  //////////////////////////////////////////////////////////////////////////////
  // World Volume
  //////////////////////////////////////////////////////////////////////////////

  G4double worldLenght = 0.2*m;

  G4Box*  fWorldSolid = new G4Box("world", worldLenght/2,worldLenght/2,worldLenght/2);

  // Assembly logical volumes
  fLogicalWorld = new G4LogicalVolume(fWorldSolid, fWorldMaterial, "World", 0,0,0);

  // World Volume
  PhysicsWorld = new G4PVPlacement(0,               // no rotation
                                   G4ThreeVector(), // at (0,0,0)
                                   fLogicalWorld,   // its logical volume
                                   "World",         // its name
                                   0,               // its mother  volume
                                   false,           // no boolean operations
                                   0,               // copy number
                                   fCheckOverlaps); // checking overlaps


  //////////////////////////////////////////////////////////////////////////////
  // Phantom Parametrisation
  //////////////////////////////////////////////////////////////////////////////

  // Defining the voxelPhantom parametrisation
  G4PhantomParameterisation* voxelizedPhantom = new G4PhantomParameterisation();

  // Setting up voxel dimension

  G4double halfVoxelSize = 0.25*mm;

  voxelizedPhantom->SetVoxelDimensions(halfVoxelSize, halfVoxelSize, halfVoxelSize);

  // Setting Phantom Dimension
  nVoxelX = 148;
  nVoxelY = 160;
  nVoxelZ = 160;

  voxelizedPhantom->SetNoVoxels(nVoxelX, nVoxelY, nVoxelZ);

  // Building Material "DataBase"
  std::vector<G4Material*> theMaterials;
  theMaterials.push_back(fAir);
  theMaterials.push_back(fLung);
  theMaterials.push_back(fSoft);
  theMaterials.push_back(fBone);
  //theMaterials.push_back(fAdipose);
  //theMaterials.push_back(fBrain);
  //theMaterials.push_back(fBlood);
  //theMaterials.push_back(fMuscle);

  voxelizedPhantom->SetMaterials(theMaterials);

  size_t* materialIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  //for (int i=0; i<nVoxelX*nVoxelY*nVoxelZ; i++) materialIDs[i]=0;

  // Importing CT
  std::ifstream ctfile("maps/CT_1.txt",std::ios::in);

  if(ctfile.fail()) std::cout << "no file\n";

  std::string buffer;

  G4int i, j, k;
  G4double HU; //Hounsfield units
  G4int n = 0; //Copy number

  while (ctfile >> buffer)
  {

    i = std::stoi(buffer);
    ctfile >> buffer;
    j = std::stoi(buffer);
    ctfile >> buffer;
    k = std::stoi(buffer);
    ctfile >> buffer;
    //HU = std::stoi(buffer);
    HU = std::stod(buffer);

    n = i+nVoxelX*j+nVoxelX*nVoxelY*k;

    if (HU < -3100) {materialIDs[n] = 0;}
    else if (HU < -200) {materialIDs[n] = 1;}
    else if (HU < 2900) {materialIDs[n] = 2;}
    else {materialIDs[n] = 3;}

  }

  //for (int i=0; i<nVoxelX*nVoxelY*nVoxelZ; i++) materialIDs[i]=2; //100% soft tissue

  voxelizedPhantom->SetMaterialIndices(materialIDs);

  G4Box* cont_solid = new G4Box("PhantomContainer",nVoxelX*halfVoxelSize,nVoxelY*halfVoxelSize,nVoxelZ*halfVoxelSize);
  G4LogicalVolume* cont_logic = new G4LogicalVolume( cont_solid, fWorldMaterial, "PhantomContainer", 0, 0, 0 );

  G4VPhysicalVolume * cont_phys = new G4PVPlacement(0, // rotation
                                                    G4ThreeVector(), // translation
                                                    cont_logic, // logical volume
                                                    "PhantomContainer", // name
                                                    fLogicalWorld, // mother volume
                                                    false, // No op. bool.
                                                    true);


  voxelizedPhantom->BuildContainerSolid(cont_phys);

  // Ensure that the voxels are completely filling the container volume

  voxelizedPhantom->CheckVoxelsFillContainer( cont_solid->GetXHalfLength(), cont_solid->GetYHalfLength(), cont_solid->GetZHalfLength() );

  voxelizedPhantom->SetSkipEqualMaterials(false);

  // The parameterised volume which uses this parameterisation is placed in the container logical volume

  G4VSolid* solVoxel = new G4Box("phantom",halfVoxelSize,halfVoxelSize,halfVoxelSize);
  G4LogicalVolume* logicVoxel = new G4LogicalVolume(solVoxel,fWorldMaterial,"phantom",0,0,0);

  logicVoxel->SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));

  G4PVParameterised * patient_phys = new G4PVParameterised("Patient", // name
                                                           logicVoxel, // logical volume
                                                           cont_logic, // mother volume
                                                           kUndefined, // optimisation hint
                                                           nVoxelX*nVoxelY*nVoxelZ, // number of voxels
                                                           voxelizedPhantom); // parameterisation

  // Indicate that this physical volume is having a regular structure

  patient_phys->SetRegularStructureId(0);

  return PhysicsWorld;
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define Materials
  DefineMaterials();

  return ConstructDetector();
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

void DetectorConstruction::ConstructSDandField()
{
  auto scintSD = new VoxelSD("VoxelSD", "VoxelHitsCollection");

  G4SDManager::GetSDMpointer()->AddNewDetector(scintSD);
  SetSensitiveDetector("phantom", scintSD);

  return ;
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
