#ifndef fx_glitch_hh
#define fx_glitch_hh

#include <commons.hh>
#include <effect.hh>
#include <mailbox.hh>

#include <SDL_mixer.h>

#include <vector>
using namespace std;

class fx_glitch : public AudioEffect
{
   public:
      fx_glitch(Demo* const demo,
		vector<double> timings,
		const double volume);
      void process(const double ms,
		   AudioSample& sample,
		   const int frequency);
      ~fx_glitch();
      
   private:
      bool hasEvents(const double ms);
      void playSample(const double ms,
		      AudioSample& sample);
      void startPlay(const double ms);
      void stopPlay();
      AudioSample& getSample(const int position);
      
      Mix_Chunk* sample;
      double lastTime;
      vector<double> timings;
      bool isPlaying;
      int playPosition;
      int lenSample;
      int randPosition;
      int startLocation;
      vector<AudioSample> buffer;

      const double volume;

      static const int PLAY_SIZE = 1500;
};

#endif
