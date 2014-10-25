#ifndef _v2_VEHA_Kernel_Property_H
#define _v2_VEHA_Kernel_Property_H
#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Feature.h"
#include "VEHA/Kernel/Class.h"

class ModelLoader;

namespace VEHA
{
class Class;
class ValueSpecification;
class DomainSpecification;
class Classifier;

class VEHA_API Property :  public Feature
{
public :
	Property( const string& name,Class* cl,
			shared_ptr<Classifier> type,
			shared_ptr<Property> opposite=shared_ptr<Property>(),
			shared_ptr<ValueSpecification> def=shared_ptr<ValueSpecification>(),
			shared_ptr<DomainSpecification> domain=shared_ptr<DomainSpecification>()
	);
	virtual ~Property();

	inline virtual string getFullName() const {return getClassifier()->getFullName()+"."+getName();}
	inline shared_ptr<ValueSpecification> createValueFromString(const string& str) {return getType()->createValueFromString(str);}

	inline bool getIsReadOnly(void) { return _isReadOnly;}
	inline void setIsReadOnly(bool isReadOnly) { _isReadOnly = isReadOnly;}

	inline bool getIsDerived(void) { return _isDerived;}
	inline void setIsDerived(bool isDerived) { _isDerived = isDerived;}

	inline bool getIsComposite(void) { return _isComposite;}
	inline void setIsComposite(bool isComposite) { _isComposite = isComposite;}

	inline bool getIsNavigable(void) { return _isNavigable;}
	inline void setIsNavigable(bool isNavigable) { _isNavigable = isNavigable;}

	inline bool getIsOrdered(void) { return _isOrdered;}
	inline void setIsOrdered(bool isOrdered) { _isOrdered = isOrdered;}

	inline bool getIsUnique(void) { return _isUnique;}
	inline void setIsUnique(bool isUnique) { _isUnique = isUnique;}

	inline unsigned int getMaximumNumber(void) { return _maximumNumber;}
	inline void setMaximumNumber(unsigned int maximumNumber) { _maximumNumber = maximumNumber;}

	inline unsigned int  getMinimumNumber(void) { return _minimumNumber;}
	inline void setMinimumNumber(unsigned int minimumNumber) { _minimumNumber = minimumNumber;}

	inline shared_ptr<Class> getClassifier(void) const{ return shared_dynamic_cast<Class>(_classifier->shared_from_this());}
	inline void setOpposite(shared_ptr<Property> opposite) { _opposite = opposite;}

	inline void setDefaultValue(shared_ptr<ValueSpecification> defaultValue) { _defaultValue = defaultValue;}
	inline shared_ptr<ValueSpecification> getDefaultValue(void) { return _defaultValue;}

	inline shared_ptr<DomainSpecification> getDomain(void) { return _domain;}
	//		inline void setDomain(DomainSpecification * domain) { _domain = domain;}
	inline shared_ptr<Property> getOpposite(void) { return _opposite;}
	inline shared_ptr<Classifier> getType(void) { return _type.lock();}
	//		inline void setType(shared_ptr<Classifier> type) { _type = type;}

protected:
	friend class ::ModelLoader;
	weak_ptr<Classifier>  _type;
	Class* _classifier;
	shared_ptr<Property>    _opposite;
	shared_ptr<ValueSpecification>  _defaultValue;
	shared_ptr<DomainSpecification> _domain;
	bool    _isComposite;
	bool _isNavigable;
	bool    _isReadOnly;
	bool    _isDerived;
	bool    _isUnique;
	bool    _isOrdered;
	unsigned int    _minimumNumber;
	unsigned int    _maximumNumber;
};

}
#endif
