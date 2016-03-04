grammar OCLExpression ;

options {
    language=CSharp2;
}

@header {
using System;
using System.Collections;
using System.Collections.Generic;
}

@members {
	public Dictionary<string, ValueSpecification> context;
	private ValueSpecification lvalue;
	public ValueSpecification value;

}

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

 expression	: logicalexpression;

 logicalexpression : relationexpression (logicaloperator relationexpression)* ;

 relationexpression : additiveexpression {lvalue = value;}
											 (('=' additiveexpression {double l = ((LiteralReal)lvalue).RValue; double r = ((LiteralReal)value).RValue; if (l == r) value = new LiteralBoolean(true); else value = new LiteralBoolean(false);})
											|('<>' additiveexpression {double l = ((LiteralReal)lvalue).RValue; double r = ((LiteralReal)value).RValue; if (l != r) value = new LiteralBoolean(true); else value = new LiteralBoolean(false);})
											|('<' additiveexpression {double l = ((LiteralReal)lvalue).RValue; double r = ((LiteralReal)value).RValue; if (l < r) value = new LiteralBoolean(true); else value = new LiteralBoolean(false);})
											|('>' additiveexpression {double l = ((LiteralReal)lvalue).RValue; double r = ((LiteralReal)value).RValue; MascaretApplication.Instance.VRComponentFactory.Log(""+l +">" +r); if (l > r) value = new LiteralBoolean(true); else value = new LiteralBoolean(false);})
											|('>=' additiveexpression {double l = ((LiteralReal)lvalue).RValue; double r = ((LiteralReal)value).RValue; if (l >= r) value = new LiteralBoolean(true); else value = new LiteralBoolean(false);})
											|('<=' additiveexpression {double l = ((LiteralReal)lvalue).RValue; double r = ((LiteralReal)value).RValue; if (l <= r) value = new LiteralBoolean(true); else value = new LiteralBoolean(false);})
											)* ;

 additiveexpression : multiplicativeexpression (addoperator multiplicativeexpression)* ;

 multiplicativeexpression : unaryexpression (multiplyoperator unaryexpression)* ;

 unaryexpression : unaryoperator postfixexpression
				 | postfixexpression;

 postfixexpression : primaryexpression (DOT propertycall {MascaretApplication.Instance.VRComponentFactory.Log("Prop " + $propertycall.text);value = (((InstanceValue)value).SpecValue).Slots[$propertycall.text].getValue(); })* ;

 primaryexpression : STRING { MascaretApplication.Instance.VRComponentFactory.Log("PRIME" + $STRING.text); value = context[$STRING.text];} | numericalconstant {value = new LiteralReal($numericalconstant.text);};

 propertycall : attributecall ;

 attributecall : STRING ;

 logicaloperator : 'and' | 'or' | 'implies'| 'xor' ;

 relationaloperator : EQUAL | NEQUAL | GE | LE | GT | LT ;

 addoperator : PLUS | MINUS;

 multiplyoperator : MULT | DIVIDE;

 unaryoperator : MINUS | 'not';

 numericalconstant : integer
                   | float;

 LPAREN : '(' ;
 RPAREN : ')' ;
 COLON  : ':' ;
 EQUAL	: '=' ;
 NEQUAL	: '<>' ;
 LT		: '<' ;
 GT		: '>' ;
 LE		: '<=' ;
 GE		: '>=' ;
 RARROW	: '->';
 DOTDOT	: '..' ;
 DOT	: '.' ;
 POUND	: '#' ;
 SEMICOL: ';' ;
 PLUS	: '+' ;
 MINUS	: '-' ;
 MULT	: '*' ;
 DIVIDE	: '/' ;

 integer : DIGIT+;

 float : DIGIT+'.'DIGIT+;

 DIGIT  : [0-9] ;
 STRING : [a-zA-Z_]([a-zA-Z]|[0-9])*;
 WS : [ \t\r\n]+ -> skip;
