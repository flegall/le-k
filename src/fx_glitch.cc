#include <fx_glitch.hh>
#include <log.hh>

fx_glitch::fx_glitch(Demo* const demo,
		     vector<double> timings,
		     const double volume
   ) :
   AudioEffect(demo), 
   timings(timings),
   isPlaying(false),   
   playPosition(0),
   startLocation(0),
   volume(volume)
{
//   sample = Mix_LoadWAV("/usr/lib/openoffice/share/gallery/sounds/nature1.wav");
   sample = Mix_LoadWAV("data/EmptyDemo/glitch.wav");
   lenSample = sample->alen / 4;
   info("sample loaded allocated:%d size:%d volume:%d", sample->allocated, sample->alen, sample->volume);

   short* values = (short*) sample->abuf;
   const int realLenTwoChans = sample->alen / 2;
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
      //info("buffer[%d] : left : %lf right : %lf",i , sample.left, sample.right);
   }
}

void fx_glitch::process(const double ms, AudioSample& sample, const int frequency)
{
   
   if(hasEvents(ms) && !isPlaying)
   {
      //info("audio event:%lf",ms);
      startPlay(ms);
   }
   playSample(ms, sample);
}

void fx_glitch::playSample(const double ms, AudioSample& sample)
{
   if(isPlaying)
   {
      if(PLAY_SIZE == playPosition)
      {
	 stopPlay();
      }
      else
      {
	 AudioSample& source = getSample((playPosition + randPosition)%lenSample);
	 sample.left += volume * source.left;
	 sample.right += volume * source.right;
      }
      playPosition++;
   }
}

AudioSample& fx_glitch::getSample(const int position)
{
   return buffer[position];
}

void fx_glitch::startPlay(const double ms)
{
   isPlaying = true;
   playPosition = 0;
   randPosition = static_cast<int>(ms) % lenSample;
}

void fx_glitch::stopPlay()
{
   isPlaying = false;
   playPosition = 0;
}

bool fx_glitch::hasEvents(const double ms)
{
   const unsigned int size = timings.size();
   for(unsigned int i = startLocation; i < size; i++)
   {
      const double value = timings[i];
      if(lastTime < value && value <= ms)
      {
	 lastTime = ms;
	 startLocation = i;
	 return true;
      }
   }

   lastTime = ms;
   return false;
}

/*
void fx_glitch::notify(vector<double>& mail)
{

   for(vector<double>::const_iterator i = mail.begin(); i != mail.end(); i++)
   {
      const double& value = *i;
      if(timings.find(value) == timings.end())
      {
	 timings.insert(value);
      }
   }
}
*/

fx_glitch::~fx_glitch()
{
   info("");
   Mix_FreeChunk(sample);
}


