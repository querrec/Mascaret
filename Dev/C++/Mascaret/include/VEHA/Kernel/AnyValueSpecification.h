#ifndef VEHA_KERNEL_ANY_VALUESPECIFICATION_H
#define VEHA_KERNEL_ANY_VALUESPECIFICATION_H
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
//#include <boost/any.hpp>
//#include <boost/operators.hpp>
#include <boost/lexical_cast.hpp>

#include <stdexcept>
#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/InstanceValue.h"
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralBoolean.h"
#include "VEHA/Kernel/LiteralString.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Kernel/InstanceSpecification.h"

namespace VEHA
{
using boost::lexical_cast;
using std::runtime_error;
using std::ostream;
using std::istream;
class InstanceSpecification;


template<typename T>
struct is_additive
{
	static const bool value= boost::is_arithmetic<T>::value;
};
template<>
struct is_additive<string>
{
	static const bool value= true;
};
class CollectionAnyValue;
class AnyValue;

template<typename ValueType,typename T>
typename boost::enable_if_c<boost::is_arithmetic<ValueType>::value,T>::type
mult
(const ValueType& v,const T& value  )
{
	return v*(ValueType)value;
}
template<typename ValueType,typename T>
typename boost::disable_if<boost::is_arithmetic<ValueType>, T >::type
mult(const ValueType& v,const T& value )
{
	throw runtime_error("bad operation on string (*)");
}







class AnyValue //: boost::less_than_comparable1<AnyValue>, boost::equality_comparable1<AnyValue>
{
public:
	AnyValue()
	: content(0)
	{
	}

	//        template<typename ValueType>
	//        AnyValue(const ValueType & value)
	//          : content(new holderCast<ValueType>(value))
	//        {
	//        }
	AnyValue(const bool & value)
	: content(new holderCast<bool>(value))
	{
	}
	AnyValue(const int & value)
	: content(new holderCast<int>(value))
	{
	}
	AnyValue(const double & value)
	: content(new holderCast<double>(value))
	{
	}
	AnyValue(const string & value)
	: content(new holderCast<string>(value))
	{
	}
	AnyValue(const shared_ptr<InstanceSpecification> & value)
	: content(new holderCast<shared_ptr<InstanceSpecification> >(value))
	{
	}
	inline AnyValue(const CollectionAnyValue & value);


	AnyValue(const AnyValue & other)
	: content(other.content ? other.content->clone() : 0)
	{
	}

	~AnyValue()
	{
		delete content;
	}

public: // modifiers

	AnyValue & swap(AnyValue & rhs)
	{
		std::swap(content, rhs.content);
		return *this;
	}

	template<typename ValueType>
	AnyValue & operator=(const ValueType & rhs)
	{
		AnyValue(rhs).swap(*this);
		return *this;
	}

	AnyValue & operator=(const AnyValue & rhs)
	{
		AnyValue(rhs).swap(*this);
		return *this;
	}

public: // queries

	bool empty() const
	{
		return !content;
	}

	const std::type_info & type() const
	{
		return content ? content->type() : typeid(void);
	}
	template<typename ValueType>
	AnyValue operator+(const ValueType& value) const
	{
		return (*content)+AnyValue(value);
	}
	AnyValue operator+(const AnyValue& value) const
	{
		return (*content)+AnyValue(value);
	}
	template<typename ValueType>
	AnyValue operator-(const ValueType& value) const
	{
		return (*content)-AnyValue(value);
	}
	AnyValue operator-(const AnyValue& value) const
	{
		return (*content)-AnyValue(value);
	}
	template<typename ValueType>
	AnyValue operator*(const ValueType& value) const
	{
		//	return content->mult(AnyValue(value));
		return (*content)*AnyValue(value);
	}
	AnyValue operator*(const AnyValue& value) const
	{
		//	return content->mult(AnyValue(value));
		return (*content)*AnyValue(value);
	}
	template<typename ValueType>
	AnyValue operator/(const ValueType& value) const
	{
		return (*content)/AnyValue(value);
	}
	AnyValue operator/(const AnyValue& value) const
	{
		return (*content)/AnyValue(value);
	}
	template<typename ValueType>
	bool operator==(const ValueType& value) const
	{
		return (*content)==AnyValue(value);
	}
	bool operator==(const AnyValue& value) const
			{
		return (*content)==AnyValue(value);
			}
	template<typename ValueType>
	bool operator<(const ValueType& value) const
	{
		return (*content)<AnyValue(value);
	}
	bool operator<(const AnyValue& value) const
	{
		return (*content)<AnyValue(value);
	}
	//
	template<typename ValueType>
	bool operator>(const ValueType& value) const
	{
		//return AnyValue(value)<(*content);
		return (*AnyValue(value).content)<(*this);
	}
	bool operator>(const AnyValue& value) const
	{
		return (*AnyValue(value).content)<(*this);
	}
	template<typename ValueType>
	bool operator<=(const ValueType& value) const
	{
		return !((*AnyValue(value).content)<(*this));
	}
	bool operator<=(const AnyValue& value) const
			{
		return !((*AnyValue(value).content)<(*this));
			}
	template<typename ValueType>
	bool operator>=(const ValueType& value) const
	{
		return !((*content)<AnyValue(value));
	}
	bool operator>=(const AnyValue& value) const
			{
		return !((*content)<AnyValue(value));
			}
	template<typename ValueType>
	bool operator!=(const ValueType& value) const
	{
		return !((*content)==AnyValue(value));
	}
	bool operator!=(const AnyValue& value) const
	{
		return !((*content)==AnyValue(value));
	}
	/*
	template<typename ValueType>
		ValueType operator-(const ValueTypeIn& value) const{return placeholder-value;}
	template<typename ValueType>
		ValueType operator/(const ValueTypeIn& value) const{}
	template<typename ValueType>
		ValueType operator*(const ValueTypeIn& value) const{}
	template<typename ValueType>
		bool operator>(const ValueTypeIn& value) const{}
	template<typename ValueType>
		bool operator<(const ValueTypeIn& value) const{}
	 */

	operator string() const{return (content?string(*content):"nil");}
	operator double() const{return double(*content);}
	operator int() const {return int(*content);}
	operator bool() const
	{
		if(content)
		{
			if(string(*content) == "true")
				return true;
			else if(string(*content) == "false")
				return false;
		}
		return bool(*content);
	}
	inline operator CollectionAnyValue() const;  //bool(*content);}
	operator shared_ptr<InstanceSpecification>() const {return shared_ptr<InstanceSpecification>(*content);}

	void replaceValueFromString(const string& value)
	{
		if (!content) //default: string:
			content=new holderCast<string>(value);
		else
		{
			content->setValueFromString(value);
		}
	}

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
private: // types
#else
public: // types (public so AnyValue_cast can be non-friend)
#endif

	class placeholder
	{
	public: // structors

		virtual ~placeholder()
		{
		}

	public: // queries

		virtual const std::type_info & type() const = 0;

		virtual placeholder * clone() const = 0;

		virtual AnyValue operator+(const AnyValue& value) const=0;
		virtual AnyValue operator-(const AnyValue& value) const=0;
		virtual AnyValue operator/(const AnyValue& value) const=0;
		virtual AnyValue operator*(const AnyValue& value) const=0;
		virtual bool operator==(const AnyValue& value) const=0;
		virtual bool operator<(const AnyValue& value) const=0;


		virtual  operator string() const=0;
		virtual  operator double() const=0;
		virtual  operator int() const=0;
		virtual  operator bool() const=0;
		virtual  operator shared_ptr<InstanceSpecification>() const=0;
		virtual operator CollectionAnyValue() const=0;

		virtual void setValueFromString(const string &value)=0;

	};

	template<typename ValueType>
	class holder : public placeholder
	{
	public: // structors

		holder(const ValueType & value)
		: held(value)
		{
		}

	public: // queries

		virtual const std::type_info & type() const
		{
			return typeid(ValueType);
		}

		virtual placeholder * clone() const
            				{
			return new holder(held);
            				}
		inline virtual AnyValue operator+(const AnyValue& value) const
		{return _plus(value);}
		inline virtual AnyValue operator-(const AnyValue& value) const
		{return _minus(value);}
		inline virtual AnyValue operator/(const AnyValue& value) const
		{return _divide(value);}
		inline virtual AnyValue operator*(const AnyValue& value) const
		{ return _mult(value);}
		inline virtual bool operator==(const AnyValue& value) const { return (held==(ValueType)value);}
		inline virtual bool operator<(const AnyValue& value) const {return (held<(ValueType)value);}

		inline virtual  operator string() const;
		inline virtual  operator double() const{
			return lexical_cast<double>(held);
		}
		inline virtual  operator int() const{
			return lexical_cast<int>(held);
		}
		inline virtual  operator bool() const{
			return lexical_cast<bool>(held);
		}
		inline virtual operator shared_ptr<InstanceSpecification>() const;
		inline virtual operator CollectionAnyValue() const;

		inline virtual void setValueFromString(const string &value);

	protected:
		template<typename T> 
		inline ValueType
		_plus (const T& value,typename boost::enable_if_c<is_additive<ValueType>::value,T>::type dummy=0  ) const
		{
			(void)dummy; // to avoid unused parameter warning
			return held+(ValueType)value;
		}
		template<typename T> 
		inline ValueType
		_plus(const T& ,typename boost::disable_if<is_additive<ValueType>, T >::type dummy=0) const
		{
			(void)dummy; // to avoid unused parameter warning
			throw runtime_error("bad operation (+) on value ");
		}
		template<typename T> 
		inline ValueType
		_minus (const T& value, typename boost::enable_if_c<boost::is_arithmetic<ValueType>::value,T>::type dummy=0  ) const
		{
			(void)dummy; // to avoid unused parameter warning
			return held-(ValueType)value;
		}
		template<typename T> 
		inline ValueType
		_minus(const T&,typename boost::disable_if<boost::is_arithmetic<ValueType>, T >::type dummy=0 ) const
		{
			(void)dummy; // to avoid unused parameter warning
			throw runtime_error("bad operation (-) on value ");
		}
		template<typename T> 
		inline ValueType
		_divide (const T& value, typename boost::enable_if_c<boost::is_arithmetic<ValueType>::value,T>::type dummy=0  ) const
		{
			(void)dummy; // to avoid unused parameter warning
			return held/(ValueType)value;
		}
		template<typename T> 
		inline ValueType
		_divide(const T&, typename boost::disable_if<boost::is_arithmetic<ValueType>, T >::type dummy=0 ) const
		{
			(void)dummy; // to avoid unused parameter warning
			throw runtime_error("bad operation (/) on value ");
		}
		template<typename T> 
		inline ValueType
		_mult (const T& value, typename boost::enable_if_c<boost::is_arithmetic<ValueType>::value,T>::type dummy=0  ) const
		{
			(void)dummy; // to avoid unused parameter warning
			return held*(ValueType)value;
		}
		template<typename T> 
		inline typename boost::disable_if<boost::is_arithmetic<ValueType>, T >::type
		_mult(const T& ) const
		{
			throw runtime_error("bad operation (*) on value ");
		}




	public: // representation

		ValueType held;

	};

	template <typename ValueType>
	class holderCast : public holder<ValueType>
	{
	public:
		holderCast(const ValueType & value)
		:holder<ValueType>(value){}
		inline virtual  operator ValueType() const
								{
			return holder<ValueType>::held;
								}

	};

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	private: // representation

	template<typename ValueType>
	friend ValueType * any_value_cast(AnyValue *);

	template<typename ValueType>
	friend ValueType * unsafe_AnyValue_cast(AnyValue *);

#else

	public: // representation (public so AnyValue_cast can be non-friend)

#endif

	placeholder * content;

};






/*
template<typename ValueType>
AnyValue AnyValue::holder<ValueType>::operator*(const AnyValue& value) const
{
	return held*(ValueType)value;
}*/
template<>
inline AnyValue AnyValue::holder<string>::operator-(const AnyValue& ) const
{
	throw runtime_error("bad operation on string (-)");
}
template<>
inline AnyValue AnyValue::holder<string>::operator/(const AnyValue& ) const
{	throw runtime_error("bad operation on string (/)");
}
//		template<>
//		AnyValue AnyValue::holder<string>::operator*(const AnyValue& ) const
//		{	throw runtime_error("bad operation on string (*)");
//		}
inline ostream& operator<<(ostream& str,const AnyValue& val)
{
	return str<<(string)val;
}



class bad_any_value_cast : public std::bad_cast
{
public:
	virtual const char * what() const throw()
	{
		return "boost::bad_any_cast: "
				"failed conversion using boost::any_cast";
	}
};

template<typename ValueType>
ValueType * any_value_cast(AnyValue * operand)
{
	return operand && operand->type() == typeid(ValueType)
			? &static_cast<AnyValue::holder<ValueType> *>(operand->content)->held
					: 0;
}

template<typename ValueType>
const ValueType * any_value_cast(const AnyValue * operand)
{
	return any_value_cast<ValueType>(const_cast<AnyValue *>(operand));
}

template<typename ValueType>
ValueType any_value_cast(const AnyValue & operand)
{
	typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
// If 'nonref' is still reference type, it means the user has not
	// specialized 'remove_reference'.

	// Please use BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION macro
	// to generate specialization of remove_reference for your class
	// See type traits library documentation for details
	BOOST_STATIC_ASSERT(!is_reference<nonref>::value);
#endif

	const nonref * result = any_value_cast<nonref>(&operand);
	if(!result)
		boost::throw_exception(bad_any_value_cast());
	return *result;
}

template<typename ValueType>
ValueType any_value_cast(AnyValue & operand)
{
	typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
	// The comment in the above version of 'any_cast' explains when this
	// assert is fired and what to do.
	BOOST_STATIC_ASSERT(!is_reference<nonref>::value);
#endif

	nonref * result = any_value_cast<nonref>(&operand);
	if(!result)
		boost::throw_exception(bad_any_value_cast());
	return *result;
}

// Note: The "unsafe" versions of any_cast are not part of the
// public interface and may be removed at any time. They are
// required where we know what type is stored in the any and can't
// use typeid() comparison, e.g., when our types may travel across
// different shared libraries.
//    template<typename ValueType>
//    inline ValueType * unsafe_any_cast(any * operand)
//    {
//        return &static_cast<any::holder<ValueType> *>(operand->content)->held;
//    }
//
//    template<typename ValueType>
//    inline const ValueType * unsafe_any_cast(const any * operand)
//    {
//        return unsafe_any_cast<ValueType>(const_cast<any *>(operand));
//    }

class CollectionAnyValue : public vector<AnyValue>
{
public:
	enum Type{Set,Sequence,Bag};
	CollectionAnyValue():type(Set){}
	CollectionAnyValue(const vector<AnyValue>& c):vector<AnyValue>(c){}
	Type type;

};
inline ostream& operator<<(ostream& s,const CollectionAnyValue& c)
{
	s<<"[ ";
	size_t i;
	for(i=0;i+1<c.size();i++)
		s<<c[i]<<" , ";
	if (i!=c.size())
		s<<c[i];
	s<<" ]";
	return s;
}
inline AnyValue::operator CollectionAnyValue() const
			{
	return (CollectionAnyValue)(*content);
			}
template<typename ValueType>
inline AnyValue::holder<ValueType>::operator string() const
{
	return lexical_cast<string>(held);
}
template<>
inline AnyValue::holder<shared_ptr<InstanceSpecification> >::operator string() const
{
	return (held?held->getName():"nil");
}
template<typename ValueType>
inline void AnyValue::holder<ValueType>::setValueFromString(const string &value)
{
	held=lexical_cast<ValueType>(value);
}
template<>
inline void AnyValue::holder<shared_ptr<InstanceSpecification> >::setValueFromString(const string &)
{
	throw std::runtime_error("Can't set InstanceSpecification from string");
}

template<typename ValueType>
inline AnyValue::holder<ValueType>::operator shared_ptr<InstanceSpecification>() const
{
	throw std::runtime_error("Value is not a InstanceSpecification");
}
template<>
inline AnyValue::holder<shared_ptr<InstanceSpecification> >::operator shared_ptr<InstanceSpecification>() const
{
	return held;
}
template<typename ValueType>
inline AnyValue::holder<ValueType>::operator CollectionAnyValue() const
{
	throw std::runtime_error("Value is not a CollectionAnyValue");
}
template<>
inline AnyValue::holder<CollectionAnyValue>::operator CollectionAnyValue() const
{
	return held;
}
//helper
inline istream& operator>>(istream& s,CollectionAnyValue& c)
{
	string str;
	s>>str;
	if (str=="[")
	{
		s>>str;
		while(str!="]") { c.push_back(str);
		s>>str;
		if (str!=",") break;
		}
		if (str=="]") return s;
	}
	throw std::runtime_error("operator >> CollectionAnyValue error");
}



inline AnyValue toAnyValue(shared_ptr<ValueSpecification> v)
{
	if (v->getType()->getName()=="real")
		return (double)(*v);
	else if (v->getType()->getName()=="boolean")
		return (bool)(*v);
	else if (v->getType()->getName()=="integer")
		return (int)(*v);
	else if (v->getType()->getName()=="string")
		return (string)(*v);
	else
		return (shared_ptr<InstanceSpecification>)(*v);
}
inline boost::shared_ptr<ValueSpecification> toValueSpecification(const AnyValue& v)
			{
	if(v.type()==typeid(double))return make_shared<LiteralReal>((double)v);
	else if(v.type()==typeid(int))return make_shared<LiteralInteger>((int)v);
	else if(v.type()==typeid(bool))return make_shared<LiteralBoolean>((bool)v);
	else if(v.type()==typeid(std::string))return boost::make_shared<LiteralString>((std::string)v);
	else return make_shared<InstanceValue>(shared_ptr<InstanceSpecification>(v));
			}
inline AnyValue::AnyValue(const CollectionAnyValue & value)
: content(new holderCast<CollectionAnyValue>(value))
{
}

}

#endif
