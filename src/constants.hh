#ifndef constants_hh
#define constants_hh

#include <SDL.h>

using namespace std;
#include <string>
#include <map>

namespace constants
{
   extern const bool log_opengl;
   extern const bool catch_opengl_errors;

   // log levels
   enum LogLevel
   {
      debug = 1,
      info = 2,
      warning = 3,
      error = 4,
      nothing = 5,
   };

   // Log configuration for one file
   class LC
   {
      public:
	 // the first param of the LC constructor is the source file name, the second is the function beginning pattern (an empty one will match all functions of the file), the third one is the level
	 LC(const string file, const string function, const LogLevel level);
	 // mandatory constructor, otherwise, an empty array of LC (like log_levels_config) couldn't be allocated, if you know why, write an email to florent.legall / gmail.com
	 LC();
	 const string file;
	 const string function;
	 const LogLevel level;
   };
   
   // log level : 1 : debug, 2 : info, 3 : warning, 4 : error.
   // static is the lowest level that can't be specialized
   extern const LogLevel static_log_level;
   // default is the level for all loggers that have no configuration
   extern const LogLevel default_log_level;
 

   extern const LC log_levels_config[];

   extern const string log_format;
   
   extern int demo_width;
   extern int demo_height;
   extern bool demo_fullscreen;   
   extern const bool showConsole;
   extern const string window_title;
   extern const int demo_bpp;
   extern const int demo_red;
   extern const int demo_blue;
   extern const int demo_green;
   extern const int demo_alpha;
   extern const int demo_stencil;
   extern const int demo_depth;
   extern const int demo_dblBuf;
   extern const int demo_acc_red;
   extern const int demo_acc_green;
   extern const int demo_acc_blue;
   extern const int demo_acc_alpha;
   extern const int demo_antialias_on;
   extern const int demo_antialias_level;
   extern const int SDL_Mixer_Frequency;
   extern const int SDL_Mixer_Channels;
   extern const int SDL_Mixer_Latency;
   extern const int thread_delay_max_latency;
   extern const int forward_jump_time;
   extern const double forward_jump_time_double;
   extern const string console_font_file;
   extern const int console_font_size;
   extern const int console_font_style;

   const map<string, map<string, LogLevel> > __Get_private_log_configs();
   extern const int __private_log_levels_config_count;
   extern const map<string, map<string, LogLevel> > __private_log_configs;
}

// The script class name to load
#define SCRIPT_CLASS_NAME EmptyDemo

#endif
