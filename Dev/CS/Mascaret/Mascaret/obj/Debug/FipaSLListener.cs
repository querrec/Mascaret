//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     ANTLR Version: 4.3
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from C:\Users\Utilisateur\Developpement\Mascaret\Mascaret\Dev\CS\Mascaret\Mascaret\BEHAVE\FipaSL.g4 by ANTLR 4.3

// Unreachable code detected
#pragma warning disable 0162
// The variable '...' is assigned but its value is never used
#pragma warning disable 0219
// Missing XML comment for publicly visible type or member '...'
#pragma warning disable 1591

namespace Mascaret {

using System;
using System.Collections;
using System.Collections.Generic;

using Antlr4.Runtime.Misc;
using IParseTreeListener = Antlr4.Runtime.Tree.IParseTreeListener;
using IToken = Antlr4.Runtime.IToken;

/// <summary>
/// This interface defines a complete listener for a parse tree produced by
/// <see cref="FipaSLParser"/>.
/// </summary>
[System.CodeDom.Compiler.GeneratedCode("ANTLR", "4.3")]
[System.CLSCompliant(false)]
public interface IFipaSLListener : IParseTreeListener {
	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.parametervalue"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterParametervalue([NotNull] FipaSLParser.ParametervalueContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.parametervalue"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitParametervalue([NotNull] FipaSLParser.ParametervalueContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.agent"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterAgent([NotNull] FipaSLParser.AgentContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.agent"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitAgent([NotNull] FipaSLParser.AgentContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.constant"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterConstant([NotNull] FipaSLParser.ConstantContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.constant"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitConstant([NotNull] FipaSLParser.ConstantContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.numericalconstant"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterNumericalconstant([NotNull] FipaSLParser.NumericalconstantContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.numericalconstant"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitNumericalconstant([NotNull] FipaSLParser.NumericalconstantContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.contentexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterContentexpression([NotNull] FipaSLParser.ContentexpressionContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.contentexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitContentexpression([NotNull] FipaSLParser.ContentexpressionContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.integer"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterInteger([NotNull] FipaSLParser.IntegerContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.integer"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitInteger([NotNull] FipaSLParser.IntegerContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.float"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterFloat([NotNull] FipaSLParser.FloatContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.float"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitFloat([NotNull] FipaSLParser.FloatContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.actionexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterActionexpression([NotNull] FipaSLParser.ActionexpressionContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.actionexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitActionexpression([NotNull] FipaSLParser.ActionexpressionContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.content"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterContent([NotNull] FipaSLParser.ContentContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.content"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitContent([NotNull] FipaSLParser.ContentContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.functionalterm"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterFunctionalterm([NotNull] FipaSLParser.FunctionaltermContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.functionalterm"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitFunctionalterm([NotNull] FipaSLParser.FunctionaltermContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.wff"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterWff([NotNull] FipaSLParser.WffContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.wff"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitWff([NotNull] FipaSLParser.WffContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.proposition"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterProposition([NotNull] FipaSLParser.PropositionContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.proposition"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitProposition([NotNull] FipaSLParser.PropositionContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.parameter"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterParameter([NotNull] FipaSLParser.ParameterContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.parameter"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitParameter([NotNull] FipaSLParser.ParameterContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.functionsymbol"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterFunctionsymbol([NotNull] FipaSLParser.FunctionsymbolContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.functionsymbol"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitFunctionsymbol([NotNull] FipaSLParser.FunctionsymbolContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.parametername"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterParametername([NotNull] FipaSLParser.ParameternameContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.parametername"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitParametername([NotNull] FipaSLParser.ParameternameContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.term"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterTerm([NotNull] FipaSLParser.TermContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.term"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitTerm([NotNull] FipaSLParser.TermContext context);

	/// <summary>
	/// Enter a parse tree produced by <see cref="FipaSLParser.actionop"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterActionop([NotNull] FipaSLParser.ActionopContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="FipaSLParser.actionop"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitActionop([NotNull] FipaSLParser.ActionopContext context);
}
} // namespace Mascaret
