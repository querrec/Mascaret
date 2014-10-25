#ifndef _BackupUtils_SlotBackup_H
#define _BackupUtils_SlotBackup_H


#include "Tools/veha_plateform.h"

#include "BackupUtils/Backup.h"

#include "VEHA/Kernel/Slot.h"
#include "VEHA/Kernel/ValueSpecification.h"


namespace BackupUtils
{
using VEHA::Slot;
using VEHA::ValueSpecification;

class VEHA_API SlotBackup : public Backup
{
friend class Slot; //allow direct access for owner

public :
	SlotBackup(const shared_ptr<IBackupEnabled> obj);
	~SlotBackup();
	
protected:
	map<string,shared_ptr<ValueSpecification> > _values;
};
}

#endif

