#include "BackupUtils/Backup.h"

#include "BackupUtils/IBackupEnabled.h"

#include "VEHA/Kernel/InstanceSpecification.h"

//#include <sys/time.h>

namespace BackupUtils
{

Backup::Backup(const shared_ptr<IBackupEnabled> ref) :
	_reference(ref), _timestamp(time(0)), _ownerInstance(0)
{
}

Backup::~Backup()
{
	cerr << "Backup::~Backup()" << endl;
}

void Backup::restore()
{
	_reference->setBackup(shared_ptr<Backup>(shared_from_this()));
}

time_t Backup::getTimestamp()
{
	return _timestamp;
}
}
