#include "BackupUtils/ProcedureExecutionBackup.h"

namespace BackupUtils
{
ProcedureExecutionBackup::ProcedureExecutionBackup(const shared_ptr<IBackupEnabled> obj) : Backup(obj)
{
}

ProcedureExecutionBackup::~ProcedureExecutionBackup()
{
	cerr << "ProcedureExecutionBackup::~ProcedureExecutionBackup()" << endl;
}
}
