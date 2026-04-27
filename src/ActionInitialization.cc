
#include "ActionInitialization.hh"
#include "RunAction.hh" //handles start and end of each run
#include "EventAction.hh" //handles start and end of each event
#include "PrimaryGeneratorAction.hh" 
#include "SteppingAction.hh" //for recording gammas at the production

ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());
}// Master thread only needs RunAction for accumulating results
    
void ActionInitialization::Build() const
{
    // Worker threads get all actions
    RunAction*   runAction   = new RunAction();
    EventAction* eventAction = new EventAction(runAction);

    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new SteppingAction());
}
