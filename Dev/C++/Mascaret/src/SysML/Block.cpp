#include "SysML/Block.h"

namespace SysML
{

Block::Block(const string& name)
: EntityClass(name), _isEncapsulated(false)
{
}

Block::~Block()
{
}

}
