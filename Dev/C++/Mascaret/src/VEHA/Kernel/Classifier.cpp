#include "VEHA/Kernel/Classifier.h"
#include <boost/lambda/algorithm.hpp>
#include <boost/bind.hpp>
#include "VEHA/Behavior/Common/Behavior.h"
//using namespace boost::lambda;

#include <boost/foreach.hpp>


namespace VEHA
{

Classifier::Classifier(const string& name)
:Element(name),_isAbstract(false)
{
}

Classifier::~Classifier()
{
}
void Classifier::_addFeaturesToChild(shared_ptr<Classifier> child)
{
	pair<string,shared_ptr<Behavior> > b;
	BOOST_FOREACH(b,_ownedBehavior)
	child->addOwnedBehavior(b.second);
}

void Classifier::addChild(shared_ptr<Classifier> child)
{
	child->accessParents().push_back(shared_dynamic_cast<Classifier>(shared_from_this()));
	_children.push_back(child);
	_addFeaturesToChild(child);
}
bool Classifier::isA(shared_ptr<Classifier> classifier) const
{
	if (classifier == shared_from_this()) return true;
	else
	{
		vector<shared_ptr<Classifier> > parents=getParents();
		return (find_if(parents.begin(),parents.end(), bind(&Classifier::isA,_1,classifier ))
				!=parents.end());
	}

}
bool Classifier::isAByName(const string& classifierName) const
{
	if (classifierName == getName()) return true;
	else if (classifierName == getFullName()) return true;
	else
	{
		vector<shared_ptr<Classifier> > parents=getParents();
		for(size_t i=0;i<parents.size();i++)
		{
			if(parents[i]->isAByName(classifierName))
				return true;
		}
		// Ca ne passe pas lorsque C++0x est activé.
		//return (find_if(parents.begin(),parents.end(), bind(&Classifier::isAByName,_1, classifierName ))!=parents.end());
	}
	return false;
}
void Classifier::addOwnedBehavior(shared_ptr<Behavior> value)
{
       //cerr << getName() << " add behavior : " << value->getName() << endl;

	_ownedBehavior.insert(make_pair(value->getName(),value));

	//Pass to children
	for (size_t i=_children.size();i--;)
		_children[i].lock()->addOwnedBehavior(value);

}

}
