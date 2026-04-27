// DetectorConstruction.cc
// Geometry (all thicknesses are simplified slabs — adjust to your model):
//
//   World (air)
//   └── Mantle slab  (200 km thick, peridotite)
//       └── Crust slab (35 km thick, granite)
//           └── Detector layer (1 m thin layer at top of crust)
//
// The detector layer is a thin scoring surface. Real Earth curvature is
// ignored at this stage — extend to a spherical shell geometry if needed.
//
// Material compositions are approximate averages. Replace with your
// model's exact elemental compositions and densities.
//=============================================================================

#include "DetectorConstruction.hh"
#include "G4NistManager.hh" 

#include "G4Material.hh"
#include "G4Element.hh"
//built in database of real materials and elements

#include "G4Box.hh" //shape of box
#include "G4LogicalVolume.hh" //shape+material
#include "G4PVPlacement.hh" //places it in a position
#include "G4SystemOfUnits.hh" 
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
// control how volume appears in the visualier

DetectorConstruction::DetectorConstruction()
    : fCrustLogical(nullptr),
      fMantleLogical(nullptr)
      {} //Initialize the 3 logical volumes 

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance(); 

    G4Element* elO  = nist->FindOrBuildElement("O");
    G4Element* elSi = nist->FindOrBuildElement("Si");
    G4Element* elAl = nist->FindOrBuildElement("Al");
    G4Element* elFe = nist->FindOrBuildElement("Fe");
    G4Element* elCa = nist->FindOrBuildElement("Ca");
    G4Element* elMg = nist->FindOrBuildElement("Mg");
    G4Element* elNa = nist->FindOrBuildElement("Na");
    G4Element* elK  = nist->FindOrBuildElement("K");

    // Granite (continental crust average composition by mass fraction)
    // TODO: Replace fractions with your model's crust composition
    G4Material* granite = new G4Material("Granite", 2.7*g/cm3, 8);
    granite->AddElement(elO,  0.4639);
    granite->AddElement(elSi, 0.3195);
    granite->AddElement(elAl, 0.0801);
    granite->AddElement(elFe, 0.0350);
    granite->AddElement(elCa, 0.0253);
    granite->AddElement(elMg, 0.0153);
    granite->AddElement(elNa, 0.0280);
    granite->AddElement(elK,  0.0329);

    // Peridotite (upper mantle average composition by mass fraction)
    // TODO: Replace fractions with your model's mantle composition
    G4Material* peridotite = new G4Material("Peridotite", 3.3*g/cm3, 6);
    peridotite->AddElement(elO,  0.4404);
    peridotite->AddElement(elSi, 0.2100);
    peridotite->AddElement(elMg, 0.2280);
    peridotite->AddElement(elFe, 0.0626);
    peridotite->AddElement(elCa, 0.0253);
    peridotite->AddElement(elAl, 0.0235);

    // Air for world volume (use NIST definition)
    nist->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    DefineMaterials();

    G4NistManager* nist = G4NistManager::Instance();
    G4Material* air       = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* granite   = G4Material::GetMaterial("Granite");
    G4Material* peridotite= G4Material::GetMaterial("Peridotite");

    // Dimensions
    // These are simplified flat slabs. Extend to G4Sphere for full Earth model.
    // X and Y are arbitrary lateral extent; Z is depth.
    G4double worldXY    = 1000.0*km; // x and y are 1000 km each
    G4double worldZ     = 250.0*km; // z is 250 km

    G4double mantleThick = 200.0*km; //thickness of mantle
    G4double crustThick  =  35.0*km; //thickness of crust

    G4double halfXY      = worldXY / 2.0; //Geant4 construction wants half width
    
    G4Box* worldSolid = new G4Box("World", halfXY, halfXY, worldZ/2.0); //Creating a world 
    G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, air, "World"); //Fill it up with air
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), worldLogical, "World", nullptr, false, 0); //placing it

    worldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Z position: bottom of world to bottom of crust
    G4double mantleZ = -worldZ/2.0 + mantleThick/2.0;

    G4Box* mantleSolid = new G4Box("Mantle",halfXY, halfXY, mantleThick/2.0);
    fMantleLogical = new G4LogicalVolume(mantleSolid, peridotite, "Mantle");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,mantleZ),fMantleLogical, "Mantle", worldLogical, false, 0); //mantle lives inside the world

    G4VisAttributes* mantleVis = new G4VisAttributes(G4Colour(0.8, 0.4, 0.1, 0.3)); //setting visualisation of mantle
    mantleVis->SetForceSolid(true);
    fMantleLogical->SetVisAttributes(mantleVis);

    // Crust slab (sits on top of mantle)
    G4double crustZ = -worldZ/2.0 + mantleThick + crustThick/2.0;

    G4Box* crustSolid = new G4Box("Crust",halfXY, halfXY, crustThick/2.0);
    fCrustLogical = new G4LogicalVolume(crustSolid, granite, "Crust");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,crustZ), fCrustLogical, "Crust", worldLogical, false, 0);

    G4VisAttributes* crustVis = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6, 0.4));
    crustVis->SetForceSolid(true);
    fCrustLogical->SetVisAttributes(crustVis);

    return worldPhysical;
}

void DetectorConstruction::ConstructSDandField()
{
    // No sensitive detector needed - scoring done via SteppingAction
}