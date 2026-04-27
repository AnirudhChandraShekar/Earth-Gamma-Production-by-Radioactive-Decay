#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"

// Defines the geometry of the simulation:
// - World volume (air)
// - Mantle region (peridotite composition)
// - Crust region (granite composition)
// - Detector surface at the top of the crust

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

private:
    void DefineMaterials();

    // Logical volumes (kept for sensitive detector assignment)
    G4LogicalVolume* fCrustLogical;
    G4LogicalVolume* fMantleLogical;
};

#endif
