#include "BackupUtils/BackupList.h"

//temp
#include "VEHA/Kernel/InstanceSpecification.h"

namespace BackupUtils
{
shared_ptr<BackupList> BackupList::_instance; //static

//private constructor
BackupList::BackupList()
{
}

BackupList::~BackupList()
{
}

shared_ptr<BackupList> BackupList::getInstance() //static
{
	if (!_instance) _instance = shared_ptr<BackupList>(new BackupList());
	
	return _instance;
}

void BackupList::addBackup(shared_ptr<Backup> bk)
{
	//if (bk->getOwnerInstance()) cerr << " >>>>> >>> > " << bk->getOwnerInstance()->getName() << endl;
	
	/*
	bool gotPrimeBasedOnInstance = false;
	for (unsigned int i=0; i<_primeBackupsBasedOnInstance.size(); ++i)
	{
		if (_primeBackupsBasedOnInstance[i]->getOwnerInstance() == bk->getOwnerInstance())
		{
			gotPrimeBasedOnInstance = true;
			break;
		}
	}
	
	if (!gotPrimeBasedOnInstance)
	{
		_primeBackupsBasedOnInstance.push_back(bk);
	}
	else
	{
		_backups.push_back(bk);
	}
	
	cerr << "Backup engine: prime list size is " << _primeBackupsBasedOnInstance.size() << endl;
	*/
	
	bool gotPrime = false;
	for (unsigned int i=0; i<_primeBackups.size(); ++i)
	{
		if (_primeBackups[i]->_getReference() == bk->_getReference())
		{
			gotPrime = true;
			break;
		}
	}
	
	if (!gotPrime)
	{
		_primeBackups.push_back(bk);
	}
	else
	{
		_backups.push_back(bk);
	}
	
	//cerr << "Backup engine: prime list size is " << _primeBackups.size() << endl;
	//cerr << "Backup engine: list size is " << _backups.size() << endl;
}

void BackupList::undoLastChange()
{
	cerr << "Backup engine: executing undo..." << endl;
	for (unsigned int i=0; i<_primeBackups.size(); ++i)
	{
		_primeBackups[i]->restore();
	}
	if (_backups.size())
	{
		for (unsigned int i=0; i<_backups.size(); ++i)
		{
			_backups[i]->restore();
		}
		_backups.pop_back();
		cerr << "Backup engine: executed undo..." << endl;
	}
	else
	{
		cerr << "Backup engine: nothing to undo, sorry" << endl;
	}
}

void BackupList::undoLastChangeFor(shared_ptr<InstanceSpecification> is)
{
	cerr << "Backup engine: executing undo..." << endl;
	for (unsigned int i=0; i<_primeBackups.size(); ++i)
	{
		if (_primeBackups[i]->getOwnerInstance() == is.get()) _primeBackups[i]->restore();
	}
	if (_backups.size())
	{
		int index = _backups.size()-1;
		for (; index >= 0; --index)
		{
			if (_backups[index]->getOwnerInstance() == is.get()) break;
		}
		for (unsigned int i=0; i<index; ++i)
		{
			if (_backups[i]->getOwnerInstance() == is.get()) _backups[i]->restore();
		}
		_backups.erase(_backups.begin()+index);
		cerr << "Backup engine: executed undo..." << endl;
	}
	else
	{
		cerr << "Backup engine: nothing to undo, sorry" << endl;
	}
}
/*
void BackupList::undoLastChangeUnlessInstance(shared_ptr<InstanceSpecification> is) //test
{
	if (_backups.size())
	{
		int index=_backups.size()-1;
		for (; index>=0; --index)
		{
			if (_backups[index]->getOwnerInstance() != is.get())
			{
				break;
			}
		}
		if (index>=0)
		{
			cerr << "Backup engine: executing undo..." << endl;
			for (int i=0; i<_backups.size(); ++i)
			{
				shared_ptr<Backup> backup = _backups.back();
				backup->restore();
			}
			_backups.pop_back();
			cerr << "Backup engine: executed undo..." << endl;
			cerr << "Backup engine: executing undo..." << endl;
			shared_ptr<Backup> backup = _backups[index];
			backup->restore();
			_backups.erase(_backups.begin()+index);
			cerr << "Backup engine: executed undo..." << endl;
		}
	}
	else
	{
		cerr << "Backup engine: nothing to undo, sorry" << endl;
	}
}
*/
}
