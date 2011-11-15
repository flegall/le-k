#include <activator.hh>

#include <log.hh>

ActivatedEffect::ActivatedEffect(shared_ptr<Effect> effect, const double position) : effect(effect), position(position)
{
}

bool ActivatedEffect::operator < (const ActivatedEffect& right) const
{
   return position < right.position;
}

Activator::~Activator()
{
   info("~Activator()");
}

vector<ActivatedEffect> TimeSegmentsActivator::getActiveEffects(const double ms) const
{
   vector<ActivatedEffect> effects;
   for(vector<TimeSegmentsEffectHolder>::const_iterator i = holders.begin(); i != holders.end(); i++)
   {
      const TimeSegmentsEffectHolder holder = *i;
      if(holder.msBegin <= ms && ms < holder.msEnd)
      {
	 const ActivatedEffect effect(holder);
	 effects.push_back(effect);
      }
   }
   return effects;
}

TimeSegmentsEffectHolder::TimeSegmentsEffectHolder(shared_ptr<Effect> effect, const double msBegin, const double msEnd, const double position) : ActivatedEffect(effect, position), msBegin(msBegin), msEnd(msEnd)
{
}

void TimeSegmentsActivator::registerEffect(shared_ptr<Effect> effect, const double position, const double msBegin, const double msEnd)
{
   const TimeSegmentsEffectHolder holder(effect, msBegin, msEnd, position);
   holders.push_back(holder);
}

vector<ActivatedEffect> ConstantActivator::getActiveEffects(const double ms) const
{
   return holders;
}

void ConstantActivator::registerEffect(shared_ptr<Effect> effect, const double position)
{
   const ActivatedEffect holder(effect, position);
   holders.push_back(holder);
}
