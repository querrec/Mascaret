#include "VEHA/Kernel/expressionParser.h"

#include <utility>
#include <stdexcept>

namespace VEHA {
namespace OclParser {
AnyValue evaluate(iter_t const& it , Context const& c);
}
}
using namespace VEHA;
using namespace VEHA::OclParser;



namespace _collectionMethods
{ 

using std::make_pair;
using std::runtime_error;

#define assertParameters(X) 	if (!(X) )throw runtime_error("OCL:Error in calling a Collection Method : wrong number of parameters : "#X  );

AnyValue isEmpty(const CollectionAnyValue& c,const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	return c.empty();
}
 AnyValue size(const CollectionAnyValue& c,const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	return (int)c.size();
}
AnyValue notEmpty(const CollectionAnyValue& c,const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	return (!c.empty());
}
 AnyValue sum(const CollectionAnyValue& c,const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	if (c.size())
	{
		AnyValue s=c[c.size()-1];
		for(size_t i=c.size()-1;i--;)
			s=s+c[i];
		return s;
	}
	else return 0;
}
AnyValue includes(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	bool found=false;
	for(size_t i=c.size();i-- && !found;)
		if (c[i]==parameters[0])found=true;
	
	return found;
}

AnyValue  excludes(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	bool found=false;
	for(size_t i=c.size();i-- && !found;)
		if (c[i]==parameters[0])found=true;
	
	return !found;
}
AnyValue count(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	AnyValue elem=parameters[0];
	int acc=0;
	for(size_t i=c.size();i--;)
		if (c[i]==elem)acc++;
	return acc;
}
AnyValue includesAll(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue c2=parameters[0];//TODO verif Collec sinon erreur
	bool found=true;
	for (size_t i2=c2.size();i2-- && found;)
	{
		found=false;
		for(size_t i=c.size();i-- && !found;)
			if (c[i]==c2[i2])found=true;
	}
	return found;
	
}
AnyValue  excludesAll(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue c2=parameters[0];//TODO verif Collec sinon erreur
	bool found=false;
	for (size_t i2=c2.size();i2-- && !found;)
	{
		for(size_t i=c.size();i-- && !found;)
			if (c[i]==c2[i2])found=true;
	}
	return !found;
}
AnyValue product(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue c2=parameters[0];//TODO verif Collec sinon erreur
	CollectionAnyValue result;
	for(size_t i=0;i<c.size();i++)
		for(size_t i2=0;i2<c2.size();i2++)
	{
		CollectionAnyValue p;
		p.push_back(c[i]);
		p.push_back(c2[i2]);
		result.push_back(p);
	}
	stable_sort(result.begin(),result.end());
	result.erase(unique(result.begin(),result.end()),result.end());
	result.type=CollectionAnyValue::Set;
	return result;	
}
 
//set
AnyValue unionFunction(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue c2=parameters[0];//TODO verif Collec sinon erreur
	CollectionAnyValue result;

	for(size_t i=0;i<c.size();i++)
	result.push_back(c[i]);
	for(size_t i2=0;i2<c2.size();i2++)
	result.push_back(c2[i2]);
	if (c.type==CollectionAnyValue::Sequence)
	{
		stable_sort(result.begin(),result.end());
	}
	else 
	if (c.type==CollectionAnyValue::Set && c2.type==CollectionAnyValue::Set)
	{
		stable_sort(result.begin(),result.end());
		result.erase(unique(result.begin(),result.end()),result.end());
		result.type=c.type;
	}
	else result.type=CollectionAnyValue::Bag;
	return result;
	
}
AnyValue  intersection(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue c2=parameters[0];//TODO verif Collec sinon erreur
	CollectionAnyValue result;//TODO diff set/Bag pour c2
	if (c2.type!=CollectionAnyValue::Set) //if not Set change it
	{
		stable_sort(c2.begin(),c2.end());
		c2.erase(unique(c2.begin(),c2.end()),c2.end());
	}
	
	for(size_t i2=0;i2<c2.size();i2++)
	{
		for(size_t i=c.size();i-- ;)
			if (c[i]==c2[i2])result.push_back(c[i]);
	}
	result.type=c.type;
	return result;
	
}
AnyValue including(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue result;
	for(size_t i=0;i<c.size();i++)
	result.push_back(c[i]);
	result.push_back(parameters[0]);
	if (c.type==CollectionAnyValue::Sequence)
	{
		stable_sort(result.begin(),result.end());
	}
	else 
	if (c.type==CollectionAnyValue::Set )
	{
		stable_sort(result.begin(),result.end());
		result.erase(unique(result.begin(),result.end()),result.end());
	}
	result.type=c.type;
	return result;	
}
AnyValue excluding(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue result;
	for(size_t i=0;i<c.size();i++)
		if (c[i]!=parameters[0])
			result.push_back(c[i]);
	result.type=c.type;
	return result;	
}
AnyValue n(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue c2=parameters[0];//TODO verif Collec sinon erreur
	CollectionAnyValue result;
	for(size_t i=0;i<c.size();i++)
	{
		bool found=false;
		for(size_t i2=c2.size();i2-- && !found;)
			if (c[i]==c2[i2])found=true;
		if (!found) result.push_back(c[i]);
	}
	result.type=c.type;
	return result;
}
AnyValue symmetricDifference(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue c2=parameters[0];//TODO verif Collec sinon erreur
	CollectionAnyValue result;
	for(size_t i=0;i<c.size();i++)
	{
		bool found=false;
		for(size_t i2=c2.size();i2-- && !found;)
			if (c[i]==c2[i2])found=true;
		if (!found) result.push_back(c[i]);
	}

	for(size_t i2=0;i2<c2.size();i2++)
	{
		bool found=false;
		for(size_t i=c.size();i-- && !found;)
			if (c[i]==c2[i2])found=true;
		if (!found) result.push_back(c2[i2]);
	}
	result.type=c.type;
	return result;
	
}
AnyValue flatten(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	CollectionAnyValue result;
	for(size_t i=0;i<c.size();i++)
	{
		if (c[i].type()==typeid(CollectionAnyValue))
		{
			CollectionAnyValue c2=c[i];
			for(size_t i2=0;i<c2.size();i2++)
				result.push_back(c2[i2]);
		}
		else result.push_back(c[i]);
	}
	if (c.type==CollectionAnyValue::Sequence)
	{
		stable_sort(result.begin(),result.end());
	}
	else if (c.type==CollectionAnyValue::Set )
	{
		stable_sort(result.begin(),result.end());
		result.erase(unique(result.begin(),result.end()),result.end());
	}
	result.type=c.type;
	return result;
	
}
 AnyValue asOrderedSet(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	CollectionAnyValue result;
	if (c.type==CollectionAnyValue::Set) return c;
	else
	{
		result=c;
		stable_sort(result.begin(),result.end());
		result.erase(unique(result.begin(),result.end()),result.end());
		result.type=CollectionAnyValue::Set;
		return result;
	}
	
}
 AnyValue asSequence(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	CollectionAnyValue result=c;
	stable_sort(result.begin(),result.end());
	result.type=CollectionAnyValue::Sequence;
	return result;
}
AnyValue asBag(const CollectionAnyValue& c, const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	CollectionAnyValue result=c;
	stable_sort(result.begin(),result.end());
	result.type=CollectionAnyValue::Sequence;
	return result;
}
//OrederedSet
AnyValue append(const CollectionAnyValue& c,  const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue result=c;
	result.push_back(parameters[0]);
	if (c.type==CollectionAnyValue::Sequence)
	{
		stable_sort(result.begin(),result.end());
	}
	else if (c.type==CollectionAnyValue::Set )
	{
		stable_sort(result.begin(),result.end());
		result.erase(unique(result.begin(),result.end()),result.end());
	}
	return result;
}
AnyValue  preprend(const CollectionAnyValue& c,  const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	CollectionAnyValue result=c;
	result.insert(result.begin(),parameters[0]);
	if (c.type==CollectionAnyValue::Sequence)
		stable_sort(result.begin(),result.end());
	else if (c.type==CollectionAnyValue::Set )
	{
		stable_sort(result.begin(),result.end());
		result.erase(unique(result.begin(),result.end()),result.end());
	}
	return result;
}
AnyValue  insertAt(const CollectionAnyValue& c,  const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==2);
	int index=parameters[0];
	CollectionAnyValue result=c;
	result.insert(result.begin()+index,parameters[1]);
	if (c.type==CollectionAnyValue::Sequence)
		stable_sort(result.begin(),result.end());
	else if (c.type==CollectionAnyValue::Set )
	{
		stable_sort(result.begin(),result.end());
		result.erase(unique(result.begin(),result.end()),result.end());
	}
	return result;
	
}
AnyValue subOrderedSet(const CollectionAnyValue& c,  const CollectionAnyValue& parameters)
{
	
	assertParameters(parameters.size()==2);
	int index=parameters[0];
	int index2=parameters[1];
	CollectionAnyValue result;
	result.assign(c.begin()+index,c.begin()+index2);
	result.type=c.type;
	return result;
}
AnyValue at(const CollectionAnyValue& c,  const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	return c[(int)parameters[0]]; //TODO peut planter
}
AnyValue indexOf(const CollectionAnyValue& c,  const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);
	int i;
	for(i=c.size();i--;)
		if (c[i]==parameters[0])break;
	return i;
}
AnyValue first(const CollectionAnyValue& c,  const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	return c.front();
}
AnyValue last(const CollectionAnyValue& c,  const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==0);
	return c.back();
}

void exists(const AnyValue& res,Context& ctx,const AnyValue& v)
{
	if (ctx.find("__Result__")==ctx.end())
		ctx["__Result__"]=false;
	ctx["__Result__"]=((bool)(ctx["__Result__"]) || (bool)(res)); 
	cerr<<"R:"<<ctx["__Result__"]<<endl;
	cerr<<"v:"<<v<<endl;
}
void forAll(const AnyValue& res,Context& ctx,const AnyValue& )
{
	if (ctx.find("__Result__")==ctx.end())
		ctx["__Result__"]=true;
	ctx["__Result__"]=((bool)(ctx["__Result__"]) && (bool)(res)); 
}
void isUnique(const AnyValue& res,Context& ctx,const AnyValue& )
{ 
	if (ctx.find("__Result__")==ctx.end())
	{
		ctx["__Result__"]=true;
		ctx["__Results__"]=CollectionAnyValue();
	}
	CollectionAnyValue vec=
	ctx["__Results__"];
	for (size_t i=vec.size();i--;)
		if (vec[i]==res)
		{
			ctx["__Result__"]=false;break;
		}
	vec.push_back(res);
	ctx["__Results__"]=vec; 
}
void any(const AnyValue& res,Context& ctx,const AnyValue& v)
{
	if (ctx.find("__Result__")==ctx.end())
		ctx["__Result__"]=AnyValue();
	if ((bool)res)
		ctx["__Result__"]=v; 
}



void one(const AnyValue& res,Context& ctx,const AnyValue& )
{
	if (ctx.find("__Result__")==ctx.end())
	{
		ctx["__Result__"]=false;
		ctx["__count__"]=0;
	}
	if ((bool)res)
	{
		bool b=ctx["__count__"];
		b=ctx["__count__"]+1;
		ctx["__count__"]=b;
		ctx["__Result__"]=(b==1); 
	}
	
}
void collect(const AnyValue& res,Context& ctx,const AnyValue& )
{
	if (ctx.find("__Result__")==ctx.end())
		ctx["__Result__"]=CollectionAnyValue();
	
	CollectionAnyValue vec=
	ctx["__Result__"];
	vec.push_back(res);
	ctx["__Result__"]=vec; 
	
	
}
void selectFunction(const AnyValue& res,Context& ctx,const AnyValue& v)
{
	if (ctx.find("__Result__")==ctx.end())
	{
		ctx["__Result__"]=CollectionAnyValue();
	}
	CollectionAnyValue vec=
	ctx["__Result__"];
	if (res) vec.push_back(v);
	ctx["__Result__"]=vec;
}
void reject(const AnyValue& res,Context& ctx,const AnyValue& v)
{
	if (ctx.find("__Result__")==ctx.end())
	{
		ctx["__Result__"]=CollectionAnyValue();
	}
	CollectionAnyValue vec=
	ctx["__Result__"];
	if (!(res)) vec.push_back(v);
	ctx["__Result__"]=vec;
}
void sortedBy(const AnyValue& res,Context& ctx,const AnyValue& v)
{
	if (ctx.find("__Result__")==ctx.end())
	{
		ctx["__Result__"]=CollectionAnyValue();
		ctx["__Results__"]=CollectionAnyValue();
	}
	CollectionAnyValue vec=
	ctx["__Results__"];
	CollectionAnyValue vec2=
	ctx["__Result__"];
	unsigned i=0;
	while(i<vec.size())
	{
		if (res<vec[i]) break;
		i++;
	}
	vec.insert(vec.begin()+i,res);
	vec2.insert(vec2.begin()+i,v);
	ctx["__Results__"]=vec;
	ctx["__Result__"]=vec2;
}
void iterate(const AnyValue& res,Context& ctx,const AnyValue& v)
{
	//TODO
}




typedef function<AnyValue (const CollectionAnyValue&, const CollectionAnyValue&)> callMethodType;
map<string,callMethodType> _mapCollectionMethod;
typedef function<void (const AnyValue&, Context&,const AnyValue&)> callIterMethodType;
map<string,callIterMethodType> _mapCollectionIterMethod;
	
void addMethods(){
_mapCollectionMethod.insert(make_pair<string,callMethodType>("isEmpty",&isEmpty)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("size",&size)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("notEmpty",&notEmpty)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("sum",&sum)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("includes",&includes)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("excludes",&excludes)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("count",&count)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("includesAll",&includesAll)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("excludesAll",&excludesAll)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("product",&product)); 
//set
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("union",&unionFunction)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("intersection",&intersection)); 
_mapCollectionMethod.insert(make_pair<string,callMethodType>("including",&including)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("excluding",&excluding)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("n",&n)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("symmetricDifference",&symmetricDifference)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("flatten",&flatten));  
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("asOrderedSet",&asOrderedSet)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("asSequence",&asSequence)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("asBag",&asBag));
//OrederedSet

 _mapCollectionMethod.insert(make_pair<string,callMethodType>("append",&append)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("preprend",&preprend)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("insertAt",&insertAt)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("subOrderedSet",&subOrderedSet)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("at",&at)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("indexOf",&indexOf)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("first",&first)); 
 _mapCollectionMethod.insert(make_pair<string,callMethodType>("last",&last));
//Bag : = TODO,  union, intersection, including,excluding,coutnt, flatten,...

_mapCollectionMethod.insert(make_pair<string,callMethodType>("subSequence",&subOrderedSet));

_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("exists",&exists));
_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("forAll",&forAll));
_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("isUnique",&isUnique));
_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("any",&any));
_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("one",&one));
_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("collect",&collect));
_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("select",&selectFunction));
_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("reject",&reject));
_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("sortedBy",&sortedBy));
_mapCollectionIterMethod.insert(make_pair<string,callIterMethodType>("iterate",&iterate));
}
	AnyValue callCollectionMethod(const string& name, const CollectionAnyValue& collection, const CollectionAnyValue& parameters)
	{
		cerr<<"calling "<<name<<"with "<<parameters<<endl;
		if (_mapCollectionMethod.empty()) addMethods();
		return _mapCollectionMethod[name](collection,parameters);
	}
	void _callIterMethod(const string& name,const AnyValue& res, Context& ctx,const AnyValue& v)
	{
		if (_mapCollectionIterMethod.empty()) addMethods();
		return _mapCollectionIterMethod[name](res,ctx,v);
	}
	void _iterate(const string& name, const CollectionAnyValue& c, const vector<string>& iterators, int iteratorIndex,  iter_t expression,Context& ctx,Context & resultCtx)
	{
		if (iteratorIndex<(int)iterators.size())
		{
			for(size_t i=0;i<c.size();i++)
			{
			ctx[iterators[iteratorIndex]]=c[i];
			cerr<<"it:"<<iteratorIndex<<":"<<iterators[iteratorIndex]<<"="<<c[i]<<"("<<i<<endl;
			 _iterate(name,c,iterators,iteratorIndex+1,expression,ctx,resultCtx);
			}
		}
		else
		{
			_callIterMethod(name,evaluate(expression,ctx),resultCtx,ctx[iterators[0]]);
		}
		
	}
	
	AnyValue callCollectionIterMethod(const string& name, const CollectionAnyValue& c, const vector<string>& iterators, iter_t expression,const Context& ctx)
	{
		Context newCtx=ctx;
		Context resultCtx;
		for(size_t i=0;i<iterators.size();i++)cerr<<"it:"<<i<<" "<< iterators[i]<<endl;
		cerr<<"c:"<<c<<endl;
		_iterate(name,c,iterators,0,expression,newCtx,resultCtx);
		return resultCtx["__Result__"]; 
//		forAll	
	}
}
using _collectionMethods::callCollectionMethod;
using _collectionMethods::callCollectionIterMethod;
