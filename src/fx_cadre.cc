#include <bezier.hh>
#include <boost.hh>
#include <commons.hh>
#include <fx_cadre.hh>
#include <log.hh>
#include <opengl.hh>

#include <SDL.h>

fx_cadre::fx_cadre(Demo* const demo) :
   VisualEffect(demo)
{
   info("");

   const int size = 2048;

   shared_array<Uint8> buffer(new Uint8[size * 4]);

   Bezier1DEasyConfig settings[] =
   {
      Bezier1DEasyConfig(0.0, 192.0, -0.8),
      Bezier1DEasyConfig(static_cast<double>(size), 0.0, 0.0),
   };

   vector<Bezier1DEasyConfig> configs = buildVectorFromArray(settings);
   Bezier1D bezier = Bezier1D(configs);
   for(int i = 0; i < size; i++)
   {
      const double t = static_cast<double>(i) + 0.5;
      const double value = bezier.getValue(t);

      if(value < 0.0 || value > 255.0)
      {
	 const string err0R = fPrint("value %lf out of [0 255] range");
	 error(err0R);
	 throw err0R;
      }

      buffer[i*4] = 255;
      buffer[i*4 + 1] = 255;
      buffer[i*4 + 2] = 255;
      buffer[i*4 + 3] = static_cast<Uint8>(value);
   }

   texture = shared_ptr<Texture1D>(new Texture1D(buffer, size));
}

void fx_cadre::process(const double ms)
{
   const double width = 20.0;
   
   Texture1D::enable();

   texture->bind();
   flSet2DMode640();

   flShadeModel(FL_FLAT);
   flEnable(FL_BLEND);
   flBlendFunc(FL_SRC_ALPHA, FL_ONE_MINUS_SRC_ALPHA);
   flColor4d(0.0, 0.0, 0.0, 1.0);

   flBegin(FL_QUADS);
   {
      flTexCoord1d(0.0);
      flVertex2d(0.0, 0.0);     
      flVertex2d(0.0, 480.0);
      flTexCoord1d(0.95);
      flVertex2d(width, 480.0);
      flVertex2d(width, 0.0);

      flVertex2d(640.0 - width, 480.0);
      flVertex2d(640.0 - width, 0.0);
      flTexCoord1d(0.0);
      flVertex2d(640.0, 0.0);   
      flVertex2d(640.0, 480.0);
   }

   flEnd();

   flDisable(FL_BLEND);
   
   Texture1D::disable();
}

fx_cadre::~fx_cadre()
{
   info("");
}
