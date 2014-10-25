#ifndef BEHAVE_PROCEDURE_H
#define BEHAVE_PROCEDURE_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Behavior/Activity/Activity.h"


namespace BEHAVE
{
using VEHA::Element;
using VEHA::Activity;
class Ressource;
class VEHA_API Procedure : public Element
{
public :
	Procedure(const string & name);
	virtual ~Procedure();

	void setActivity(shared_ptr<Activity> activity) {_activity = activity;}
	shared_ptr<Activity> getActivity(void) {return _activity;}

private :
	shared_ptr<Activity> _activity;

	// Ressources
public :
	inline void addRessource(shared_ptr<Ressource> ressource) {_ressources.push_back(ressource);}
	inline vector < shared_ptr<Ressource> > getRessources(void) {return _ressources;}
private :
	vector < shared_ptr<Ressource> > _ressources;


};
}

#endif

