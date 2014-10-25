#ifndef BEHAVE_AID_H
#define BEHAVE_AID_H

#include "Tools/veha_plateform.h"

namespace BEHAVE
{
class VEHA_API AID
{
public :
	AID(const string & name, string plateformName= "", int portNumber = 0);
	virtual ~AID();

	inline string getName() { return _name;}
	inline string getPlateformName() { return _plateformName;}
	inline void setPlateformName(string plateformName) { _plateformName = plateformName;}
	inline int getPlateformPort() { return _port;}
	string toString();
	static shared_ptr<AID> parseFromString(const string& aidStr);

	/* Ces operateurs ne sont visiblement pas appelé avec des shared_ptr !!
         inline bool operator==(const AID& aid)
		{
        	 cerr << "AID: operator ==" << endl;
        	 return _name == aid._name && _plateformName == aid._plateformName && _port == aid._port;
		}
         inline bool operator<(const AID& aid)
        {
        	 if(_name != aid._name)
        		 return _name < aid._name;
        	 if(_plateformName != aid._plateformName )
        		 return _plateformName < aid._plateformName;
        	 if(_port != aid._port)
        		 return _plateformName < aid._plateformName;
        	 return false;
        }*/
public :

	string _name;
	string _plateformName;
	int _port;


};
}

#endif

