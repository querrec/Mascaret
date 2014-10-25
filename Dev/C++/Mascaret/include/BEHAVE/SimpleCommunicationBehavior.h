#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/Slot.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/Entity.h"
#include "BEHAVE/OneShotBehaviorExecution.h"
#include "BEHAVE/CyclicBehaviorExecution.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AID.h"
#include "BEHAVE/ACLMessage.h"
#include "HAVE/VirtualHuman.h"


namespace BEHAVE
{

class VEHA_API SimpleCommunicationBehavior : public BEHAVE::CyclicBehaviorExecution
{
public:
	SimpleCommunicationBehavior(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host,const Parameters& p) ;

	virtual void action ();

protected :
	virtual void _manageRequestAction(shared_ptr<ACLMessage> msg);
	virtual void _manageRequest(shared_ptr<ACLMessage> msg);
	virtual void _manageInform(shared_ptr<ACLMessage> msg);
	virtual void _manageQueryRef(shared_ptr<ACLMessage> msg);
	virtual void _sendModel(shared_ptr<ACLMessage> msg);
	virtual void _sendOrganisation(shared_ptr<ACLMessage> msg);
	virtual void _sendActions(shared_ptr<ACLMessage> msg);
	virtual void _sendEntities(shared_ptr<ACLMessage> msg);
	//virtual void _sendLearningSession(shared_ptr<ACLMessage> msg);
	virtual void _suscribeEntitiesChanged(shared_ptr<ACLMessage> msg);
	virtual shared_ptr<XmlNode> _buildWorldInfo();
	void _onBehaviorStop(BehaviorExecution * be);
	void _onSlotChanged(string, shared_ptr<ValueSpecification>);
	map < string, vector<shared_ptr<AID> > > _requestedAction;
	vector<pair<string, shared_ptr<AID> > > _requestedSlotChanged;
	bool _callbackOnEntitiesAdded;
	
public :
//temporary operation->action translation feature
	vector<shared_ptr<ActionNode> > _translateOperationToActions(shared_ptr<Operation> operation);
	
};
}
