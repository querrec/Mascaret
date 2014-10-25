
#include "httpServer/httpSession.h"
#include <sstream>

using std::ostringstream;
long HttpSession::_nb=0;

HttpSession::HttpSession()
{
	ostringstream oss;
	oss << _nb;
	_id=oss.str();
	_nb++;
}
HttpSession::~HttpSession()
{
}
