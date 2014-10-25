#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H 1
#include "httpServer_plateform.h"
#include <iostream>
#include <map>
#include <vector>

class HTTPSERVER_API HttpSession
{
	public:
		HttpSession();
		virtual ~HttpSession();
		std::string getId();
		std::map<std::string,void*> getAttributes();
		std::vector<std::string> getAttributesNames();
		void* getAttribute(std::string& name);
		void setAttribute(std::string& name, void* value);
	private:
		void setId(const std::string& id);
		std::string _id;
		std::map<std::string,void*> _attributes;
		static long _nb;
};

inline std::string HttpSession::getId()
{
	return _id;
}
inline void HttpSession::setAttribute(std::string & name, void *value)
{
	_attributes[name]=value;
}

inline void HttpSession::setId(const std::string& id)
{
	_id=id;
}
inline std::map<std::string,void*> HttpSession::getAttributes()
{
	return _attributes;
}

inline std::vector<std::string> HttpSession::getAttributesNames()
{
	return std::vector<std::string>();
}
inline void* HttpSession::getAttribute(std::string& name)
{
	return _attributes[name];
}
#endif
