#ifndef log_hh
#define log_hh
#include <string>
#include <iostream>
using namespace std;

#include <constants.hh>
#include <commons.hh>
#include <boost.hh>

inline void logGlOut(const string message, va_list ap)
{
   if(constants::log_opengl)
   {
      cout << "0PENGL : " << fPrint(message,ap) << endl;
   }
}

inline void logGlOut(const string message, ...)
{
   if(constants::log_opengl)
   {
      va_list ap;
      va_start(ap, message);
      logGlOut(message, ap);
      va_end(ap);
   }
}

// tells whether we should log or not.
bool shouldLog(const char* file, const int line, const char* func, const constants::LogLevel level);

#define debug(message, ...) if(constants::debug == constants::static_log_level) {_debug(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, message, ## __VA_ARGS__);}

inline void _debug(const char* file, const int line, const char* func, const string message, ...)
{
   if(constants::debug == constants::static_log_level && shouldLog(file, line, func, constants::debug))
   {
      va_list ap;
      va_start(ap, message);
      cout << "D3BUG : " << fPrint(fPrint(constants::log_format, file, line, func, message.c_str()),ap) << endl;
      va_end(ap);
   }
}

#define info(message, ...) if(constants::static_log_level <= constants::info) {_info(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, message, ## __VA_ARGS__);}

inline void _info(const char* file, const int line, const char* func, const string message, ...)
{
   if(constants::static_log_level <= constants::info && shouldLog(file, line, func, constants::info))
   {
      va_list ap;
      va_start(ap, message);
      cout << "INF0 : " << fPrint(fPrint(constants::log_format, file, line, func, message.c_str()),ap) << endl;
      va_end(ap);
   }
}

#define warning(message, ...) if(constants::static_log_level <= constants::warning) {_warning(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, message, ## __VA_ARGS__);}

inline void _warning(const char* file, const int line, const char* func, const string message, ...)
{
   if(constants::static_log_level <= constants::warning && shouldLog(file, line, func, constants::warning))
   {
      va_list ap;
      va_start(ap, message);
      cout << "W4RNING : " << fPrint(fPrint(constants::log_format, file, line, func, message.c_str()),ap) << endl;
      va_end(ap);
   }
}

#define error(message, ...) if(constants::static_log_level <= 3) {_error(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, message, ## __VA_ARGS__);}

inline void _error(const char* file, const int line, const char* func, const string message, ...)
{
   if(constants::static_log_level <= 4 && shouldLog(file, line, func, constants::error))
   {
      va_list ap;
      va_start(ap, message);
      cout << "ERR0R : " << fPrint(fPrint(constants::log_format, file, line, func, message.c_str()),ap) << endl;
      va_end(ap);
   }
}
#endif
