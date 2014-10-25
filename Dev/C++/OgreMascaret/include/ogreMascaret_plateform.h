#ifndef OGRE_MASCARET_PLATEFORM_H
#define OGRE_MASCARET_PLATEFORM_H

#ifdef _WIN32
#  ifdef _OGRE_MASCARET_LIB // This should be defined when building the OgreMascaret dll
#    define OGRE_MASCARET_API __declspec( dllexport )
#  else
#    define OGRE_MASCARET_API __declspec( dllimport )
#  endif
#else // _WIN32
#  define OGRE_MASCARET_API
#endif // POSIX

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

namespace OgreMascaret
{
    using std::string;
    using std::vector;
    using std::map;
    using boost::shared_ptr;
    using boost::shared_dynamic_cast;
}

#endif //OGRE_MASCARET_PLATEFORM_H
