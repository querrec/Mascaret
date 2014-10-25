#ifndef _v2_VEHA_Kernel_Classifier_H
#define _v2_VEHA_Kernel_Classifier_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"

namespace VEHA
{
class Behavior;
class ValueSpecification;

class VEHA_API Classifier : public Element
{
public :
	Classifier(const string& name);
	virtual ~Classifier();
	// --- Attributes ---
	//------ isAbstract -----
public :
	inline bool getIsAbstract(void) { return _isAbstract;}
	inline void setIsAbstract(bool isAbstract) { _isAbstract = isAbstract;}
	//inline const vector<shared_ptr<Classifier> >& getParents()const {return _parent;}

	//inline const vector<shared_ptr<Classifier> >& getChildren()const {return _children;}
	inline vector<shared_ptr<Classifier> > getParents()const
                		{
		vector<shared_ptr<Classifier> > parents;
		for(size_t i=0;i<_parent.size();i++)
			parents.push_back(_parent[i].lock());
		return parents;
                		}

	inline vector<shared_ptr<Classifier> > getChildren()const
						{
		vector<shared_ptr<Classifier> > children;
		for(size_t i=0;i<_children.size();i++)
			children.push_back(_children[i].lock());
		return children;
						}

	void addChild(shared_ptr<Classifier> child);
	virtual shared_ptr<ValueSpecification> createValueFromString(const string& str)=0;
	// --- Relations ---
	//------ generalization -----
	typedef map<string,shared_ptr<Behavior> > OwnedBehaviorMap;
protected :
	virtual void _addFeaturesToChild(shared_ptr<Classifier> child);
	bool    _isAbstract;

	/*
                        vector<Generalization * >	_generalization;
                    public :
                        inline vector<Generalization * > getGeneralization(void) { return _generalization;}
                        inline Generalization * getGeneralization_at(int index) { return _generalization[index];}
                        inline void addGeneralization(Generalization * value) { _generalization.push_back(value);}
	 */
	//------ behaviors -----
	//
protected:
	vector<weak_ptr<Classifier> > _parent;
	vector<weak_ptr<Classifier> > _children;
	inline vector<weak_ptr<Classifier> >& accessParents() {return _parent;}

protected :
	OwnedBehaviorMap _ownedBehavior;
public :
	inline const OwnedBehaviorMap getOwnedBehavior(void) const
	{
		OwnedBehaviorMap temp(_ownedBehavior);

		for(size_t i=0;i<_parent.size();i++)
		{
			OwnedBehaviorMap parentBehaviors=_parent[i].lock()->getOwnedBehavior();
			temp.insert(parentBehaviors.begin(),parentBehaviors.end());
		}
		return temp;
	}
	inline OwnedBehaviorMap& access_ownedBehavior(void) { return _ownedBehavior;}
	inline shared_ptr<Behavior> getOwnedBehavior(const string&  key) { return _ownedBehavior[key];}
	void addOwnedBehavior(shared_ptr<Behavior> value);
	// --- Operations ---
public :
	bool isA(shared_ptr<Classifier> classifier) const;
	bool isAByName(const string& classifierName) const;
};

}
#endif
