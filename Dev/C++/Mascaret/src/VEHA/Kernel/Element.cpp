#include "VEHA/Kernel/Element.h"
#include "VEHA/Kernel/Comment.h"


namespace VEHA
{

Element::Element(const string& name)
:_name(name)
/*
Element::Element(const string& name,const string& summary,
				const string& description)
:_name(name),_summary(summary),_description(description)*/
{
}

Element::~Element()
{
	//cerr<<"Destroying "<<_name<<endl;
}

}
