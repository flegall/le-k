#ifndef fx_cadre_hh
#define fx_cadre_hh

#include <effect.hh>
#include <texture.hh>

class fx_cadre : public VisualEffect
{
   public:
      fx_cadre(Demo* const demo);
       void process(const double ms);
      ~fx_cadre();
   private:
      shared_ptr<Texture1D> texture;
};

#endif
