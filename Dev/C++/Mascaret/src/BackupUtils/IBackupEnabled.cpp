#include "BackupUtils/IBackupEnabled.h"

#include "BackupUtils/Backup.h"
#include "BackupUtils/BackupList.h"

namespace BackupUtils
{
void IBackupEnabled::createBackup()
{
#if BACKUP_ACTIVATED
	shared_ptr<Backup> backup = _createBackupHook();
	
	//add backup to singleton list
	if (backup)
	{
		BackupList::getInstance()->addBackup(backup);
	}
#endif
}
}
