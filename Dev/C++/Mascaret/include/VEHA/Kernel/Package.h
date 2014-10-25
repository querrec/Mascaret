#ifndef _v2_VEHA_Kernel_Package_H
#define _v2_VEHA_Kernel_Package_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"

namespace VEHA
{
class Signal;
class Class;
class Association;

class VEHA_API Package : public Element
{
public :
	Package(const string& name);
	virtual ~Package();
	inline virtual string getFullName() const{return (getParentPackage()?getParentPackage()->getFullName()+"::"+getName():getName());}
	// --- Attributes ---
	// --- Relations ---
	//------ classes -----
protected :
	vector< shared_ptr<Class> > _classes;
public :
	inline const vector< shared_ptr<Class> >& getClasses(void) const { return _classes;}
	shared_ptr<Class>  getClass(const string& name) const ;
	inline shared_ptr<Class> getClasses_at(int index) { return _classes[index];}
	void addClasses(shared_ptr<Class> value);
	//------ classes -----
protected :
	vector< shared_ptr<Signal> >  _signals;
public :
	inline const vector< shared_ptr<Signal> >& getSignals(void) const{ return _signals;}
	inline shared_ptr<Signal> getSignals_at(int index) { return _signals[index];}
	inline void addSignals(shared_ptr<Signal> value) { _signals.push_back(value);}
	//------ associations -----
protected :
	//------ packages -----
protected :
	weak_ptr<Package> _parentPackage;
	vector< shared_ptr<Package> >   _packages;
public :
	inline shared_ptr<Package> getParentPackage()const {return _parentPackage.lock();}
	inline const vector< shared_ptr<Package> >& getPackages(void)const  { return _packages;}
	inline shared_ptr<Package> getPackages_at(int index) { return _packages[index];}
	inline void addPackage(shared_ptr<Package> value)
	{
		_packages.push_back(value);
		value->_parentPackage=shared_dynamic_cast<Package> (shared_from_this());
	}
	// --- Operations ---
};

}
#endif
