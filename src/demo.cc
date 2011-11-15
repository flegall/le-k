#include <SDL.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

#include <script.hh>
#include <demo.hh>
#include <log.hh>
#include <constants.hh>
#include <opengl.hh>
#include <demo_empty.hh>
#include <commons.hh>


Demo::Demo(const int width, const int height, const int frequency, const int channels, const Uint16 audio_format, const string audio_format_str) :
   frequency(frequency),
   channels(channels),
   audio_format(audio_format),
   audio_format_str(audio_format_str),
   musicLocation(""),
   musicVolume(0.5),
   music(NULL),
   demoDuration(0.0),
   video_firstFrame(true),
   video_console_active(false),
   consoleFontRenderer(constants::console_font_file, constants::console_font_size, constants::console_font_style),
   lastTime(0),
   frameCount(0),
   fps(0.0),
   audio_time(0.0)
{
   debug("Demo()");

   assert(2 == channels);
   assert(AUDIO_U16LSB == audio_format || AUDIO_S16LSB == audio_format || AUDIO_U16MSB == audio_format || AUDIO_S16MSB == audio_format);

   initDemo();

   debug("Demo() done");
}

void Demo::initDemo()
{
   // Creates the script instance and runs its initDemo method.
   debug("initDemo();");
   shared_ptr<Script> ptr(new SCRIPT_CLASS_NAME(this));
   script = ptr;
   script->initDemo();

   if(0.0 == demoDuration)
   {
      throw string("Set the demo duration please.");
   }

   // load audio
   // checks if the music location is filled
   if(musicLocation.empty())
   {
      info("No music location specified : the demo will be running without music");
   }
   else
   {
      info("Music location : %s", musicLocation.c_str());
		
      // checks if the file exists
      if(fileExists(musicLocation))
      {
	 info("Music location valid");
	 music = Mix_LoadMUS(musicLocation.c_str());
	 if(NULL == music)
	 {
	    string err0r = fPrint("Mix_LoadMusic: %s, tried loading %d", Mix_GetError(), musicLocation.c_str());
	    throw err0r;
	 }
      }
      else
      {
	 throw "Music location invalid : " + musicLocation;
      }
   }
}


bool Demo::fileExists(const string location) const
{
   bool result;
   fstream fin;

   fin.open(location.c_str(), ios::in);
   if( fin.is_open() )
   {
      result = true;
   }
   else
   {
      result = false;
   }
   fin.close();

   return result;
}

void Demo::runBeforeFirstFrame()
{
   // Sets the music volume :
   const int vol = static_cast<int>(musicVolume * static_cast<double>(MIX_MAX_VOLUME));
   if(-1 == Mix_VolumeMusic(vol))
   {
      string err0r = fPrint("Mix_VolumeMusic: tried setting the volume to  %d", vol);
      error(err0r);
      throw err0r;
   }
   
   // checks if the music location is filled
   if(musicLocation.empty())
   {
      // do nothing
      Mix_HookMusic(staticAudioCallBack, this);
   }
   else
   {
      if(-1 == Mix_PlayMusic(music, -1))
      {
	 string err0r = fPrint("Mix_PlayMusic: %s, tried loading %d", Mix_GetError(), musicLocation.c_str());
      error(err0r);	 
	 throw err0r;
      }
      else
      {
	 Mix_SetPostMix(staticAudioCallBack, this);
      }
   }
}

bool Demo::drawGraphics(const vector<SDL_Event>& events, const Uint32 time)
{
   // Gets the number of opengl operations;
   const int glOpsCount = getGlOpsCount();
   resetGlOpsCount();
   
   // preprocess first frame
   if(video_firstFrame)
   {
      runBeforeFirstFrame();
      video_timeAdjust = time;	
   }
   video_firstFrame = false;

   // route events
   routeEvents(events);

   const int timeDemo = time - video_timeAdjust;
   double timeDemoMs = static_cast<double>(timeDemo);

   processDemoControlEvents(timeDemoMs);

   if(demoDuration < timeDemoMs)
   {
      return false;
   }
   
   debug("Time : %f", timeDemoMs);
   
   vector<ActivatedEffect> effectsList = getActivatedVideoEffects(timeDemoMs);

   for(vector<ActivatedEffect>::const_iterator i = effectsList.begin(); i != effectsList.end(); i++)
   {
      const ActivatedEffect effect = *i;
      shared_ptr<Effect> fx = effect.effect;
      fx->processVisualsFrame(timeDemoMs);
   }

   if(video_console_active || constants::showConsole)
   {
      displayConsole(timeDemoMs, time, glOpsCount);
   }

   return true;
}

void Demo::displayConsole(const double timeDemoMs, const Uint32 time, const int glOpsCount)
{
   flSet2DMode();
   FontRenderSquare square(7.0, static_cast<double>(constants::demo_width), 0.0, static_cast<double>(constants::demo_height)-1.0);
   FontRenderConstraints constraints(FontRenderConstraints::XLeft, FontRenderConstraints::YTop, square);

   const Uint32 elapsedTime = time - lastTime;
   frameCount++;
   if(elapsedTime > 1000)
   {
      fps = (1000.0 * static_cast<double>(frameCount)) / static_cast<double>(elapsedTime);
      lastTime = time;
      frameCount = 0;
   }
   
   const string text = fPrint("FPS : %.2lf       GL calls : %d\nTime : %.2lf", fps, glOpsCount,timeDemoMs / 1000.0);

   FontRenderPlan plan = consoleFontRenderer.getRenderDimensions(text, constraints);

   flShadeModel(FL_FLAT);
   flEnable(FL_BLEND);
   flBlendFunc(FL_SRC_ALPHA, FL_ONE_MINUS_SRC_ALPHA);
   flColor4d(0.0, 0.0, 0.0, 0.8);
   
   const double top = static_cast<double>(constants::demo_height);
   const double right = static_cast<double>(constants::demo_width);
   const double hauteur = 45;
   
   flBegin(FL_QUADS);
   {
      flVertex2d(0.0, top - hauteur);
      flVertex2d(right, top - hauteur);
      flVertex2d(right, top);
      flVertex2d(0.0, top);
   }
   flEnd();
   flDisable(FL_BLEND);
   
   consoleFontRenderer.renderText(text, constraints);
}

void Demo::backwardMusic(double& timeDemoMs)
{
   // calculates the new time
   const double actualTime = timeDemoMs;
   const double requestedTime = actualTime - constants::forward_jump_time_double;
   const double realisticTime = requestedTime >= 0.0 ? requestedTime : 0.0;
   const double timeCommandSec = realisticTime / 1000.0;

   // Modify the time
   timeDemoMs = realisticTime;
   // Notifies the audio thread.
   musicPositionChanges.post(realisticTime);

   // restart the audio thread
   Mix_PauseMusic();
   SDL_Delay(constants::thread_delay_max_latency);
   Mix_RewindMusic();
   Mix_SetMusicPosition(timeCommandSec);
   Mix_ResumeMusic();

   // adjusts the time of the video thread
   video_timeAdjust += min<int>(constants::forward_jump_time, static_cast<int>(actualTime)) + constants::thread_delay_max_latency;
}

void Demo::forwardMusic(double& timeDemoMs)
{
   // calculates the new time
   const double actualTime = timeDemoMs;
   const double requestedTime = actualTime + constants::forward_jump_time_double;
   const double realisticTime = requestedTime >= 0.0 ? requestedTime : 0.0;
   const double timeCommandSec = realisticTime / 1000.0;

   // Modify the time
   timeDemoMs = realisticTime;
   // Notifies the audio thread.
   musicPositionChanges.post(realisticTime);

   // restart the audio thread
   Mix_PauseMusic();
   SDL_Delay(constants::thread_delay_max_latency);
   Mix_RewindMusic();
   Mix_SetMusicPosition(timeCommandSec);
   Mix_ResumeMusic();

   // adjusts the time of the video thread
   video_timeAdjust -= constants::forward_jump_time + constants::thread_delay_max_latency;
}

void Demo::restartMusic(double& timeDemoMs)
{
   // delays the video_timeAdjust thing
   video_timeAdjust += static_cast<int>(timeDemoMs);
   // resets the actual time
   timeDemoMs = 0.0;
   // notifies the audio thread
   musicPositionChanges.post(0.0);

   // restart the audio thread
   Mix_PauseMusic();
   SDL_Delay(constants::thread_delay_max_latency);
   Mix_RewindMusic();
   Mix_ResumeMusic();

   // adjusts the time of the video thread
   video_timeAdjust += constants::thread_delay_max_latency;
}

void Demo::processDemoControlEvents(double& timeDemoMs)
{
   for(vector<SDL_KeyboardEvent>::const_iterator i = keyboard_events.begin(); i != keyboard_events.end(); i++)
   {
      const SDL_KeyboardEvent event = *i;
      if(event.type == SDL_KEYDOWN && event.keysym.sym == SDLK_LEFT && (event.keysym.mod == KMOD_RCTRL || event.keysym.mod == KMOD_LCTRL))
      {
	 debug("backwardMusic:%f",timeDemoMs);
	 backwardMusic(timeDemoMs);
	 debug("backwardMusic(done):%f",timeDemoMs);
      }
      if(event.type == SDL_KEYDOWN && event.keysym.sym == SDLK_RIGHT && (event.keysym.mod == KMOD_RCTRL || event.keysym.mod == KMOD_LCTRL))
      {
	 debug("forwardMusic:%f",timeDemoMs);
	 forwardMusic(timeDemoMs);
	 debug("forwardMusic(done):%f",timeDemoMs);
      }
      if(event.type == SDL_KEYDOWN && event.keysym.sym == SDLK_HOME && (event.keysym.mod == KMOD_RCTRL || event.keysym.mod == KMOD_LCTRL))
      {
	 debug("restartMusic:%f",timeDemoMs);
	 restartMusic(timeDemoMs);
	 debug("restartMusic(done):%f",timeDemoMs);	 
      }
      if(event.type == SDL_KEYDOWN && event.keysym.sym == SDLK_TAB)
      {
	 debug("TAB DOWN");
	 video_console_active = true;
      }
      if(event.type == SDL_KEYUP && event.keysym.sym == SDLK_TAB)
      {
	 debug("TAB UP");
	 video_console_active = false;
      }
   }
}

vector<ActivatedEffect> Demo::getActivatedEffects(vector< shared_ptr<Activator> > activators, const double timeDemoMs) const
{
   vector<ActivatedEffect> effectsList;
   for(vector< shared_ptr<Activator> >::const_iterator i = activators.begin(); i != activators.end(); i++)
   {
      const shared_ptr<Activator> activator = *i;
      const vector<ActivatedEffect> effects = activator->getActiveEffects(timeDemoMs);
      for(vector<ActivatedEffect>::const_iterator j = effects.begin(); j != effects.end(); j++)
      {
	 const ActivatedEffect effect = *j;
	 effectsList.push_back(effect);
      }
   }
   sort(effectsList.begin(), effectsList.end());
   return effectsList;
}

vector<ActivatedEffect> Demo::getActivatedAudioEffects(const double timeDemoMs) const
{
   vector<ActivatedEffect> effects = getActivatedEffects(audioActivators, timeDemoMs);
   return effects;
}

vector<ActivatedEffect> Demo::getActivatedVideoEffects(const double timeDemoMs) const
{
   vector<ActivatedEffect> effects = getActivatedEffects(videoActivators, timeDemoMs);
   return effects;
}

void Demo::addVideoActivator(shared_ptr<Activator> activator)
{
   videoActivators.push_back(activator);
}

void Demo::addAudioActivator(shared_ptr<Activator> activator)
{
   audioActivators.push_back(activator);
}

void Demo::addAudioNotifier(shared_ptr<Notifier> notifier)
{
   audioNotifiers.push_back(notifier);
}


Demo::~Demo()
{
   debug("~Demo()");
   if(NULL != music)
   {
      Mix_FreeMusic(music);
      music = NULL;
   }
   debug("~Demo() done");
}

void Demo::setMusicLocation(const string location)
{
   musicLocation = location;
}

void Demo::setMusicVolume(const double volume)
{
   musicVolume = volume;
}

void Demo::setDemoDuration(const double ms)
{
   demoDuration = ms;
}

void Demo::routeEvents(const vector<SDL_Event>& events)
{
   // clear all previous events: events are just presented once!
   keyboard_events.clear();
   mouse_motion_events.clear();
   mouse_button_events.clear();
   
   
   //vector<SDL_Event>::const_iterator i;
   for(vector<SDL_Event>::const_iterator i = events.begin(); i != events.end(); i++)
   {
      const SDL_Event event = *i;
      switch(event.type)
      {
	 case SDL_KEYUP:
	 case SDL_KEYDOWN:
	 {
	    keyboard_events.push_back(event.key);
	    break;
	 }
	 case SDL_MOUSEMOTION:
	 {
	    mouse_motion_events.push_back(event.motion);
	    break;
	 }
	 case SDL_MOUSEBUTTONDOWN:
	 case SDL_MOUSEBUTTONUP:
	 {		
	    mouse_button_events.push_back(event.button);
	    break;
	 }
	 default:
	 {
	    break;
	 }	      
      }
   } 
}

void Demo::processAudio(vector<AudioSample>& buffer)
{
   if(audio_firstFrame)
   {
      audio_firstFrame = false;
      info("Audio thread started, first buffer of size : %d", buffer.size());
   }

   const vector<double> mail = musicPositionChanges.fetchMail();
   for(vector<double>::const_iterator i = mail.begin(); i != mail.end(); i++)
   {
      const double value = *i;
      info("audio thread : new time value : %f", value);
      audio_time = value;
   }

   const double initTime = audio_time;

   for(vector<shared_ptr<Notifier> >::const_iterator i = audioNotifiers.begin(); i != audioNotifiers.end(); i++)
   {
      const shared_ptr<Notifier> notifier = *i;
      notifier->notify();
   }

   vector<ActivatedEffect> effectsList = getActivatedAudioEffects(audio_time);
   for(vector<ActivatedEffect>::const_iterator i = effectsList.begin(); i != effectsList.end(); i++)
   {
      int frameCount = 0;
      const ActivatedEffect effect = *i;
      for(vector<AudioSample>::iterator j = buffer.begin(); j != buffer.end(); j++)
      {
	 const double time = initTime + (static_cast<double>(frameCount) * (1000.0 / static_cast<double>(constants::SDL_Mixer_Frequency)));
	 effect.effect->processAudioFrame(time, *j, constants::SDL_Mixer_Frequency);
	 frameCount++;
      }
   }
   audio_time += static_cast<double>(buffer.size()) * (1000.0 / static_cast<double>(constants::SDL_Mixer_Frequency));
   
   debug("audio_time:%f", audio_time);
}

void Demo::audioCallBack(Uint8 *stream, int len)
{
   short* values = (short*) stream;
   const int realLenTwoChans = len / 2;
   vector<AudioSample> buffer;
   buffer.reserve(realLenTwoChans / 2);
   AudioSample sample;
   for(int i=0; i<realLenTwoChans; i++)
   {
      const short value = values[i];
      const double dblValue = static_cast<double>(value) / 32768.0;
      
      if(0 == i%2)
      {
	 sample.left = dblValue;
      }
      else
      {
	 sample.right = dblValue;
	 buffer.push_back(sample);
      }
   }

   processAudio(buffer);
	
   bool didClip = false;
   int count = 0;
   for(vector<AudioSample>::const_iterator i = buffer.begin(); i != buffer.end(); i++)
   {
      sample = *i;
      double leftMax = sample.left * 32768.0;
      double rightMax = sample.right * 32768.0;

      const long clipLeft = abs(static_cast<long>(leftMax));
      const long clipRight = abs(static_cast<long>(rightMax));

      if(clipLeft > 32767)
      {
	 didClip = true;
	 leftMax = leftMax > 0 ? 32767.0 : -32767.0;
      }

      if(clipRight > 32767)
      {
	 didClip = true;
	 rightMax = rightMax > 0 ? 32767.0 : -32767.0;
      }

      const short left = static_cast<short> (leftMax);
      const short right = static_cast<short> (rightMax);
      
      values[count] = left;    
      values[count+1] = right;
      count+=2;
   }
   if(didClip)
   {
      warning("music processing clipped!!");
   }
};

void Demo::staticAudioCallBack(void* const udata, Uint8* const stream, const int len)
{
   Demo* const demo = reinterpret_cast<Demo* const>(udata);
   try
   {
      demo->audioCallBack(stream, len);
   }
   catch(const string message)
   {
      Mix_HaltMusic();
      Mix_CloseAudio();
      TTF_Quit();
      error("Exception in audio thread: " + message);    
      throw message;
   }
   catch(...)
   {
      Mix_HaltMusic();
      Mix_CloseAudio();
      TTF_Quit();
      error("Unknow error in audio thread");    
      throw;
   }
   
}
