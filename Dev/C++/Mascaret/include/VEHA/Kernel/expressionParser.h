#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H
//#define BOOST_SPIRIT_DEBUG 1
#include <boost/spirit/include/classic_spirit.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>

#include "VEHA/Kernel/AnyValueSpecification.h"
namespace VEHA
{
namespace OclParser
{
using namespace boost::spirit::classic; //BOOST_SPIRIT_CLASSIC_NS;
// Here's some typedefs to simplify things
typedef AnyValue data_t; 

typedef node_val_data_factory<data_t> factory_t;
typedef char const*         iterator_t;
typedef tree_match<iterator_t,factory_t> parse_tree_match_t;
typedef parse_tree_match_t::const_tree_iterator iter_t;
typedef tree_parse_info<iterator_t,factory_t> ExpressionTree;
typedef map<string,AnyValue> Context;

	VEHA_API ExpressionTree parseExpression(const string &String);
	VEHA_API AnyValue evalExpression(ExpressionTree & exp, const Context& c);
	VEHA_API AnyValue evalExpression(ExpressionTree & exp, shared_ptr<InstanceSpecification> self);
}
}

#endif
