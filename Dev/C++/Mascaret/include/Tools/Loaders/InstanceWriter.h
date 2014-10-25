#ifndef INSTANCE_SAVER_H
#define INSTANCE_SAVER_H
#include "Tools/veha_plateform.h"
#include <string>
#include <fstream>
using boost::shared_ptr;

namespace VEHA
{
class Environment;
class Entity;
}


class VEHA_API InstanceWriter
{
public:
	InstanceWriter(shared_ptr<VEHA::Environment> environment, const std::string& filename);
protected:
	std::string _basedir;
	shared_ptr<VEHA::Environment> _environment;

	void _writeEntity(std::ofstream& file,shared_ptr<VEHA::Entity> entity,const std::string& startLine="");

};
#endif
