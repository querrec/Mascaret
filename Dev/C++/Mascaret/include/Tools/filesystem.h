#ifndef __TOOLS__FILESYSTEM_H__
#define __TOOLS__FILESYSTEM_H__

#include <boost/version.hpp>
#if BOOST_VERSION <= 104700
#define BOOST_FILESYSTEM_VERSION 2
#else
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include <boost/filesystem.hpp>

#if BOOST_VERSION <= 104700
#define DECLARE_NAMESPACE_FILESYSTEM namespace fs=boost::filesystem
#else
# if BOOST_VERSION <= 104900
#define DECLARE_NAMESPACE_FILESYSTEM namespace fs=boost::filesystem3
# else
#define DECLARE_NAMESPACE_FILESYSTEM namespace fs=boost::filesystem
# endif
#endif                                  

#endif /* #ifndef __TOOLS__FILESYSTEM_H__ */
