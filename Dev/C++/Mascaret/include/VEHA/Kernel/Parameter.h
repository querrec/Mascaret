#ifndef _v2_VEHA_Kernel_Parameter_H
#define _v2_VEHA_Kernel_Parameter_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
class ModelLoader;

namespace VEHA
{
class Classifier;

class VEHA_API Parameter : public Element
{
public :
	Parameter(const string& name,const string& direction,shared_ptr<Classifier> type=shared_ptr<Classifier>());
	virtual ~Parameter();
	// --- Attributes ---
	//------ direction -----
protected :
	string  _direction;
public :
	inline string getDirection(void) { return _direction;}
	//		inline void setDirection(string direction) { _direction = direction;}
	// --- Relations ---
	//------ type -----
protected :
	weak_ptr<Classifier>  _type;
	friend class ::ModelLoader;
public :
	inline void setType(shared_ptr<Classifier> type) { _type = type;}
	inline shared_ptr<Classifier> getType(void) { return _type.lock();}
	// --- Operations ---
};

}
#endif
