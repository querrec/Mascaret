#include "BackupUtils/SlotBackup.h"

namespace BackupUtils
{
SlotBackup::SlotBackup(const shared_ptr<IBackupEnabled> obj) : Backup(obj)
{
}

SlotBackup::~SlotBackup()
{
	cerr << "SlotBackup::~SlotBackup()" << endl;
}
}
