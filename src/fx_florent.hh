#ifndef fx_florent_hh
#define fx_florent_hh

#include <effect.hh>
#include <actuator.hh>

class fx_florent : public VisualEffect
{
   public:
      fx_florent(Demo* const demo, shared_ptr<Actuator<double> > param);

      void process(const double ms);

      ~fx_florent();

   private:
      FontRenderer fontRenderer;
      shared_ptr<Source<double> > source;
};

#endif
