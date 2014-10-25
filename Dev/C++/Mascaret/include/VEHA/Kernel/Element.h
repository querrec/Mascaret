#ifndef _v2_VEHA_Kernel_Element_H
#define _v2_VEHA_Kernel_Element_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/ID.h"

namespace VEHA
{
class Comment;

class VEHA_API Element :  public enable_shared_from_this<Element>
{
public :
	//,const string& summary="",const string& description="");
	Element(const string& name);
	virtual ~Element();

	inline ID getId(void) { return _id;}
	inline void setId(ID id) { _id = id;}

	inline const string& getName(void) const { return _name;}
	inline void setName(const string& name) { _name = name;}

	inline string getSummary(void) const { return _summary;}
	inline void setSummary(string summary) { _summary = summary;}

	inline string getDescription(void) const { return _description;}
	inline void setDescription(string description) { _description = description;}

	inline string getTag(string name) const
	{
		map<string, string>::const_iterator it=_tags.find(name);
		if(it != _tags.end())
			return it->second;
		return "";
	}
	inline void setTag(string name, string value) { _tags[name] = value;}
	inline void setTags(const map<string, string>& tags) { _tags = tags;}

	inline std::ostream & operator<<(std::ostream& os) const{return (os<<_name);}
	inline virtual string getFullName() const{return _name;}
protected :
	ID  _id;
	string  _name;
	string  _summary;
	string  _description;
	map<string, string> _tags;
#if 0
/* USE ? */
		// --- Relations ---
		//------ redefinedElement -----
protected :
	//		vector< Element * >	_redefinedElement;
public :
	inline vector<Element * > getRedefinedElement(void) { return _redefinedElement;}
	inline Element * getRedefinedElement_at(int index) { return _redefinedElement[index];}
	inline void addRedefinedElement(Element * value) { _redefinedElement.push_back(value);}
	//------ comment -----
protected :
	Comment * _comment;
public :
	inline Comment * getComment(void) { return _comment;}
	inline void setComment(Comment * comment) { _comment = comment;}
	// --- Operations ---
#endif
};

}
#endif
