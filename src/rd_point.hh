#ifndef rd_point_hh
#define rd_point_hh

#include <2d_renderer.hh>
#include <boost.hh>
#include <texture.hh>

#include <SDL.h>

class PointRenderer : public _2D_Renderer
{
   public:
      PointRenderer();
      void begin();
      void render(const double size);
      void end();
      ~PointRenderer();
      
   private:
      static int instanceCount;
      static Texture2D* texture;
      
      static void newInstanceProcess();
      static void lostInstanceProcess();
      static void buildTexture();
      static void destroyTexture();
      static const double getDistance(const double x, const double y, const double middleX, const double middleY);
      static const Uint8 getValue(const double distance, const double radius);
};

#endif
