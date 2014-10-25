#ifndef COOKIE_H
#define COOKIE_H 1

#include <iostream>

class Cookie
{
	public:
		Cookie(const std::string& name, const std::string& value, int expires);
		virtual ~Cookie();
		std::string getName();
		std::string getValue();
		int getExpires();
	private:
		std::string _name;
		std::string _value;
		int _expires;
};
inline Cookie::Cookie(const std::string& name, const std::string& value, int expires)
{
	_name = name;
	_value = value;
	_expires = expires;
}
inline Cookie::~Cookie()
{
}
inline std::string Cookie::getName()
{
	return _name;
}
inline std::string Cookie::getValue()
{
	return _value;
}
inline int Cookie::getExpires()
{
	return _expires;
}

#endif
