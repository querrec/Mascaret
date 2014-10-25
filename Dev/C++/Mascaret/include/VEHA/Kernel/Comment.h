#ifndef _v2_VEHA_Kernel_Comment_H
#define _v2_VEHA_Kernel_Comment_H

#include "Tools/veha_plateform.h"
namespace VEHA
{

class VEHA_API Comment
{
public :
	Comment();
	virtual ~Comment();
	// --- Attributes ---
	//------ body -----
protected :
	string  _body;
public :
	inline string getBody(void) { return _body;}
	inline void setBody(string body) { _body = body;}
	// --- Relations ---
	// --- Operations ---
};

}
#endif
