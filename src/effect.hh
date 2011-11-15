class Effect;
#ifndef effect_hh
#define effect_hh

#include <demo.hh>
#include <commons.hh>

#include <vector>
using namespace std;

// Base class for effects
class Effect
{
   public:
      // The constructor only takes the demo engine instance
      Effect(Demo* const demo);
      // Processes visuals for one frame
      virtual void processVisualsFrame(const double ms) = 0;
      // Processes audio for one frame
      virtual void processAudioFrame(const double ms, AudioSample& sample, const int frequency) = 0;
      // Virtual destructor
      virtual ~Effect();
   protected:
      Demo* const demo;
};

// Base class for visual effects
class VisualEffect : public Effect
{
   public:
      // Constructor
      VisualEffect(Demo* const demo);
      // Redirects to the implementation
      void processVisualsFrame(const double ms);
      // Empty implementation
      void processAudioFrame(const double ms, AudioSample& sample, const int frequency);
      // Virtual method that should be overridden by the concrete class.
      virtual void process(const double ms) = 0;
      // Destructor
      virtual ~VisualEffect();
};

// Base class for audio effects
class AudioEffect : public Effect
{
   public:
      // Constructor
      AudioEffect(Demo* const demo);
      // Empty implementation
      void processVisualsFrame(const double ms);
      // Redirects to the implementation
      void processAudioFrame(const double ms, AudioSample& sample, const int frequency);   
      // Virtual method that should be overridden by the concrete class.
      virtual void process(const double ms, AudioSample& sample, const int frequency) = 0;
      // Destructor
      virtual ~AudioEffect();
};

#endif
