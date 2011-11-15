#include <stdlib.h>    	       	    
#include <SDL.h>
#include <iostream>
#include <memory>
using namespace std;
#include <vector>

#include <commons.hh>
#include <demo.hh>
#include <log.hh>

#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <GL/glew.h>

void checkEquals (const string text, const int requested, const int got)
{
   if(got != requested)
   {
      warning("%s : %d : got %d", text.c_str(), requested, got);
   }
   else
   {
      debug("%s : %d : got %d", text.c_str(), requested, got);
   }
}

const bool checkGlExtensions()
{
   bool ok = true;
   
   if(GLEW_ARB_texture_rectangle||GLEW_EXT_texture_rectangle||GLEW_NV_texture_rectangle)
   {
      info("ARB_texture_rectangle or EXT_texture_rectangle or NV_texture_rectangle : OK");
   }
   else
   {
      error("ARB_texture_rectangle or EXT_texture_rectangle or NV_texture_rectangle : NOT PRESENT");
      ok = false;
   }

   return ok;
}

// Entry point
int main (const int argc, char *argv[]) 
{
   for(int i = 1; i < argc; i++)
   {
      const string param(argv[i]);
      if("-fullscreen" == param)
      {
	 constants::demo_fullscreen = true;
      }
      if("-windowed" == param)
      {
	 constants::demo_fullscreen = false;
      }
      if("-640" == param)
      {
	 constants::demo_width = 640;
	 constants::demo_height = 480;	 
      }
      if("-800" == param)
      {
	 constants::demo_width = 800;
	 constants::demo_height = 600;	 
      }
      if("-1024" == param)
      {
	 constants::demo_width = 1024;
	 constants::demo_height = 768;	 
      }
      if("-1280" == param)
      {
	 constants::demo_width = 1280;
	 constants::demo_height = 1024;	 
      }
      if("-1600" == param)
      {
	 constants::demo_width = 1600;
	 constants::demo_height = 1200;	 
      }            
   }
   // Init SDL
   if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) < 0 )    
   {
      fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
      return 1;
   }
   // quit sdl properly at exit
   atexit(SDL_Quit);

   const int redRequest = constants::demo_red;
   int redGot;
   const int greenRequest = constants::demo_green;
   int greenGot;
   const int blueRequest = constants::demo_blue;
   int blueGot;
   const int alphaRequest = constants::demo_alpha;
   int alphaGot;
   const int stencilRequest = constants::demo_stencil;
   int stencilGot;
   const int depthRequest = constants::demo_depth;
   int depthGot;
   const int dblBufRequest = constants::demo_dblBuf;
   int dblBufGot;
   const int accumRedRequest = constants::demo_acc_red;
   int accumRedGot;
   const int accumGreenRequest = constants::demo_acc_green;
   int accumGreenGot;
   const int accumBlueRequest = constants::demo_acc_blue;
   int accumBlueGot;
   const int accumAlphaRequest = constants::demo_acc_alpha;
   int accumAlphaGot;
   const int multisampleEnabledRequest = constants::demo_antialias_on;
   int multisampleEnabledGot; 
   const int multisamplesRequest = constants::demo_antialias_level;
   int multisamplesGot;

   // requests nice colors
   SDL_GL_SetAttribute( SDL_GL_RED_SIZE, redRequest );
   SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, greenRequest);
   SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, blueRequest);
   SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, alphaRequest );
   SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, stencilRequest );
   SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, depthRequest );
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, dblBufRequest );
   SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, accumRedRequest);
   SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, accumGreenRequest);
   SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, accumBlueRequest);
   SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, accumAlphaRequest);
   SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, multisampleEnabledRequest);
   SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, multisamplesRequest);

   Uint32 flags = constants::demo_fullscreen ? SDL_OPENGL|SDL_FULLSCREEN : SDL_OPENGL;
   

   // opens the window/screen
   //const SDL_Surface* screen = SDL_SetVideoMode(demo_width, demo_height, 32, SDL_OPENGL|SDL_FULLSCREEN);
   const SDL_Surface* screen = SDL_SetVideoMode(constants::demo_width, constants::demo_height, constants::demo_bpp, flags);
   if ( ! screen ) 
   {
      fprintf(stderr, "Couldn't set GL video mode: %s\n", SDL_GetError());
      return 2;
   }

   // checks what opengl gave us.
   SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &redGot);
   SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &greenGot);
   SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &blueGot);
   SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &alphaGot);
   SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &stencilGot);
   SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depthGot);
   SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &dblBufGot);
   SDL_GL_GetAttribute(SDL_GL_ACCUM_RED_SIZE, &accumRedGot);
   SDL_GL_GetAttribute(SDL_GL_ACCUM_GREEN_SIZE, &accumGreenGot);
   SDL_GL_GetAttribute(SDL_GL_ACCUM_BLUE_SIZE, &accumBlueGot);
   SDL_GL_GetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, &accumAlphaGot);
   SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &multisampleEnabledGot);
   SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &multisamplesGot);

   // checks if opengl was friendly and gave us our daily bread, doesn't fail if it didn't, just logs it.
   checkEquals( "Requested red size", redRequest, redGot);
   checkEquals( "Requested green size", greenRequest, greenGot);
   checkEquals( "Requested blue size", blueRequest, blueGot);
   checkEquals( "Requested alpha size", alphaRequest, alphaGot);
   checkEquals( "Requested stencil size", stencilRequest, stencilGot);
   checkEquals( "Requested depth size", depthRequest, depthGot);
   checkEquals( "Requested double buffer size", dblBufRequest, dblBufGot);
   checkEquals( "Requested accum red size", accumRedRequest, accumRedGot);
   checkEquals( "Requested accum green size", accumGreenRequest, accumGreenGot);
   checkEquals( "Requested accum blue size", accumBlueRequest, accumBlueGot);
   checkEquals( "Requested accum alpha size", accumAlphaRequest, accumAlphaGot);
   checkEquals( "Requested multisample enabled", multisampleEnabledRequest, multisampleEnabledGot);
   checkEquals( "Requested multisample", multisamplesRequest, multisamplesGot);
   
   const GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      /* Problem: glewInit failed, something is seriously wrong. */
      error("Couldn't init GLEW : %s", glewGetErrorString(err));
      return 3;
   }
   else
   {
      info("Using GLEW %s", glewGetString(GLEW_VERSION));
      if(checkGlExtensions())
      {
	 info("All required opengl extensions are here \\o/");
      }
      else
      {
	 error("Not all required opengl extensions are here");
	 return 6;
      }
   
      // Some sdl things
      SDL_WM_SetCaption(constants::window_title.c_str(), constants::window_title.c_str());
      SDL_ShowCursor(constants::demo_fullscreen ? SDL_DISABLE : SDL_ENABLE);

      {
	 SDL_version mixer_compile_version;
	 MIX_VERSION(&mixer_compile_version);
	 const SDL_version* mixer_link_version = Mix_Linked_Version();
	 info("compiled with SDL_mixer version: %d.%d.%d", 
	      mixer_compile_version.major,
	      mixer_compile_version.minor,
	      mixer_compile_version.patch);
	 info("running with SDL_mixer version: %d.%d.%d", 
	      mixer_link_version->major,
	      mixer_link_version->minor,
	      mixer_link_version->patch);
      }
      {
	 SDL_version ttf_compile_version;
	 TTF_VERSION(&ttf_compile_version);
	 info("compiled with SDL_ttf version: %d.%d.%d", 
	      ttf_compile_version.major,
	      ttf_compile_version.minor,
	      ttf_compile_version.patch);
	 const SDL_version* ttf_link_version=TTF_Linked_Version();
	 info("running with SDL_ttf version: %d.%d.%d", 
	      ttf_link_version->major,
	      ttf_link_version->minor,
	      ttf_link_version->patch);
      }

      if(TTF_Init()==-1) 
      {
	 error("TTF_Init: %s", TTF_GetError());
	 return 5;
      }

      if(-1 == Mix_OpenAudio(constants::SDL_Mixer_Frequency, MIX_DEFAULT_FORMAT, constants::SDL_Mixer_Channels, constants::SDL_Mixer_Latency))
      {
	 error("Mix_OpenAudio: %s", Mix_GetError());
	 return 4;
      }

      int frequency, channels;
      Uint16 format;
      const int numtimesopened = Mix_QuerySpec(&frequency, &format, &channels);
      string format_str;
      if(!numtimesopened)
      {
	 error("Mix_QuerySpec: %s\n",Mix_GetError());
	 return 4;
      }
      else
      {
	 switch(format)
	 {
	    case AUDIO_U8: format_str = "U8"; break;
	    case AUDIO_S8: format_str = "S8"; break;
	    case AUDIO_U16LSB: format_str = "U16LSB"; break;
	    case AUDIO_S16LSB: format_str = "S16LSB"; break;
	    case AUDIO_U16MSB: format_str = "U16MSB"; break;
	    case AUDIO_S16MSB: format_str = "S16MSB"; break;
	    default: format_str = "UNKNOWN"; break;
	 }
	 info("SDL_mixer opened : %d times  frequency : %d Hz  format : %s  channels : %d", numtimesopened, frequency, format_str.c_str(), channels);
      }

      	 
      // inits a demo object as a scoped pointer so the destructor
      // get called automatically when the scope of the pointer end..
      scoped_ptr<Demo> demo(new Demo(constants::demo_width, constants::demo_height, frequency, channels, format, format_str));
      // The core :
      try
      {
	 // bool a la con telling whether to loop or not.
	 bool loop = true;
	 
	 // actually loops
	 while(loop)
	 {
	    // an sdl event.
	    SDL_Event event;

	    vector<SDL_Event> events;
	    
	    // Exit Event
	    while ( SDL_PollEvent(&event) ) 
	    {
	       events.push_back(event);
	       switch(event.type) 
	       {
		  case SDL_QUIT:
		  {
		     loop = false;
		     break;
		  }
		  case SDL_KEYDOWN:
		  {
		     if(SDLK_ESCAPE == event.key.keysym.sym)
		     {
			loop = false;
			break;
		     }
		  }
	       }
	    }

	    if(true == loop)
	    {      
	       // time
	       const Uint32 time = SDL_GetTicks();
	       
	       // do graphics !
	       loop = demo->drawGraphics(events, time);
	       
	       // swap the gl buffers
	       SDL_GL_SwapBuffers();
	    }
	 }
      }

	 
      // i usually only throw string messages, but someday i might do a real exception class.
      catch(const string message)
      {
	 Mix_PauseMusic();
	 Mix_HaltMusic();
	 Mix_CloseAudio();
	 TTF_Quit();
	 error("Exception : " + message);
	 throw message;
      }
      catch(...)
      {
	 Mix_PauseMusic();
	 Mix_HaltMusic();
	 Mix_CloseAudio();
	 TTF_Quit();
	 error("Unknow error");
	 throw;
      }
      
      Mix_PauseMusic();
      Mix_HaltMusic();
      Mix_CloseAudio();
      TTF_Quit();
      
      
      // when things go well : quit the main function !
      return 0;
   }
}
