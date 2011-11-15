#include <constants.hh>
#include <commons.hh>

#include <iostream>

#include <SDL_ttf.h>

namespace constants
{
   // opengl log : submits opengl statements to the logging system if true
   const bool log_opengl (false);

   // catches all opengl errors
   const bool catch_opengl_errors (false);

   // log level : 1 : debug, 2 : info, 3 : warning, 4 : error.
   // static is the lowest level that can't be specialized
   const LogLevel static_log_level (info);
   // default is the level for all loggers that have no configuration
   const LogLevel default_log_level(info);
   
  // lists of specific loggers configurations an empty function matches the whole file
   const LC log_levels_config[] =
   {
      //LC("font.cc", "FontRendererGlyph::FontRendererGlyph", debug),
      //LC("font.cc", "FontRenderPlan FontRenderer::renderText", debug),
      //LC("main.cc", "void checkEquals", debug),
      //LC("opengl.cc", "void flLoadIdentity", debug),
      //LC("opengl.cc", "void flTexImage2D", debug),
   };

   const string log_format("%s:%d:\"%s\" :: %s");
   
   int demo_width (1024);
   int demo_height (768);
   bool demo_fullscreen(true);
   const bool showConsole(false);
   const int demo_antialias_on(0);
   const int demo_antialias_level(0);
   const string window_title("lobstre");
   const int demo_bpp (32);
   const int demo_red (8);
   const int demo_blue (8);
   const int demo_green (8);
   const int demo_alpha (8);
   const int demo_stencil (8);
   const int demo_depth (24);
   const int demo_dblBuf (1);
   const int demo_acc_red (16);
   const int demo_acc_green (16);
   const int demo_acc_blue (16);
   const int demo_acc_alpha (16);
   const int SDL_Mixer_Frequency (44100);
   const int SDL_Mixer_Channels (2);
   const int SDL_Mixer_Latency (4096);
   const int thread_delay_max_latency(250);
   const int forward_jump_time(10000);
   const double forward_jump_time_double(static_cast<double>(forward_jump_time));
   const string console_font_file("data/commons/Bitstream-Vera-Sans.ttf");
   const int console_font_size(12);
   const int console_font_style(TTF_STYLE_NORMAL);
   
   const int  __private_log_levels_config_count = sizeof(log_levels_config) / sizeof(LC);
   
   const map<string, map<string, LogLevel> > __private_log_configs = __Get_private_log_configs();
   
   const map<string, map<string, LogLevel> > __Get_private_log_configs()
   {
      map<string, map<string, LogLevel> > result;
      
      for(int i = 0; i < __private_log_levels_config_count; i++)
      {
	 LC config = log_levels_config[i];
	 if(result.find(config.file) == result.end())
	 {
	    result[config.file] = map<string, LogLevel>();
	 }
	 
	 map<string, LogLevel> subConfig = result[config.file];
	 
	 if(subConfig.find(config.function) == subConfig.end())
	 {
	    subConfig[config.function] = config.level;
	 }
	 else
	 {
	    const string message = fPrint("duplicate log level configuration : position: %d file:\"%s\" function:\"%s\"", i, config.file.c_str(), config.function.c_str());
	    cout << "ERROR : " << message << endl;
	    throw message;
	 }

	 result[config.file] = subConfig;
      }
	 
      return result;
   }

   LC::LC(const string file, const string function, const LogLevel level) : file(file), function(function), level(level)
   {
   }

   LC::LC() : file(string()), function(string()), level(nothing)
   {
   }
}
