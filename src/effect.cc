#include <effect.hh>
#include <log.hh>

Effect::Effect(Demo* const demo) : demo(demo)
{
}

Effect::~Effect()
{
   info("~Effect()");
}


VisualEffect::VisualEffect(Demo* const demo) : Effect(demo)
{
}

void VisualEffect::processVisualsFrame(const double ms)
{
   process(ms);
}

void VisualEffect::processAudioFrame(const double ms, AudioSample& sample, const int frequency)
{
}

VisualEffect::~VisualEffect()
{
   info("~VisualEffect()");
}

AudioEffect::AudioEffect(Demo* const demo) : Effect(demo)
{
}

void AudioEffect::processVisualsFrame(const double ms)
{
}

void AudioEffect::processAudioFrame(const double ms, AudioSample& sample, const int frequency)
{
   process(ms, sample, frequency);
}


AudioEffect::~AudioEffect()
{
   info("~AudioEffect()");
}
