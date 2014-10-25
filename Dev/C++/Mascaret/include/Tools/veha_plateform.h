#ifndef VEHA_PLATEFORM_H
#define VEHA_PLATEFORM_H

#ifdef _WIN32
#  ifdef _VEHA_LIB // This should be defined when building the VEHA dll
#    define VEHA_API __declspec( dllexport )
#  else
#    define VEHA_API __declspec( dllimport )
#  endif
#define VEHA_PLUGIN __declspec( dllexport )
#else // _WIN32
#  define VEHA_API
#define VEHA_PLUGIN
#endif // POSIX
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
namespace BEHAVE
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
	using boost::make_shared;
	using boost::weak_ptr;
	using boost::shared_dynamic_cast;
	using boost::enable_shared_from_this;
}
namespace HAVE
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
	using boost::make_shared;
	using boost::weak_ptr;
	using boost::shared_dynamic_cast;
	using boost::enable_shared_from_this;
}
namespace VEHA
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
	using boost::make_shared;
	using boost::weak_ptr;
	using boost::shared_dynamic_cast;
	using boost::enable_shared_from_this;
}
namespace IEHA
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
	using boost::make_shared;
	using boost::weak_ptr;
	using boost::shared_dynamic_cast;
	using boost::enable_shared_from_this;
}
namespace GraphTools
{
	using std::string;
	using std::vector;
	using std::pair;
	using std::make_pair;
	using std::cout;
	using std::cerr;
	using std::endl;
	using boost::shared_ptr;
}
namespace BackupUtils
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
	using boost::make_shared;
	using boost::weak_ptr;
	using boost::shared_dynamic_cast;
	using boost::enable_shared_from_this;
}
namespace Poseidon
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
	using boost::make_shared;
	using boost::weak_ptr;
	using boost::shared_dynamic_cast;
	using boost::enable_shared_from_this;
}
#endif //HLIB_PLATEFORM_H
