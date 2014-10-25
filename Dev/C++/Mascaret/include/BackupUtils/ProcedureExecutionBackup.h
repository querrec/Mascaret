#ifndef _BackupUtils_ProcedureExecutionBackup_H
#define _BackupUtils_ProcedureExecutionBackup_H


#include "Tools/veha_plateform.h"

#include "BackupUtils/Backup.h"

#include "BEHAVE/ProcedureExecution.h"
#include "BEHAVE/AID.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"

namespace BackupUtils
{
using VEHA::ActivityPartition;
using VEHA::ActivityNode;
using VEHA::ActionNode;
using VEHA::ActivityEdge;
using BEHAVE::AID;
using BEHAVE::ProcedureExecution;

class VEHA_API ProcedureExecutionBackup : public Backup
{
friend class ProcedureExecution; //allow direct access for owner

public :
	ProcedureExecutionBackup(const shared_ptr<IBackupEnabled> obj);
	~ProcedureExecutionBackup();
	
protected:
	vector < shared_ptr<ActionNode> > runningNodes;
	vector < shared_ptr<ActivityEdge> > toExecuteNodes;
	vector < shared_ptr<ActivityEdge> > lastToExecuteNodes;
	vector<pair<string,string> > actionsDone;
	map<string, vector<string> > allActionsDone;
	vector<pair<string,string> > actionsRunning;
	vector< shared_ptr<ActionNode> >forkFirstActionsDone;
	vector< shared_ptr<ActionNode> >forkLastActionsDone;
	map<string, shared_ptr<ActivityPartition> > agentToPartition;
	map<shared_ptr<ActivityPartition>, shared_ptr<AID> > partitionToAgent;
};
}

#endif

