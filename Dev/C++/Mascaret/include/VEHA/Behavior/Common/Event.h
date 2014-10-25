#ifndef _v2_VEHA_Behavior_Common_Event_H
#define _v2_VEHA_Behavior_Common_Event_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"

namespace VEHA
{

class VEHA_API Event : public Element
{
public :
	Event(const string& name);
	virtual ~Event();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
	inline string getType() {return _type;}
	inline void setType(string type) {_type = type;}
protected :
	string _type;
};

}
#endif
