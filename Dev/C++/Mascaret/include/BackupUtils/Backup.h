#ifndef _BackupUtils_Backup_H
#define _BackupUtils_Backup_H

#include "Tools/veha_plateform.h"

namespace VEHA
{
class InstanceSpecification;
}

namespace BackupUtils
{
class IBackupEnabled;
class BackupList;

using VEHA::InstanceSpecification;

class VEHA_API Backup :  public enable_shared_from_this<Backup>
{
friend class BackupList;

public :
	Backup(const shared_ptr<IBackupEnabled> ref);
	virtual ~Backup();
	
	void restore();
	
	time_t getTimestamp();
	
	//Owner instance
	void setOwnerInstance(InstanceSpecification *is) { _ownerInstance = is; }
	InstanceSpecification* getOwnerInstance() { return _ownerInstance; }
	
private :
	shared_ptr<IBackupEnabled> _reference;
	shared_ptr<IBackupEnabled> _getReference() { return _reference; }
	time_t _timestamp;
	
	InstanceSpecification *_ownerInstance;
};
}

#endif
