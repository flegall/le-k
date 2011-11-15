class Demo;
#ifndef demo_hh
#define demo_hh
 
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
using namespace std;
#include <vector>

#include <boost.hh>

#include <script.hh>
#include <activator.hh>
#include <effect.hh>
#include <commons.hh>
#include <mailbox.hh>
#include <font.hh>

class Demo
{
   public:
      // the following methods are to be called from the main func.
      // Constructor called in the main func
      Demo(const int width, const int height, const int frequency, const int channels, const Uint16 audio_format, const string audio_format_str);
      // method called each frame during the main loop
      bool drawGraphics(const vector<SDL_Event>& events, const Uint32 time);
      // destructor called in the main func
      ~Demo();

      // the following methods are to be called from script classes.
      // 
      // sets the music location to play.
      void setMusicLocation(const string location);
      void setMusicVolume(const double volume);
      // sets the demo duration.
      void setDemoDuration(const double ms);
      // adds a visual activator.
      void addVideoActivator(shared_ptr<Activator> activator);
      // adds an audio activator.
      void addAudioActivator(shared_ptr<Activator> activator);
      // adds an audio notifier
      void addAudioNotifier(shared_ptr<Notifier> notifier);
      
   private:
      // the music frequency
      const int frequency;
      // the number of channels
      const int channels;
      // the audio format
      const Uint16 audio_format;
      // the audio format descriptor
      const string audio_format_str;
      // the demo script instance
      shared_ptr<Script> script;
      // location of the music
      string musicLocation;
      // music's volume
      double musicVolume;
      // music element
      Mix_Music* music;
      // duration of the demo (in ms)
      double demoDuration;
      // boolean indicating whether the first frame has happened or not.
      bool video_firstFrame;
      // time to substract from SDL time to obtain the position in the demo (in ms)
      int video_timeAdjust;
      // boolean indicating whether the console is active atm.
      bool video_console_active;
      // list of all keyboard events
      vector<SDL_KeyboardEvent> keyboard_events;
      // list of all mouse motion events
      vector<SDL_MouseMotionEvent> mouse_motion_events;
      // list of all mouse button events
      vector<SDL_MouseButtonEvent> mouse_button_events;
      // list of all video effects activators
      vector<shared_ptr<Activator> > videoActivators;
      // list of all audio effects activators
      vector<shared_ptr<Activator> > audioActivators;
      vector<shared_ptr<Notifier> > audioNotifiers;
      // This mailbox is for the communication from the video thread to the audio thread : notifying changes of the music position.
      MailBox<double> musicPositionChanges;
      // Font renderer used by the console.
      FontRenderer consoleFontRenderer;
      // last time reminder (for fps calculation)
      Uint32 lastTime;
      // frame count
      Uint32 frameCount;
      // fps
      double fps;
      
      // Called from the constructor, creates the script instance and runs its initDemo method
      void initDemo();
      // Called just before starting the visuals.
      void runBeforeFirstFrame();
      // Called from the draw graphics, performs events routing.
      void routeEvents(const vector<SDL_Event>& events);
      // Checks whether a file exists
      bool fileExists(const string location) const;
      // gets a list of all activated video effects
      vector<ActivatedEffect> getActivatedVideoEffects(const double timeDemoMs) const;
      // gets a list of all activated audio effets
      vector<ActivatedEffect> getActivatedAudioEffects(const double timeDemoMs) const;
      // gets a list of all activated effects from a list of activators
      vector<ActivatedEffect> getActivatedEffects(vector< shared_ptr<Activator> > activators, const double timeDemoMs) const;
      // process demo control events such as changing the music position, rewinding the demo..
      void processDemoControlEvents(double& timeDemoMs);
      // pushes the music backward.
      void backwardMusic(double& timeDemoMs);
      // pushes the music forward.
      void forwardMusic(double& timeDemoMs);
      // restarts playing the music.
      void restartMusic(double& timeDemoMs);
      // displays the console
      void displayConsole(const double timeDemoMs, const Uint32 time, const int glOpsCount);
      

      // high level audio callback
      void processAudio(vector<AudioSample>& buffer);
      // audio callback.
      void audioCallBack(Uint8*stream, int len);
      // static audio callback.
      static void staticAudioCallBack(void *udata, Uint8 *stream, int len);

      // boolean indicating whether the first frame has happened or not.
      bool audio_firstFrame;
      // audio time (in ms)
      double audio_time;
};


#endif
