grammar FipaSL;
 
options {
    language=CSharp2;
}

@header {
using System;
using System.Collections;
using System.Collections.Generic;
}

@members {

    public bool isAction = false;
	public bool isIota = false;
	public bool isEqual = false;

    public bool done = false;
    public bool started = false;

    public string value = "";

    public string performer = "";
    public string entityName = "";
    public string actionName = "";
    public string askedTerm = "";
	public string predicateSymbol = "";

    public List<string> paramValue = new List<string>();
    public List<string> paramName = new List<string>();

    
}

	
	

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

 content : LPAREN contentexpression RPAREN ;

 contentexpression : identifyingexpression
				   | actionexpression
				   | proposition ;

 proposition : wff;

 wff : LPAREN actionop actionexpression RPAREN
	 | atomicformula;

 atomicformula : propositionsymbol
				| LPAREN predicatesymbol {predicateSymbol = $predicatesymbol.text;} term+ RPAREN
				| LPAREN binarytempop term term {value=$term.text;} RPAREN
				| 'true'
				| 'false';

 actionop : 'done' {done = true;}
          | 'feasible'
		  | 'started' {started = true;};

 term : constant
	  | identifyingexpression
	  | variable;

 binarytempop : '=' {isEqual = true;}
			  | '>'
			  | '>='
			  | '<'
			  | '<='
			  | '!='
			  | 'member'
			  | 'contains'
			  | 'result';

 identifyingexpression : LPAREN referentialoperator {isIota = true;} term {askedTerm = $term.text;} wff RPAREN;

 referentialoperator : 'iota';

 actionexpression : LPAREN 'action'{isAction = true;} agent {performer = $agent.text;} functionalterm RPAREN;

 functionalterm : LPAREN functionsymbol {actionName = $functionsymbol.text;} (parameter)* RPAREN;

 parameter : parametername parametervalue ;

 parametername : COLON ID  {paramName.Add($ID.text);};

 parametervalue : term  {paramValue.Add($term.text);};

 constant : numericalconstant
          | ID;

 variable : variableidentifier;

 variableidentifier : QUES ID {paramName.Add($ID.text);};

 numericalconstant : integer
                   | float;

 agent : ID;

 predicatesymbol : ID;

 propositionsymbol : ID;

 functionsymbol : ID;

 integer : DIGIT+;

 float : DIGIT+'.'DIGIT+;

 DIGIT  : [0-9] ;
 ID : [a-zA-Z_]([a-zA-Z_-]|[0-9])*;
 WS : [ \t\r\n]+ -> skip;

 LPAREN : '(' ;
 RPAREN : ')' ;
 COLON  : ':' ;
 QUES : '?' ;
