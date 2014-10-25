#ifndef _v2_VEHA_Entity_ShapeSpecification_H
#define _v2_VEHA_Entity_ShapeSpecification_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/VirtualRealitySpecification.h"
#include "VEHA/Entity/Color.h"

using VEHA::Entity;

namespace VEHA
{

class VEHA_API ShapeSpecification : public VirtualRealitySpecification
{
public :
	ShapeSpecification(const string& name, const string & url, bool movable=true,bool recursive =false,const string& shader="");
	ShapeSpecification(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable=true,bool recursive = false,const string& shader="");
	virtual ~ShapeSpecification();

	// --- Attributes ---
	virtual void setVisibility(bool v)=0;
	virtual bool getVisibility() const=0;

	virtual void setScale(const Vector3& scale)=0;
	virtual Vector3 getScale() const=0;

	inline const string& getUrl() const
	{
		return _url;
	}
	inline const string& getName() const
	{
		return _name;
	}

	virtual void setEntity(shared_ptr<Entity> entity)=0;
	virtual shared_ptr<Entity> getEntity() const=0;

	// --- Operations ---
	virtual const ValueSpecification& operator= (const ValueSpecification&);
	virtual shared_ptr<ValueSpecification> clone() const;


	// --- Shape Modifier Operations ---
	virtual shared_ptr<Color> getColor()=0;
	virtual void setColor(shared_ptr<Color> color)=0;
	virtual void setTransparent(shared_ptr<Color> color)=0;
	virtual void growUp(int percent)=0;
	virtual bool restaureColor(shared_ptr<Color> color)=0;
	virtual bool restaureShape()=0;


protected:
	ShapeSpecification(const string& name);
	std::string _url;
	std::string _name;
};

}
#endif
