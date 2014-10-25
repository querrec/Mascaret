#ifndef TOOLS_UTILS_H
#define TOOLS_UTILS_H

#include <map>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#endif

#define ASSISTANCE_TIME 5

inline std::vector<std::string> split( const std::string& Msg, const std::string & Separators )
{
    std::vector<std::string> recup;
    typedef boost::tokenizer<boost::char_separator<char> > my_tok;
    boost::char_separator<char> sep( Separators.c_str() );
    my_tok tok( Msg, sep );
    for ( my_tok::const_iterator i = tok.begin();i != tok.end();++i )
	recup.push_back(*i);

    return recup;
}

template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline _Tp MAP_AT(const std::map<_Key, _Tp, _Compare, _Alloc> &c, const _Key& v)
{
    typename std::map<_Key, _Tp, _Compare, _Alloc>::const_iterator it;
    if ((it=c.find(v))!=c.end())
        return it->second;
    else throw std::out_of_range("map::out_of_range");
}


//MAP_ADDING: recupere le nom de la valeur  inserer, et le change s'il existe dj
template<typename _Tp>
inline std::string MAP_ADDING(std::map<std::string, _Tp> &c, const _Tp& value)
{
    std::string name=value->getName();
    int id=1;
    while(c.find(name)!=c.end())
    {
        name=value->getName()+"."+boost::lexical_cast<std::string>(id);
        ++id;
    }
    c.insert(std::make_pair(name,value));
    return name;
}

//Remove element from a vector, using its index, keeping elements order
template <typename T>
void vectorOrderedErase(std::vector<T> & vect, unsigned int index)
{
  typedef typename std::vector<T>::iterator clt;
  clt it;
  it = vect.begin();
  for(unsigned int i=0; i<index; i++)
    it++;
  vect.erase(it);
}

//Convert string to int, float or whatever
// the third parameter of from_string() should be one of std::hex, std::dec (default) or std::oct
template <class T> //<int>, <float>, etc...
bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&) = std::dec)
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}
#ifdef _WIN32
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

#ifndef __MINGW32__
struct timezone
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};


inline int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;
  
  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;


    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
	tmpres /= 10;  /*convert into microseconds*/
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
    long timezone;
    errno_t err=_get_timezone(&timezone);
	tz->tz_minuteswest = timezone / 60;
	int daylight;
	err=_get_daylight(&daylight);
	tz->tz_dsttime = daylight;
#else
	tz->tz_minuteswest = _timezone / 60;
	tz->tz_dsttime = _daylight;
#endif
  }

  return 0;
}
#endif
#endif

#define GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 \
    + __GNUC_PATCHLEVEL__)

const std::string pythonLibPath="../WrapperBoostPy/";
#endif

#if defined(_MSC_VER)
#define PRE_DEPRECATED __declspec(deprecated)
#define POST_DEPRECATED
#elif defined(__GNUC__)
#define PRE_DEPRECATED
#define POST_DEPRECATED __attribute__ ((deprecated))
#else
#define PRE_DEPRECATED
#define POST_DEPRECATED
#endif
