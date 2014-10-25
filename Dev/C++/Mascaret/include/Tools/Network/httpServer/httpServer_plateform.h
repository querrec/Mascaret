#ifndef HTTPSERVER_PLATEFORM_H
#define HTTPSERVER_PLATEFORM_H

#ifdef _WIN32
#  ifdef _HTTPSERVER_LIB // This should be defined when building the VEHA dll
#    define HTTPSERVER_API __declspec( dllexport )
#  else
#    define HTTPSERVER_API __declspec( dllimport )
#  endif
#else // _WIN32
#  define HTTPSERVER_API
#endif // POSIX

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using boost::shared_ptr;

#endif
