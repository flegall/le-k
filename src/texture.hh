#ifndef texture_hh
#define texture_hh

#include <SDL.h>

#include <boost.hh>
#include <opengl.hh>

using namespace std;
#include <map>
#include <vector>

class Texture
{
   public:
      Texture();
      virtual void bind() = 0;
      virtual ~Texture();
   protected:
      const unsigned long textureId;
};

class TextureRectangle : public Texture
{
   public:
      TextureRectangle(const SDL_Surface* const surface);
      void bind();
      ~TextureRectangle();
      static void enable();
      static void disable();
   private:
      static FLEnum getToken();
};

class Texture1D : public Texture
{
   public:
      Texture1D(const shared_array<Uint8> line, int width);
      void bind();
      ~Texture1D();
      static void enable();
      static void disable();
};

class Texture2D : public Texture
{
   public:
      Texture2D(const map<int, shared_array<Uint8> > mipMaps);
      void bind();
      ~Texture2D();
      static void enable();
      static void disable();
};

#endif
