#ifndef _BackupUtils_BackupList_H
#define _BackupUtils_BackupList_H

#include "Tools/veha_plateform.h"

#include "BackupUtils/Backup.h"

namespace BackupUtils
{
class VEHA_API BackupList :  public enable_shared_from_this<BackupList>
{
public :
	virtual ~BackupList();
	
	void undoLastChange();
	void undoLastChangeFor(shared_ptr<InstanceSpecification>);
	//void undoLastChangeUnlessInstance(shared_ptr<InstanceSpecification>); //test
	
	void addBackup(shared_ptr<Backup>);
	
	static shared_ptr<BackupList> getInstance();
	
private :
	BackupList(); //Singleton
	static shared_ptr<BackupList> _instance;
	
	vector<shared_ptr<Backup> > _backups;
	vector<shared_ptr<Backup> > _primeBackups;
	//vector<shared_ptr<Backup> > _primeBackupsBasedOnInstance; //Thou shalt not delete these
};
}

#endif
