class Activator;
class ActivatedEffect;

#ifndef activator_hh
#define activator_hh

#include <vector>

#include <effect.hh>
#include <boost.hh>

// Class holding a pointer to an effect to activate and holding position indicator, effects are activated following the position's order
class ActivatedEffect
{
   public:
      // Builds an ActivatedEffect instance
      ActivatedEffect(shared_ptr<Effect> effect, const double position);
      shared_ptr<Effect> effect;
      double position;
      // Comparison operator used to sort collections of ActivatedEffect.
      bool operator < (const ActivatedEffect& right) const;
};

// Activator interface
class Activator
{
   public:
      // method returning a vector of ActivatedEffect that are to be displayed at this precise moment
      virtual vector<ActivatedEffect> getActiveEffects(double ms) const = 0;
      // destructor
      virtual ~Activator ();
};


class TimeSegmentsEffectHolder : public ActivatedEffect
{
   public:
      TimeSegmentsEffectHolder(shared_ptr<Effect> effect, const double msBegin, const double msEnd, const double position);
      double msBegin;
      double msEnd;
};

// Simplest implementation
// Activates effects between time segments
class TimeSegmentsActivator : public Activator
{
   public:
      // implementation
      vector<ActivatedEffect> getActiveEffects(const double ms) const;
      // register an effect to be activated with a certain position where msBeing <= currentTimeMs <= msEnd
      void registerEffect(shared_ptr<Effect> effect, const double position, const double msBegin, const double msEnd);
   private:
      vector<TimeSegmentsEffectHolder> holders;
};

// Even simplest implementation
// Activates effects constantly
class ConstantActivator : public Activator
{
   public:
      // implementation
      vector<ActivatedEffect> getActiveEffects(const double ms) const;
      // register an effect to be activated constantly
      void registerEffect(shared_ptr<Effect> effect, const double position);
   private:
      vector<ActivatedEffect> holders;
};


#endif
