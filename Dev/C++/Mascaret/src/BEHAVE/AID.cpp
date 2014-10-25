#include "BEHAVE/AgentPlateform.h"
#include "BEHAVE/AID.h"
#include <cstdio>

namespace BEHAVE
{

AID::AID(const string & name, string plateformName, int portNumber) : _name(name), _plateformName(plateformName), _port(portNumber)
{}

AID::~AID()
{}

string AID::toString()
{
	char tmp[1024];
	sprintf(tmp,"%s@%s:%d",_name.c_str(),_plateformName.c_str(),_port);
	return tmp;
}
shared_ptr<AID> AID::parseFromString(const string& aidStr)
{
	size_t pos=aidStr.find('@');
	if(pos != string::npos)
	{
		string name=aidStr.substr(0,pos);
		size_t pos2=aidStr.find(':',pos);
		if(pos2 != string::npos)
		{
			string plateformName=aidStr.substr(pos+1,pos2-pos-1);
			string portStr=aidStr.substr(pos2+1);
			return shared_ptr<AID>(new AID(name,plateformName,atoi(portStr.c_str())));
		}
	}
	return shared_ptr<AID>();
}
}
