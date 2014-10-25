#ifndef ANIMLIB_PLATEFORM_H
#define ANIMLIB_PLATEFORM_H

#ifdef _WIN32
#  ifdef _ANIMLIB_LIB // This should be defined when building the ANIMLIB dll
#    define ANIMLIB_API __declspec( dllexport )
#  else
#    define ANIMLIB_API __declspec( dllimport )
#  endif
#else // _WIN32
#  define ANIMLIB_API
#endif // POSIX

#endif //ANIMLIB_PLATEFORM_H
