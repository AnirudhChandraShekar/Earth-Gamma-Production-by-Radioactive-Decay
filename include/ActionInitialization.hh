#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

// Registers all user action classes:
// - PrimaryGeneratorAction (GPS source)
// - RunAction (output file management)
// - EventAction (per-event scoring)

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization();
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;
};

#endif
