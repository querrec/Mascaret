#define BOOST_SPIRIT_DUMP_PARSETREE_AS_XML

#include <boost/spirit/include/classic_spirit.hpp>
#include <boost/spirit/include/classic_parser.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>
#include <boost/spirit/include/classic_rule_parser.hpp>
#include <boost/spirit/include/classic_ast.hpp>

//TEST
#include <boost/any.hpp>

#include <iostream>
#include <stack>
#include <functional>
#ifndef _WIN32
#include <ext/functional>
#endif
#include <string>
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/Slot.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Kernel/AnyValueSpecification.h"

#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/SoundSpecification.h"
#include "VEHA/Entity/PointSpecification.h"
#include "VEHA/Entity/PathSpecification.h"
#include "VEHA/Entity/AnimationSpecification.h"

#include <boost/lambda/algorithm.hpp>
#include <algorithm>
#include "Tools/utils.h"
//using boost::_1; 

#ifdef BOOST_SPIRIT_DUMP_PARSETREE_AS_XML
#include <boost/spirit/include/classic_tree_to_xml.hpp>
#include <map>
#endif
#include "VEHA/Kernel/expressionParser.h"
#include "VEHA/Kernel/CollectionMethods.h"
#include "VEHA/Kernel/OCLSpecialMethods.h"
#include "VEHA/Kernel/Model.h"
#include "MascaretApplication.h"

#ifdef _WIN32
#  define IS_TOKEN(a,b) a.id().to_long()-3==parser_id(&b).to_long()
#else
#  define IS_TOKEN(a,b) a.id()==parser_id(&b)
#endif

namespace VEHA { namespace OclParser {

using namespace boost::spirit::classic; 
using std::unary_function;
using namespace VEHA;

typedef AnyValue data_t; 
typedef node_val_data_factory<data_t> factory_t;
typedef char const*         iterator_t;
typedef tree_match<iterator_t,factory_t> parse_tree_match_t;
typedef parse_tree_match_t::const_tree_iterator iter_t;
typedef ast_match_policy<iterator_t,factory_t> match_policy_t;
typedef scanner_policies<boost::spirit::classic::skip_parser_iteration_policy<boost::spirit::classic::space_parser>, match_policy_t, action_policy> scanner_policy_t;
typedef scanner<iterator_t, scanner_policy_t> scanner_t;
typedef rule<scanner_t> rule_t;
typedef tree_node<node_val_data<const char *,data_t > > tree_node_t ;
#define str_token(v)  str_p(v) //token_node_d[str_p(v)]

typedef map<string,AnyValue> Context;
AnyValue evaluate(iter_t const& it , Context const& c);

AnyValue vector2OneOrCollection(const vector<shared_ptr<ValueSpecification> >& vec)
{
	if (vec.size()==1) return toAnyValue(vec[0]);
	CollectionAnyValue avec;
	for(size_t i=0;i<vec.size();i++)
	{
		avec.push_back(toAnyValue(vec[0]));
	}
	return avec;
}

struct assign_int
{
	void operator()(tree_node_t& n, iterator_t b,iterator_t e) const
	{
		n.value.value(lexical_cast<int>(string(b,e)));
	}
};
struct assign_real
{
	void operator()(tree_node_t& n, iterator_t b,iterator_t e) const
	{
		n.value.value(lexical_cast<double>(string(b,e)));
	}
};
struct assign_string
{
	void operator()(tree_node_t& n, iterator_t b,iterator_t e) const
	{
		n.value.value(string(b+1,e-1));
	}
};

rule_t oclExpressions,constraint,contextDeclaration,classifierContext,operationContext,stereotype,operationName,formalParameterList,typeSpecifier,collectionType,oclExpression,returnType,expression,letExpression,ifExpression,logicalExpression,relationExpression,additiveExpression,multiplicativeExpression,unaryExpression,postfixExpression,primaryExpression,propertyCallParameters,simpleTypeSpecifier,literalCollection,collectionItem,propertyCall,qualifiers,pathName,timeExpression,actualParameterList,logicalOperator,collectionKind,relationalOperator,addOperator,multiplyOperator,unaryOperator;
rule_t formalParameter,constraintDefinition,propertyCallArrowParameters,operationCall,attributeCall;
rule_t specialOperationCall, spatialOperationCallWithFoR, spatialOperationCallWithoutFoR; //@XOCL
rule_t collectionCall,iteratorCallParameters,iteratorCallName,collectionMethodName;
rule_t oclFile,packageName;
rule_t LPAREN,RPAREN,LBRACK,RBRACK,LCURLY,RCURLY,COLON,DCOLON,COMMA,EQUAL,NEQUAL,LT,GT,LE,GE,RARROW,DOTDOT,DOT,POUND,SEMICOL,BAR,ATSIGN,PLUS,MINUS,MULT,DIVIDE, NAME , STRING, NUMBER;

void initGrammar()
{
oclFile = +(no_node_d[str_token("package")]>> root_node_d[packageName]>> oclExpressions >>no_node_d[str_token("endpackage")] );
packageName=pathName;

oclExpressions= *(constraint);
constraint=contextDeclaration>> 
	(+(constraintDefinition  | (stereotype >> !(NAME)>> COLON>> oclExpression) ))
	;
constraintDefinition=str_token("def") >>NAME >> !(formalParameterList) >>  !(COLON >> typeSpecifier)  >>EQUAL >> expression;
contextDeclaration=
	no_node_d[str_token("context")] >> (operationContext | classifierContext);
classifierContext =
	(NAME >> COLON >> NAME)
	| NAME
	;
operationContext= 
	NAME >> DCOLON >>  operationName >> LPAREN >> formalParameterList >> RPAREN
	>> !(COLON >> returnType) ;

stereotype= str_token("pre")| str_token("post") | str_token("inv");

operationName=NAME | EQUAL | NEQUAL| PLUS | MINUS | GE | LT | LE |  GT | DIVIDE | MULT | 
str_token("implies") | str_token("not") | str_token("or") | str_token("xor") | str_token("and"); 
formalParameterList= !(infix_node_d[formalParameter >> *(COMMA >> formalParameter)]);
formalParameter =NAME >> COLON >> typeSpecifier;

typeSpecifier= simpleTypeSpecifier | collectionType;

collectionType=collectionKind >> LPAREN>> simpleTypeSpecifier >>RPAREN;

oclExpression = !(*(letExpression ) >> no_node_d[str_token("in")] )>>  expression; //

returnType=typeSpecifier;


expression= logicalExpression; //
letExpression = no_node_d[str_token("let")] >> NAME >>  !(COLON >> typeSpecifier) >>EQUAL>> expression; // @XOCL: TODO: prob with let..in let...in...
ifExpression= discard_node_d[str_token("if")] >>expression >> discard_node_d[str_token("then")] >> expression >> discard_node_d[str_token("else")]>> expression >> no_node_d[str_token("endif")]; //
logicalExpression=
	relationExpression >> *(root_node_d[logicalOperator] >> relationExpression);//
relationExpression= additiveExpression >> !(root_node_d[relationalOperator] >> additiveExpression);//
additiveExpression=multiplicativeExpression >> *(root_node_d[addOperator] >> multiplicativeExpression);//

multiplicativeExpression= unaryExpression >> *(root_node_d[multiplyOperator] >> unaryExpression);//

unaryExpression = (unaryOperator>> postfixExpression)
	| postfixExpression;

postfixExpression = primaryExpression >> *( (root_node_d[DOT] >> propertyCall) //environ
	| (root_node_d[RARROW] >> collectionCall  ) );
collectionCall = (iteratorCallName>>  iteratorCallParameters) | (collectionMethodName >> propertyCallParameters);
iteratorCallName=str_token("exists") | str_token("forAll") | str_token("isUnique") | str_token("any") | str_token("one") | str_token("collect")
	| str_token("select") | str_token("reject") | str_token("sortBy") | str_token("iterate");
iteratorCallParameters=
	 (LPAREN >> NAME >> *(COMMA >> NAME) >>
		!(COLON >> simpleTypeSpecifier) >>
		!(SEMICOL >> NAME >> COLON >> typeSpecifier >> EQUAL >> expression) >>  //accumulator for iterate
		BAR >> oclExpression >> discard_node_d[RPAREN] ) // ?
	| (LPAREN >> oclExpression >> discard_node_d[RPAREN]);
collectionMethodName=str_token("isEmpty") | str_token("size") | str_token("notEmpty") | str_token("sum") | str_token("includes") | str_token("excludes") | str_token("count") | str_token("includesAll") | str_token("excludesAll") | str_token("product") 
//set
| str_token("union") | str_token("intersection") |str_token("including") | str_token("excluding") | str_token("n") | str_token("symmetricDifference") | str_token("flatten")  | str_token("asOrderedSet") | str_token("asSequence") | str_token("asBag")
//OrederedSet
| str_token("append") | str_token("preprend") | str_token("insertAt") | str_token("subOrderedSet") | str_token("at") | str_token("indexOf") | str_token("first") | str_token("last")
|str_token("subSequence");
//
primaryExpression = literalCollection  |ifExpression
	|  (STRING  | NUMBER |  leaf_node_d[(NAME  >> DCOLON >> NAME >> *(DCOLON >> NAME) )] ) //literal
	| propertyCall | (inner_node_d[ LPAREN >> expression >> RPAREN]); //@XOCL

propertyCallParameters=
	 LPAREN >> !( expression >> *(discard_node_d[COMMA] >> expression)) >> RPAREN;
	

simpleTypeSpecifier = pathName;
literalCollection = collectionKind >> discard_node_d[LCURLY] >> !(collectionItem >> *(discard_node_d[COMMA] >> collectionItem) )>> discard_node_d[RCURLY] ; //environ

collectionItem = expression >> !(DOTDOT >> expression);

propertyCall = specialOperationCall
	| spatialOperationCallWithFoR | spatialOperationCallWithoutFoR //@XOCL:
 	| operationCall | attributeCall;


specialOperationCall = (
	/**@OCL default**/
	str_token("oclName")	/*| str_token("in")*/ | str_token("oclIsTypeOf") | str_token("oclIsKindOf") | str_token("oclIsInState") | str_token("oclAsType") | str_token("allInstances") 
)
>> propertyCallParameters;


spatialOperationCallWithoutFoR = (
	/**@XOCL distance OperationCall**/
	str_token("vehaDistance") | /* for compatibility with previous implementation */
	str_token("ray") | str_token("isInRay") | str_token("distance") | str_token("radar") | str_token("isInRadar") 
	/**@XOCL space OperationCall**/
	| str_token("dimensions") | str_token("isInDimensions") 		
	/**@XOCL position OperationCall**/			
	| str_token("position")	 		
	/**@XOCL directional OperationCall**/
	| str_token("orientation") 			
	/**@XOCL topological OperationCall**/
	| str_token("disjoint") | str_token("meet") | str_token("equal") | str_token("inside") | str_token("coveredBy") | str_token("contains") 
	| str_token("covers") | str_token("overlap")	
	/**@XOCL projective OperationCall**/
	| str_token("asideOnPlane") | str_token("betweenOnPlane") | str_token("nonbetweenOnPlane") | str_token("beforeOnPlane")	| str_token("afterOnPlane") 
			
	| str_token("collinear") | str_token("noncollinear") | str_token("between")| str_token("nonbetween")
	| str_token("before")| str_token("after") |str_token("aside")
	| str_token("outside")

	| str_token("coplanar") | str_token("noncoplanar") | str_token("internal") | str_token("external")| str_token("above")| str_token("below")
	)
	>> propertyCallParameters;

spatialOperationCallWithFoR=(
	/**@XOCL projective OperationCall**/
 	str_token("leftsideOnPlane") | str_token("rightsideOnPlane")

	/**@XOCL directional OperationCall**/
 	| str_token("leftOf") | str_token("rightOf")
 	| str_token("frontOf") | str_token("behindOf")
 	| str_token("aboveOf") | str_token("belowOf")	
) 
>> propertyCallParameters >> !(str_token("@viewpoint")) >> !(propertyCallParameters); //@XOCL:
//TODO: changed @viewpoint to an option 
//viewpointExp = str_token("@viewpoint") >> propertyCallParameters; //@XOCL:

operationCall= NAME >>propertyCallParameters;
attributeCall= NAME >> !(qualifiers);

qualifiers = LBRACK >>  expression >> *(discard_node_d[COMMA] >> expression)  >> RBRACK;

//declarator 


pathName=NAME >> *( DCOLON >> NAME );
timeExpression= ATSIGN >> str_token("pre");
//
actualParameterList = expression >> *(discard_node_d[COMMA] >> expression); //


logicalOperator = str_token("and") | str_token("or") | str_token("xor") | str_token("implies");//

collectionKind =str_token("Set") | str_token("Bag") | str_token("Sequence") | str_token("Collection") | str_token("OrderedSet"); 

relationalOperator=EQUAL |NEQUAL |  GE | LE | GT | LT ;//


addOperator = PLUS | MINUS;//
multiplyOperator = MULT | DIVIDE;//
unaryOperator=MINUS | str_token("not");

LPAREN			= ch_p('(');
RPAREN			= ch_p(')');
LBRACK			= ch_p('[');
RBRACK			= ch_p(']');
LCURLY			= ch_p('{');
RCURLY			= ch_p('}');
COLON			= ch_p(':');
DCOLON			= str_token("::");
COMMA			= ch_p(',');

EQUAL			= ch_p('=');
NEQUAL			= str_token("<>");
LT			= ch_p('<');
GT			= ch_p('>');
LE			= str_token("<=");
GE			= str_token(">=");
RARROW			= str_token("->");
DOTDOT			= str_token("..");
DOT			= ch_p('.');
POUND			= ch_p('#');
SEMICOL			= ch_p(';');
BAR			= ch_p('|');
ATSIGN			= ch_p('@');
PLUS			= ch_p('+');
MINUS			= ch_p('-');
MULT			= ch_p('*');
DIVIDE			= ch_p('/');


NAME = no_node_d[*(str_token(" "))] >> leaf_node_d[lexeme_d[chset<>("a-zA-Z_")>> *(chset<>("a-zA-Z0-9_"))]] >> no_node_d[*(str_token(" "))];
//NAME =  leaf_node_d[lexeme_d[chset<>("a-zA-Z_")>> *(chset<>("a-zA-Z0-9_"))]];
//NAME = leaf_node_d[token_node_d[lexeme_d[chset<>("a-zA-Z_")>> *(chset<>("a-zA-Z0-9_"))]]];

STRING = access_node_d[leaf_node_d[lexeme_d[ch_p('\'') >> *(~(chset<>("'\n\r") )) >> ch_p('\'')]]][assign_string()];
NUMBER = access_node_d[real_p][assign_real()]| access_node_d[int_p][assign_int()];



//DEBUGGING:
BOOST_SPIRIT_DEBUG_RULE(oclExpressions);
BOOST_SPIRIT_DEBUG_RULE(constraint);
BOOST_SPIRIT_DEBUG_RULE(contextDeclaration);
BOOST_SPIRIT_DEBUG_RULE(classifierContext);
BOOST_SPIRIT_DEBUG_RULE(operationContext);
BOOST_SPIRIT_DEBUG_RULE(stereotype);
BOOST_SPIRIT_DEBUG_RULE(operationName);
BOOST_SPIRIT_DEBUG_RULE(formalParameterList);
BOOST_SPIRIT_DEBUG_RULE(typeSpecifier);
BOOST_SPIRIT_DEBUG_RULE(collectionType);
BOOST_SPIRIT_DEBUG_RULE(oclExpression);
BOOST_SPIRIT_DEBUG_RULE(returnType);
BOOST_SPIRIT_DEBUG_RULE(expression);
BOOST_SPIRIT_DEBUG_RULE(letExpression);
BOOST_SPIRIT_DEBUG_RULE(ifExpression);
BOOST_SPIRIT_DEBUG_RULE(logicalExpression);
BOOST_SPIRIT_DEBUG_RULE(relationExpression);
BOOST_SPIRIT_DEBUG_RULE(additiveExpression);
BOOST_SPIRIT_DEBUG_RULE(multiplicativeExpression);
BOOST_SPIRIT_DEBUG_RULE(unaryExpression);
BOOST_SPIRIT_DEBUG_RULE(postfixExpression);
BOOST_SPIRIT_DEBUG_RULE(primaryExpression);
BOOST_SPIRIT_DEBUG_RULE(propertyCallParameters);
BOOST_SPIRIT_DEBUG_RULE(simpleTypeSpecifier);
BOOST_SPIRIT_DEBUG_RULE(literalCollection);
BOOST_SPIRIT_DEBUG_RULE(collectionItem);
BOOST_SPIRIT_DEBUG_RULE(propertyCall);
BOOST_SPIRIT_DEBUG_RULE(qualifiers);
BOOST_SPIRIT_DEBUG_RULE(pathName);
BOOST_SPIRIT_DEBUG_RULE(timeExpression);
BOOST_SPIRIT_DEBUG_RULE(actualParameterList);
BOOST_SPIRIT_DEBUG_RULE(logicalOperator);
BOOST_SPIRIT_DEBUG_RULE(collectionKind);
BOOST_SPIRIT_DEBUG_RULE(relationalOperator);
BOOST_SPIRIT_DEBUG_RULE(addOperator);
BOOST_SPIRIT_DEBUG_RULE(multiplyOperator);
BOOST_SPIRIT_DEBUG_RULE(unaryOperator);
BOOST_SPIRIT_DEBUG_RULE(oclFile);
BOOST_SPIRIT_DEBUG_RULE(packageName);
BOOST_SPIRIT_DEBUG_RULE( LPAREN);
BOOST_SPIRIT_DEBUG_RULE(RPAREN);
BOOST_SPIRIT_DEBUG_RULE(LBRACK);
BOOST_SPIRIT_DEBUG_RULE(RBRACK);
BOOST_SPIRIT_DEBUG_RULE(LCURLY);
BOOST_SPIRIT_DEBUG_RULE(RCURLY);
BOOST_SPIRIT_DEBUG_RULE(COLON);
BOOST_SPIRIT_DEBUG_RULE(DCOLON);
BOOST_SPIRIT_DEBUG_RULE(COMMA);
BOOST_SPIRIT_DEBUG_RULE(EQUAL);
BOOST_SPIRIT_DEBUG_RULE(NEQUAL);
BOOST_SPIRIT_DEBUG_RULE(LT);
BOOST_SPIRIT_DEBUG_RULE(GT);
BOOST_SPIRIT_DEBUG_RULE(LE);
BOOST_SPIRIT_DEBUG_RULE(GE);
BOOST_SPIRIT_DEBUG_RULE(RARROW);
BOOST_SPIRIT_DEBUG_RULE(DOTDOT);
BOOST_SPIRIT_DEBUG_RULE(DOT);
BOOST_SPIRIT_DEBUG_RULE(POUND);
BOOST_SPIRIT_DEBUG_RULE(SEMICOL);
BOOST_SPIRIT_DEBUG_RULE(BAR);
BOOST_SPIRIT_DEBUG_RULE(ATSIGN);
BOOST_SPIRIT_DEBUG_RULE(PLUS);
BOOST_SPIRIT_DEBUG_RULE(MINUS);
BOOST_SPIRIT_DEBUG_RULE(MULT);
BOOST_SPIRIT_DEBUG_RULE(DIVIDE);
BOOST_SPIRIT_DEBUG_RULE( NAME );
BOOST_SPIRIT_DEBUG_RULE( STRING);
BOOST_SPIRIT_DEBUG_RULE(NUMBER);
}

struct getSecond : public unary_function<pair<string,shared_ptr<ValueSpecification> > , AnyValue >
{
	AnyValue operator ()(const pair<string,shared_ptr<ValueSpecification> >& p  ) const
		{return toAnyValue(p.second);}
};

AnyValue evaluate(iter_t const& it , Context const& c)
{
	if (IS_TOKEN(it->value,PLUS))
	{
		//cerr<<"test in add:"<<*it->value.begin()<<endl;
		return 	evaluate(it->children.begin(),c)+evaluate(it->children.begin()+1,c);
	}
	else if (IS_TOKEN(it->value,MINUS))
		return 	evaluate(it->children.begin(),c)-evaluate(it->children.begin()+1,c);
	else if (IS_TOKEN(it->value,MULT))
		return 	evaluate(it->children.begin(),c)*evaluate(it->children.begin()+1,c);
	else if (IS_TOKEN(it->value,DIVIDE))
		return 	evaluate(it->children.begin(),c)/evaluate(it->children.begin()+1,c);
	else if (IS_TOKEN(it->value,EQUAL))
		return evaluate(it->children.begin(),c)== evaluate(it->children.begin()+1,c);
	else if (IS_TOKEN(it->value,NEQUAL))
		return evaluate(it->children.begin(),c)!= evaluate(it->children.begin()+1,c);
	else if (IS_TOKEN(it->value,LT))
		return evaluate(it->children.begin(),c)< evaluate(it->children.begin()+1,c);
	else if (IS_TOKEN(it->value,GT))
		return evaluate(it->children.begin(),c)> evaluate(it->children.begin()+1,c);	
	else if (IS_TOKEN(it->value,LE)	)
		return evaluate(it->children.begin(),c)<= evaluate(it->children.begin()+1,c);	
	else if (IS_TOKEN(it->value,GE))
		return evaluate(it->children.begin(),c)>= evaluate(it->children.begin()+1,c);	
	else if (IS_TOKEN(it->value,logicalOperator))
	{
		if (string(it->value.begin(),it->value.end())=="and")
		return  ((bool)evaluate(it->children.begin(),c) && (bool)evaluate(it->children.begin()+1,c));
		else if (string(it->value.begin(),it->value.end())=="implies")
		{
			bool a,b;
			a=evaluate(it->children.begin(),c);
			b=evaluate(it->children.begin()+1,c);
			return ((!(bool)evaluate(it->children.begin(),c))||((bool)evaluate(it->children.begin()+1,c)));
		}
		else if (string(it->value.begin(),it->value.end())=="or")
			return  ((bool)evaluate(it->children.begin(),c) || (bool)evaluate(it->children.begin()+1,c));
		else if (string(it->value.begin(),it->value.end())=="xor")
		{
			bool a,b;
			a=evaluate(it->children.begin(),c);
			b=evaluate(it->children.begin()+1,c);
			return (a&&!b)||(!a&&b);
		}
	}
	else if (IS_TOKEN(it->value,unaryExpression))
	{
		if (IS_TOKEN(it->children.begin()->value,MINUS))
		{
			AnyValue a=evaluate(it->children.begin()+1,c);
			AnyValue b=a;
			b.replaceValueFromString("0");
			return b-a;
		}
		else 
			return !(bool)evaluate(it->children.begin()+1,c);
	}
	else if(IS_TOKEN(it->value,ifExpression))
	{
		if ((bool)evaluate(it->children.begin(),c))
			return evaluate(it->children.begin()+1,c);
		else	return evaluate(it->children.begin()+2,c);
	}
	else if(IS_TOKEN(it->value,oclExpression) )//let expressions
	{
		//cerr<<"oclExpr"<<endl;
		iter_t child=it->children.begin();
		Context newC=c;
		while(child!=it->children.end()-1)
		{
			//child: letExpresssion
			AnyValue exp=evaluate(child->children.end()-1,newC);
			//cerr << "exp= " << exp << endl;
			//TODO gestion du typeSpecifier;
			iter_t itName=child->children.begin();
			newC[string(itName->value.begin(),itName->value.end()) ]=exp;
			//cerr << "newC= " << string(itName->value.begin(),itName->value.end()) << endl;
			child++;
		}
		return evaluate(child,newC);
	}
	else if(IS_TOKEN(it->value,constraintDefinition))
	{
		
		
	}
	/*else if(IS_TOKEN(it->value,spatialOperationCallWithFoR))	//@XOCL
	{
		
			//cerr << "spatialOperationCallWithFoR1"<<endl;
			iter_t nameChild=it->children.begin();
			string name=string(nameChild->value.begin(),nameChild->value.end()); //name
			InstanceSpecification::OperationCallParameters parameters;
			CollectionAnyValue params=evaluate(it->children.begin()+1,c); //param1 = arguments
			//operation->children.begin()+2 = //param2 = @viewpoint
			CollectionAnyValue params3=evaluate(it->children.begin()+3,c);//param2 = observer (FoR)
			params.push_back(params3[0]);
			if (c.find("__self__")!=c.end())
				params.push_back( MAP_AT(c,string("__self__")));
			else params.push_back( MAP_AT(c,string("self")) );

			return specialOpCall(name,params);
	}
	else if (IS_TOKEN(it->value,spatialOperationCallWithoutFoR))
	{
			//cerr << "spatialOperationCallWithoutFoR1"<<endl;
			iter_t nameChild=it->children.begin();
			string name=string(nameChild->value.begin(),nameChild->value.end()); //name
			InstanceSpecification::OperationCallParameters parameters;
						
			CollectionAnyValue params=evaluate(it->children.begin()+1,c);
			if (c.find("__self__")!=c.end())
				params.push_back( MAP_AT(c,string("__self__")));
			else params.push_back( MAP_AT(c,string("self")) );
			return specialOpCall(name,params);
	}
*/
	else if (IS_TOKEN(it->value,specialOperationCall))
	{
		//	cerr << "specialOperationCall1"<<endl;
			iter_t nameChild=it->children.begin();
			string name=string(nameChild->value.begin(),nameChild->value.end()); //name
			InstanceSpecification::OperationCallParameters parameters;
						
			CollectionAnyValue params=evaluate(it->children.begin()+1,c);
			if (c.find("__self__")!=c.end())
				params.push_back( MAP_AT(c,string("__self__")));
			else params.push_back( MAP_AT(c,string("self")) );
			return specialOpCall(name,params);
	}
	else if (IS_TOKEN(it->value,operationCall))
	{
			iter_t nameChild=it->children.begin();
			string name=string(nameChild->value.begin(),nameChild->value.end()); //name
			InstanceSpecification::OperationCallParameters parameters;
			shared_ptr<InstanceSpecification> is;
			if (c.find("__self__")!=c.end())
				is=MAP_AT(c,string("__self__"));
			else is=MAP_AT(c,string("self"));
			CollectionAnyValue params=evaluate(it->children.begin()+1,c);
			for (size_t i=0;i<params.size();i++)
				parameters.push_back(toValueSpecification(params[i]));
			 shared_ptr<BehaviorExecution> be= is ->executeOperation(name,parameters);
			return 	vector2OneOrCollection(be->result);
			
	}
	else if(IS_TOKEN(it->value,NAME))
	{
		//cerr << "NAME" <<endl;
		Context::const_iterator ctxIt;
		string varName(it->value.begin(),it->value.end());

		//cerr << "Context of varNAME====" << varName << endl;

		Context::const_iterator it;
		for (it= c.begin(); it!= c.end(); it++) {
			//cerr << it->first << "=" << it->second << endl;
		}

		if ((ctxIt=c.find(varName))!=c.end())
                {
			return ctxIt->second;
                }
		else //search in slot
		{
			shared_ptr<InstanceSpecification> is;
			if (c.find("__self__")!=c.end())
				is=MAP_AT(c,string("__self__"));
			else is=MAP_AT(c,string("self"));
			if (is->getSlots().find(varName)!=is->getSlots().end())
			{
				map<string,shared_ptr<ValueSpecification> > values= MAP_AT(is->getSlots(),varName)->getValues();
				vector<AnyValue> valStr(values.size(),string(""));
				transform(values.begin(),values.end(),valStr.begin(),getSecond() );
				return CollectionAnyValue(valStr);
			}

		//cerr << "NAME is a Class name..." << endl;
		const map<string,vector<shared_ptr<Class> > >& mapClass = MascaretApplication::getInstance()->getModel()->getAllClasses();
		vector<shared_ptr<Class> > vecClass;
		for (map<string,vector<shared_ptr<Class> > >::const_iterator it2 = mapClass.begin();
			it2 != mapClass.end(); it2++ )
			if (it2->first == varName ) vecClass = it2->second;

		if (vecClass.size()>0 ) {
			map<string,shared_ptr<InstanceSpecification> > vecIs = vecClass[0]->getInstances();
			map<string,shared_ptr<InstanceSpecification> >::const_iterator it2;
			if (vecIs.size()>0) return AnyValue(vecIs.begin()->second); 
			}

		//return varName+"_Not_found";
		return varName;
		}
	}
	else if (IS_TOKEN(it->value,DOT))
	{

		//cerr << "DOT" << endl;
		AnyValue obj=evaluate(it->children.begin(),c);

		//obj must be an AnyValue, not a Collection
		//Sometimes, the selection by OID in VRXOCL returns a collection with only one element!
		if (obj.type() == typeid(CollectionAnyValue)) {
			CollectionAnyValue col=obj;
			obj = col[0];
		}
 
		//cerr << "OBJ====" << obj << endl;

                shared_ptr<InstanceSpecification> is =  MascaretApplication::getInstance()->getEnvironment()->getInstance((string)obj);

        if(!is)
        {
            cerr << "[OclParser] no Instance found with name " << obj << endl;
            // TODO: Correct error handling
        }

		//get Property
		map<string, shared_ptr<Property> >  mapProp = is->getClassifier()->getAttributes();
		map<string, shared_ptr<Property> >::const_iterator it2;
		//for (it2=mapProp.begin(); it2!= mapProp.end(); it2++)
			//cerr << it2->second->getFullName() << "*** Type =" << it2->second->getClassifier()->getFullName() << endl;

		//get Slots
		map<string,shared_ptr<Slot> >  mapSlot = is->getSlots();
		map<string,shared_ptr<Slot> > ::const_iterator it3;
		vector<AnyValue> val;
		for (it3=mapSlot.begin(); it3!= mapSlot.end(); it3++) {
			//cerr << "EXXXXXXXXXXXXXXXXXXXXXX" << it3->first << endl;
			map<string,shared_ptr<ValueSpecification> > values= it3->second->getValues();
			vector<AnyValue> valStr;
			for (map<string,shared_ptr<ValueSpecification> >::const_iterator it4=values.begin()
				; it4!= values.end(); it4++) {			
				//TODO : patch temporaire pour ignorer les animations, shapes et type de base non géré par l'interpreteur OCL...
				if(!shared_dynamic_cast<ShapeSpecification>(it4->second) && !shared_dynamic_cast<SoundSpecification>(it4->second)&&
						!shared_dynamic_cast<AnimationSpecification>(it4->second)&&!shared_dynamic_cast<PointSpecification>(it4->second) &&
						!shared_dynamic_cast<PathSpecification>(it4->second))
					valStr.push_back(toAnyValue(it4->second));
				//cerr << toAnyValue(it4->second) << endl;
				//cerr << it4->first << "==" <<it4->second->getStringFromValue() << "typename "<< it4->second->getTypeName() << endl;
			}
			val.insert(val.end(), valStr.begin(), valStr.end());			
//			AnyValue valStr = vector2OneOrCollection( values.value_compare);
//			val.push_back(valStr);
		}
		//cerr << "Slot=====" << CollectionAnyValue(val) << endl;

		/*if (IS_TOKEN((it->children.begin()+1)->value,spatialOperationCallWithFoR)) 	//@XOCL
		{
			//cerr << "spatialOperationCallWithFoR2" <<endl;
			iter_t operation=(it->children.begin()+1);
			iter_t nameChild=operation->children.begin();
			string name=string(nameChild->value.begin(),nameChild->value.end()); //name
			InstanceSpecification::OperationCallParameters parameters;
			CollectionAnyValue params=evaluate(operation->children.begin()+1,c); //param1 = arguments
			//operation->children.begin()+2 = //param2 = @viewpoint

			CollectionAnyValue params3;
			if ( (operation->children.begin()+2) != operation->children.end() ) {
				//cerr << "Explicit FoR!" << endl;
				params3=evaluate(operation->children.begin()+3,c);//param3 = observer (FoR)
				params.push_back(params3[0]);
			}
			else {
				//cerr << "Implicit FoR!" << endl;
				// implicit FoR
				vector<string> dirOp;
				dirOp.push_back("leftOf"); dirOp.push_back("rightOf");
				dirOp.push_back("frontOf"); dirOp.push_back("behindOf");
				dirOp.push_back("aboveOf"); dirOp.push_back("belowOf");

				if ( find(dirOp.begin(), dirOp.end(), name)== dirOp.end() ) {
					params.push_back(obj); // observer = primary object 
					//TODO: or reference object ???
					//params.push_back(operation->children.begin()+1); 
				}
			}
			params.push_back(obj);
			return specialOpCall(name,params);
		}
		else if (IS_TOKEN((it->children.begin()+1)->value,spatialOperationCallWithoutFoR))
		{
			//cerr << "spatialOperationCallWithoutFoR2"<<endl;
			iter_t operation=(it->children.begin()+1);
			iter_t nameChild=operation->children.begin();
			string name=string(nameChild->value.begin(),nameChild->value.end()); //name
			InstanceSpecification::OperationCallParameters parameters;
			CollectionAnyValue params=evaluate(operation->children.begin()+1,c);
			params.push_back(obj);
			return specialOpCall(name,params);
		}
		else */
                if (IS_TOKEN((it->children.begin()+1)->value,specialOperationCall))
		{
			cerr << "specialOperationCall2"<<endl;
			iter_t operation=(it->children.begin()+1);
			iter_t nameChild=operation->children.begin();
			string name=string(nameChild->value.begin(),nameChild->value.end()); //name
			InstanceSpecification::OperationCallParameters parameters;
			CollectionAnyValue params=evaluate(operation->children.begin()+1,c);
			params.push_back(obj);
			return specialOpCall(name,params);
		}
		else if (IS_TOKEN((it->children.begin()+1)->value,operationCall))
		{
			shared_ptr<InstanceSpecification> is=shared_ptr<InstanceSpecification>(obj);
			iter_t operation=(it->children.begin()+1);
			iter_t nameChild=operation->children.begin();
			string name=string(nameChild->value.begin(),nameChild->value.end()); //name
			InstanceSpecification::OperationCallParameters parameters;
			CollectionAnyValue params=evaluate(operation->children.begin()+1,c);
			for (size_t i=0;i<params.size();i++)
				parameters.push_back(toValueSpecification(params[i]));
			 shared_ptr<BehaviorExecution> be= is ->executeOperation(name,parameters);
			return 	vector2OneOrCollection(be->result);
			
		}
		else
		{
			shared_ptr<InstanceSpecification> is =  MascaretApplication::getInstance()->getEnvironment()->getInstance((string)obj);
		//	shared_ptr<InstanceSpecification> is=shared_ptr<InstanceSpecification>(obj);
			iter_t itSlotName=( (it->children.begin()+1) ); //->children.begin());
			bool haveQualifier=false;
			haveQualifier= (IS_TOKEN(itSlotName->value,attributeCall));
			if (haveQualifier)
				itSlotName=itSlotName->children.begin();
			string slotName=string(itSlotName->value.begin(),itSlotName->value.end());
			map<string,shared_ptr<ValueSpecification> > values;
			try
			{
				values = MAP_AT(is->getSlots(),slotName)->getValues();//else : child+1 is the attribute name -> return slot? ou return un bag ou autre?
			}
			catch (std::out_of_range& e) {
				cerr << "ERROR: OCL evaluate, slotName " << slotName << " not found on " << obj << endl;
				throw;
			}
			vector<AnyValue> valStr(values.size(),string(""));
			transform(values.begin(),values.end(),valStr.begin(),getSecond() );
			CollectionAnyValue v= CollectionAnyValue(valStr);
			if (!haveQualifier)return v;
			vector<AnyValue> av;
			iter_t it2=(itSlotName+1)->children.begin()+1;                //after [
			iter_t it_end=(itSlotName+1)->children.end()-1;       //before ]
			while (it2!=it_end)
			{
				av.push_back(evaluate(it2,c));
				++it2;
			}
			if (av.size()==0) return v;
			else if (av.size()==1) return v[(int) av[0] ];  //cas d'une sequence !! TODO (MAP) si la cle est autre
			else 
			{
				CollectionAnyValue v2;
				for (size_t i=0;i<av.size();i++)
					v2.push_back(v[(int)av[i]]);
				return v2;
			}
		}
	}
	else if (IS_TOKEN(it->value,literalCollection))
	{
		CollectionAnyValue vec;
		string type=string(it->children.begin()->value.begin(),it->children.begin()->value.end());
		iter_t it2=it->children.begin()+1;		
		iter_t it_end=it->children.end();
		while(it2!=it_end)
		{
			if (IS_TOKEN(it2->value,collectionItem))   //  x .. y
			{//on dit que ça marche que pour les int TODO (LES chars?)
				int start=evaluate(it2->children.begin(),c); 
				int end=evaluate(it2->children.end()-1,c); 
				int inc=(end-start>0?1:-1);
				for (;start!=end+inc;start+=inc)
					vec.push_back(start);
			}
			else vec.push_back(evaluate(it2,c));
			++it2;
		}
		if (type=="sequence") vec.type=CollectionAnyValue::Sequence;
		if (type=="Set" || type=="OrderedSet" || type=="Collection") vec.type=CollectionAnyValue::Set;
		if (type=="Bag") vec.type=CollectionAnyValue::Bag;
		if (vec.type==CollectionAnyValue::Set || vec.type== CollectionAnyValue::Sequence )
			stable_sort(vec.begin(),vec.end());
		if (vec.type==CollectionAnyValue::Set)
			vec.erase(unique(vec.begin(),vec.end()),vec.end());
		return vec;
	}
	else if (IS_TOKEN(it->value,attributeCall))
	{
		AnyValue a=evaluate(it->children.begin(),c);
		CollectionAnyValue v=a;
		vector<AnyValue> av;
		iter_t it2=(it->children.begin()+1)->children.begin()+1;                //after [
                iter_t it_end=(it->children.begin()+1)->children.end()-1;       //before ]
		while (it2!=it_end)
		{
			av.push_back(evaluate(it2,c));
			++it2;
		}
		if (av.size()==0) return v;
		else if (av.size()==1)
		{
		  int index=av[0];
		  return (index<(int)v.size()?v[index ]:"Invalid Index");  //cas d'une sequence !! TODO (MAP)
		}
		else 
		{
			CollectionAnyValue v2;
			for (size_t i=0;i<av.size();i++)
		  	{	int index=av[i];
				if (index<(int)v.size())
					v2.push_back(v[index]);
				else return "Invalid Index";
			}
			return v2;
		}
		
	}
	else if (IS_TOKEN(it->value,RARROW))
	{
		//cerr << "RARROW" << endl;
		CollectionAnyValue col=evaluate(it->children.begin(),c);
		//cerr << "RARROW2" << endl;
		iter_t itName= ( (it->children.begin()+1)->children.begin());
		if (IS_TOKEN(itName->value,iteratorCallName))
		{
			//cerr<<"ici"<<endl;
			vector<string> iterators;
			if (((it->children.begin()+1)->children.begin()+1)->children.begin()+2==((it->children.begin()+1)->children.begin()+1)->children.end()) // pas de |
			{
				iterators.push_back("__self__");
			}
			else {
			//cerr<<"la ou il faut"<<endl;
				iter_t itVarName=((it->children.begin()+1)->children.begin()+1)->children.begin()+1;
				iterators.push_back(string(itVarName->value.begin(),itVarName->value.end()));
				while(IS_TOKEN((itVarName+1)->value,COMMA))
				{
					itVarName+=2;
					iterators.push_back(string(itVarName->value.begin(),itVarName->value.end()));
				}
			}
			iter_t expression=((it->children.begin()+1)->children.begin()+1)->children.end()-1;
			return callCollectionIterMethod(string(itName->value.begin(),itName->value.end()),col, iterators, expression,c);
		}
		else 
		{
			//cerr<<"la"<<endl;
			iter_t itParameters=  ( (it->children.begin()+1)->children.begin()+1);
			CollectionAnyValue parameters=evaluate(itParameters,c);
			return callCollectionMethod(string(itName->value.begin(),itName->value.end()),col,parameters);
				
		}
		
	}
	else if (IS_TOKEN(it->value,propertyCallParameters))
	{
		//cerr<<"propertyCallParameteers"<<endl;
		CollectionAnyValue parameters;parameters.type=CollectionAnyValue::Bag;
		iter_t it2= it->children.begin()+1;
		for (;it2!=it->children.end()-1;it2++)
		{
			parameters.push_back(evaluate(it2,c));
		}
		//cerr<<parameters<<endl;
		return parameters;
	}
	else { //cerr << "LAST RETURN" << endl; 
		return it->value.value();} 
}

bool initialized=false;

VEHA::OclParser::ExpressionTree parseExpression(const string & str)
{
	if (!initialized)
		initGrammar();

	//TODO: IMPORTANT UPDATE of Boost!!!
        tree_parse_info<iterator_t,factory_t> info = 
	ast_parse<factory_t>(str.c_str(),str.c_str()+str.size(),  oclExpression >> !end_p ,space_p); //TODO: skip parser for comments??
	//ast_parse<factory_t>(str.c_str(),str.c_str()+str.size(),  oclExpression >> !end_p ,space_p); //TODO:
	//ast_parse<factory_t>(str.c_str(),str.c_str()+str.size(),  oclFile,space_p);
        if (info.full)
        {

	#if defined(BOOST_SPIRIT_DUMP_PARSETREE_AS_XML)
	      std::map<parser_id, std::string> rule_names;
		    rule_names[oclExpressions.id()] = "oclExpression";
		    rule_names[letExpression.id()] = "letExpression";
		    rule_names[constraint.id()] = "constraint";
		    rule_names[contextDeclaration.id()] = "contextDeclaration";
		    rule_names[spatialOperationCallWithFoR.id()] = "spatialOperationCallWithFoR";


	//oclExpressions,constraint,contextDeclaration,classifierContext,operationContext,stereotype,operationName,formalParameterList,typeSpecifier,collectionType,oclExpression,returnType,expression,letExpression,ifExpression,logicalExpression,relationExpression,additiveExpression,multiplicativeExpression,unaryExpression,postfixExpression,primaryExpression,propertyCallParameters,simpleTypeSpecifier,literalCollection,collectionItem,propertyCall,qualifiers,pathName,timeExpression,actualParameterList,logicalOperator,collectionKind,relationalOperator,addOperator,multiplyOperator,unaryOperator;
	//formalParameter,constraintDefinition,propertyCallArrowParameters,operationCall,attributeCall,specialOperationCall;
	//collectionCall,iteratorCallParameters,iteratorCallName,collectionMethodName;
	//oclFile,packageName;

		    //tree_to_xml(cout, info.trees, str.c_str(), rule_names);
	#endif

	return info;
	}
	else
	{
		string result;
		result+="OCL Expression parsing failed\n"
			+string(str.c_str(),info.stop)+" <--\n";
		throw runtime_error(result.c_str());
	}
}

	AnyValue evalExpression(ExpressionTree & exp, const Context& c)
	{
		return evaluate(exp.trees.begin(),c);
	}

	AnyValue evalExpression(ExpressionTree & exp, shared_ptr<InstanceSpecification> self)
	{
		Context ctx;
		ctx["self"]=self;
		return evalExpression(exp,ctx);
	}
}

}

