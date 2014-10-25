#include "VEHA/Behavior/Time/TimeExpression.h"

#ifdef _WIN32
#include "Tools/utils.h"
#else
#include <sys/select.h>
#endif
#include <sys/time.h>

#include <string>
#include <sstream>

using std::string;
using std::stringstream;

namespace VEHA
{

TimeExpression::TimeExpression():
    _time(0.0)
{
}

TimeExpression::TimeExpression(double time):
    _time(time)
{
}

TimeExpression::TimeExpression(int year, int month, int day, int hour, int min, int sec)
{
    struct tm date;
    date.tm_year = year-1900;
    date.tm_mon = month-1;
    date.tm_mday = day;
    date.tm_hour = hour;
    date.tm_min = min;
    date.tm_sec = sec;
    date.tm_isdst = -1;
    _time = (double)mktime(&date);
}

TimeExpression::~TimeExpression()
{
}

double TimeExpression::getAsDouble() const
{
    return _time;
}

string TimeExpression::getAsString() const
{
    stringstream stream;
    time_t tmp = (time_t)_time;
    stream << ctime(&tmp);
    return stream.str().erase(stream.str().size()-1);
}

TimeExpression TimeExpression::createTimeOfDay()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv,&tz);
    return TimeExpression(((double)tv.tv_sec)+(((double)tv.tv_usec)*1e-6));
}

}
