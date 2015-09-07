//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     ANTLR Version: 4.3
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from C:\Users\JOANNA\Documents\Mascaret\Dev\CS\Mascaret\Mascaret\VEHA\Kernel\OCLExpression.g4 by ANTLR 4.3

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
using Antlr4.Runtime.Tree;
using IToken = Antlr4.Runtime.IToken;

/// <summary>
/// This interface defines a complete generic visitor for a parse tree produced
/// by <see cref="OCLExpressionParser"/>.
/// </summary>
/// <typeparam name="Result">The return type of the visit operation.</typeparam>
[System.CodeDom.Compiler.GeneratedCode("ANTLR", "4.3")]
[System.CLSCompliant(false)]
public interface IOCLExpressionVisitor<Result> : IParseTreeVisitor<Result> {
	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.expression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitExpression([NotNull] OCLExpressionParser.ExpressionContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.relationaloperator"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitRelationaloperator([NotNull] OCLExpressionParser.RelationaloperatorContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.unaryoperator"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitUnaryoperator([NotNull] OCLExpressionParser.UnaryoperatorContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.numericalconstant"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitNumericalconstant([NotNull] OCLExpressionParser.NumericalconstantContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.additiveexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitAdditiveexpression([NotNull] OCLExpressionParser.AdditiveexpressionContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.integer"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitInteger([NotNull] OCLExpressionParser.IntegerContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.postfixexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitPostfixexpression([NotNull] OCLExpressionParser.PostfixexpressionContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.propertycall"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitPropertycall([NotNull] OCLExpressionParser.PropertycallContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.float"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitFloat([NotNull] OCLExpressionParser.FloatContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.unaryexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitUnaryexpression([NotNull] OCLExpressionParser.UnaryexpressionContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.logicaloperator"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitLogicaloperator([NotNull] OCLExpressionParser.LogicaloperatorContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.primaryexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitPrimaryexpression([NotNull] OCLExpressionParser.PrimaryexpressionContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.multiplyoperator"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitMultiplyoperator([NotNull] OCLExpressionParser.MultiplyoperatorContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.relationexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitRelationexpression([NotNull] OCLExpressionParser.RelationexpressionContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.addoperator"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitAddoperator([NotNull] OCLExpressionParser.AddoperatorContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.logicalexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitLogicalexpression([NotNull] OCLExpressionParser.LogicalexpressionContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.multiplicativeexpression"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitMultiplicativeexpression([NotNull] OCLExpressionParser.MultiplicativeexpressionContext context);

	/// <summary>
	/// Visit a parse tree produced by <see cref="OCLExpressionParser.attributecall"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	/// <return>The visitor result.</return>
	Result VisitAttributecall([NotNull] OCLExpressionParser.AttributecallContext context);
}
} // namespace Mascaret
