#include <iostream>
#include <string>

#include "BEHAVE/fipaSLParser.h"

namespace BEHAVE {

FIPASLParserResult parserResult;


   // Terminaux
rule<> LPAREN, RPAREN, COLON, GUILL, ACTION, RESULT, TRUE_T, FALSE_T, DONE, SET, SEQUENCE, NOT, AND, OR, BELIEF, FEASIBLE, STARTED, QUESTION, EQUAL, DIESE, DOT;
  
   // Non terminaux
rule<> Content, ContentExpression, Proposition, Wff, AtomicFormula, ActionOp, Term, ActionExpression, FunctionalTerm, Parameter;
rule<> ParameterValue, Agent, PropositionSymbol, FunctionSymbol, PredicateSymbol, Constant, Set, Sequence, NumericalConstant;
rule<> UnaryLogicalOp, BinaryLogicalOp, BinaryTermOp;
rule<> ParameterName, VariableIdentifier, String, Float, Integer,Word, StringLiteral;


// Regles
void _initGrammar() 
{
// DEBUT GRAMMAIRE
// Terminaux
LPAREN                        = ch_p('(');
RPAREN                        = ch_p(')');
COLON                         = ch_p(':');
GUILL                         = ch_p('"');
QUESTION                      = ch_p('?');
EQUAL                         = ch_p('=');
DIESE                         = ch_p('#');
DOT                           = ch_p('.');

BELIEF                        = ch_p('B');

ACTION                        = str_p("action");
RESULT                        = str_p("result");
TRUE_T                        = str_p("true");
FALSE_T                       = str_p("false");
DONE                          = str_p("done");
FEASIBLE                      = str_p("feasible");
STARTED                       = str_p("started");
SET                           = str_p("set");
SEQUENCE                      = str_p("sequence");
NOT                           = str_p("not");
AND                           = str_p("and");
OR                            = str_p("or");

Content                       = LPAREN >> +ContentExpression >> RPAREN;

ContentExpression             = ActionExpression
                              | Proposition;

Proposition                   = Wff;

Wff                           = LPAREN >> UnaryLogicalOp >> space_p >> Wff >> RPAREN
   			      | LPAREN >> BinaryLogicalOp >> space_p >> Wff >> space_p >> Wff >> RPAREN
        		      | LPAREN >> ActionOp >> space_p >> ActionExpression >> RPAREN
        		      | AtomicFormula;


UnaryLogicalOp                = NOT;

BinaryLogicalOp               = AND
                              | OR;

AtomicFormula                 = FALSE_T
			      | TRUE_T
			      | LPAREN >> BinaryTermOp >> Term >> space_p >> Term >> RPAREN
                              | LPAREN >> PredicateSymbol >> space_p >> +Term >> RPAREN
                              | PropositionSymbol;

BinaryTermOp		      = RESULT
			      | EQUAL[&_Debug];

ActionOp                      = DONE[&_done]
			      | FEASIBLE
			      | STARTED[&_started];

Term                          = Constant [&_varValue]
                              | Set
                              | Sequence
                              | FunctionalTerm
                              | ActionExpression
			      | VariableIdentifier;

ActionExpression              = LPAREN >> ACTION >> space_p >> Agent[&_performer] >> space_p >> Term >> RPAREN;

FunctionalTerm                = LPAREN >> FunctionSymbol [&_action] >> *(space_p >> Term[&_paramValue]) >> RPAREN
                              | LPAREN >> FunctionSymbol >> *(space_p >> Parameter) >> RPAREN;

Parameter                     = ParameterName >> space_p >>ParameterValue[&_paramValue];

ParameterValue                = Term;

Agent                         = Term;

FunctionSymbol                = String;

PropositionSymbol             = String;

PredicateSymbol               = String;

Constant                      = NumericalConstant
                              | String;

Set                           = LPAREN >> SET >> *(space_p >> Term) >> RPAREN;

Sequence                      = LPAREN >> SEQUENCE >> *(space_p >> Term) >> RPAREN;

NumericalConstant             = Integer
                              | Float;

ParameterName                 = COLON >> String[&_paramName]; 

VariableIdentifier            = QUESTION >> String[&_entityName] >> DOT >> String[&_varName]; 

String                        = Word
                              | StringLiteral;

StringLiteral                 = GUILL >> *(anychar_p - "\"") >> GUILL;

Word                          = alpha_p >> *( (alnum_p - space_p) | '_');

Integer                       = int_p;

Float                         = real_p;

}
// FIN GRAMMAIRE

void _action(const char * first, const char * end)
{
   string s(first, end);
   parserResult.isAction = true;
   parserResult.action.actionName = s;
}
void _performer(const char * first, const char * end)
{
   string s(first, end);
   parserResult.action.performer = s;
}
void _paramValue(const char * first, const char * end)
{
   string s(first, end);
   parserResult.action.paramValue.push_back(s);
}

void _paramName(const char * first, const char * end)
{
   string s(first, end);
   parserResult.action.paramName.push_back(s);
}
void _varName(const char * first, const char * end)
{
   string s(first, end);
   parserResult.variable = s;
}
void _entityName(const char * first, const char * end)
{
   string s(first, end);
   parserResult.entity = s;
}
void _varValue(const char * first, const char * end)
{
   string s(first, end);
   parserResult.value = s;
}
void _done(const char * , const char * )
{
	parserResult.isDone=true;
}
void _started(const char * , const char * )
{
	parserResult.isStarted=true;
}

void _Debug(const char * first, const char * end)
{
   string s(first, end);
   cerr << " Debuging : " << s << endl;
}

void _resetParserResult()
{
   parserResult.success = false;
   parserResult.isAction = false;
   parserResult.isDone = false;
   parserResult.isStarted = false;
   parserResult.action.actionName = "";
   parserResult.action.performer = "";
   parserResult.action.paramName.erase(parserResult.action.paramName.begin(), parserResult.action.paramName.end());
   parserResult.action.paramValue.erase(parserResult.action.paramValue.begin(), parserResult.action.paramValue.end());
}

// Fonction de parse
FIPASLParserResult& parseFipaSLExpression(const string & str)
{
   _initGrammar();
   _resetParserResult();
   parse_info<const char*> result  = parse(str.c_str(), Content, space_p);

   parserResult.success = result.full;   
   return parserResult;

}

}

