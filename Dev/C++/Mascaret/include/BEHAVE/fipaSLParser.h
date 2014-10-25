#ifndef FIPASL_PARSER_H
#define FIPASL_PARSER_H
#include "Tools/veha_plateform.h"
#include <boost/spirit/include/classic_spirit.hpp>
using namespace boost::spirit::classic; 

namespace BEHAVE
{
VEHA_API typedef struct _SLaction
{
	string actionName;
	string performer;
	vector<string> paramName;
	vector<string> paramValue;
} SLAction;

VEHA_API typedef struct _FIPASLParserResult
{
	bool success;
	bool isAction;
	bool isDone;
	bool isStarted;
	SLAction action;
	string entity;
	string variable;
	string value;
	string predicate;
	string term;
} FIPASLParserResult;

VEHA_API FIPASLParserResult& parseFipaSLExpression(const string &String);

void _initGrammar(void);
void _resetParserResult(void);
void _action(const char * first, const char * end);
void _performer(const char * first, const char * end);
void _paramValue(const char * first, const char * end);
void _paramName(const char * first, const char * end);
void _entityName(const char * first, const char * end);
void _varName(const char * first, const char * end);
void _varValue(const char * first, const char * end);
void _done(const char * first, const char * end);
void _started(const char * first, const char * end);

void _Debug(const char * first, const char * end);


}

#endif
