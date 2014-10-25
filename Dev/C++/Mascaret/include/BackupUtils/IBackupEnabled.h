#ifndef _BackupUtils_IBackupEnabled_H
#define _BackupUtils_IBackupEnabled_H

#include "Tools/veha_plateform.h"

#include "BackupUtils/Backup.h"

namespace BackupUtils
{
class VEHA_API IBackupEnabled // : public shared_from_this<IBackupEnabled>
{
public :
	void createBackup(); //using 'template' design pattern to call the hook
	
	virtual void setBackup(shared_ptr<Backup>) = 0;

protected :
	virtual shared_ptr<Backup> _createBackupHook() = 0; //implement specific backup creation
};
}

#endif
