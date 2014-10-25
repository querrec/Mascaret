#ifndef AREVI_MASCARET_PLATEFORM_H
#define AREVI_MASCARET_PLATEFORM_H

#ifdef _WIN32
#  ifdef _AREVI_MASCARET_LIB // This should be defined when building the VEHA dll
#    define AREVI_MASCARET_API __declspec( dllexport )
#  else
#    define AREVI_MASCARET_API __declspec( dllimport )
#  endif
#else // _WIN32
#  define AREVI_MASCARET_API
#endif // POSIX
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>

namespace AReViMascaret
{
	using std::string;
	using std::vector;
	using std::map;
	using std::list;
	using std::queue;
	using std::pair;
	using std::make_pair;
	using std::cout;
	using std::cerr;
	using std::endl;
	using boost::shared_ptr;
	using boost::weak_ptr;
	using boost::shared_dynamic_cast;
	using boost::make_shared;
	using boost::enable_shared_from_this;
}

#endif //AREVI_MASCARET_PLATEFORM_H
