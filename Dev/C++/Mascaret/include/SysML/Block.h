#ifndef _v2_SYSML_Block_H
#define _v2_SYSML_Block_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/EntityClass.h"

using namespace VEHA;

namespace SysML
{

class VEHA_API Block : public EntityClass
{
public :
	Block(const string& name);
	virtual ~Block();

	inline bool isEncasulated(void){return _isEncapsulated;}
	inline void setIsEncapsulated(bool isEncapsulated){_isEncapsulated = isEncapsulated;}

private :
	bool _isEncapsulated; // True if block is a kind of black box system
};

}
#endif
