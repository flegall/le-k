#include <log.hh>
#include <opengl.hh>
#include <texture.hh>

#include <GL/glew.h>

#include <algorithm>

Texture::Texture() : textureId(flGenTexture())
{
   info("textureId:%d",textureId);
}

Texture::~Texture()
{
   info("textureId:%d", textureId);
   flDeleteTexture(textureId);
}

TextureRectangle::TextureRectangle(const SDL_Surface* const surface) : Texture()
{
   const SDL_PixelFormat* const format = surface->format;
   if(32 != format->BitsPerPixel)
   {
      const string message = fPrint("Invalid texture format, expecting a 32 bits one, got a %d one", format->BitsPerPixel);
      error(message);
      throw message;
   }
   
   bind();
   flTexParameteri(getToken(), FL_TEXTURE_MAG_FILTER, FL_LINEAR);
   flTexParameteri(getToken(), FL_TEXTURE_MIN_FILTER, FL_LINEAR);
   flTexImage2D(getToken(), 0, FL_RGBA, surface->w, surface->h, 0, FL_RGBA, FL_UNSIGNED_BYTE, surface->pixels);
}

void TextureRectangle::bind()
{
   flBindTexture(getToken(), textureId);
}

TextureRectangle::~TextureRectangle()
{
   info("~TextureRectangle()");
}

void TextureRectangle::enable()
{   
   flEnable(getToken());
}

FLEnum TextureRectangle::getToken()
{
   if(GLEW_ARB_texture_rectangle)
   {
      return FL_TEXTURE_RECTANGLE_ARB;
   }
   else
      if(GLEW_EXT_texture_rectangle)
      {
	 return FL_TEXTURE_RECTANGLE_EXT;
      }
      else
	 if(GLEW_NV_texture_rectangle)
	 {
	    return FL_TEXTURE_RECTANGLE_NV;
	 }
	 else
	 {
	    throw string("unreachable code");
	 }
}

void TextureRectangle::disable()
{
   flDisable(getToken());
}

Texture1D::Texture1D(const shared_array<Uint8> line, const int width)
{
   info("");
   
   bind();
   flTexParameteri(FL_TEXTURE_1D, FL_TEXTURE_MAG_FILTER, FL_LINEAR);
   flTexParameteri(FL_TEXTURE_1D, FL_TEXTURE_MIN_FILTER, FL_LINEAR_MIPMAP_LINEAR);

   fluBuild1DMipmaps(FL_TEXTURE_1D, FL_RGBA, width, FL_RGBA, FL_UNSIGNED_BYTE, line.get());
}

Texture1D::~Texture1D()
{
   info("");
}

void Texture1D::bind()
{
   flBindTexture(FL_TEXTURE_1D, textureId);
}

void Texture1D::enable()
{
   flEnable(FL_TEXTURE_1D);
}

void Texture1D::disable()
{
   flDisable(FL_TEXTURE_1D);
}

Texture2D::Texture2D(map<int, shared_array<Uint8> > mipMaps) : Texture()
{
   bind();
   flTexParameteri(FL_TEXTURE_2D, FL_TEXTURE_MAG_FILTER, FL_LINEAR);
   flTexParameteri(FL_TEXTURE_2D, FL_TEXTURE_MIN_FILTER, FL_LINEAR_MIPMAP_LINEAR);

   vector<int> sizes;

   for(map<int, shared_array<Uint8> >::const_iterator i = mipMaps.begin(); i != mipMaps.end(); i++)
   {
      sizes.push_back((*i).first);
   }
   reverse(sizes.begin(), sizes.end());
   
   int count = 0;
   for(vector<int>::const_iterator i = sizes.begin(); i != sizes.end(); i++)
   {
      const shared_array<Uint8> array = mipMaps[*i];
      flTexImage2D(FL_TEXTURE_2D, count, FL_RGBA, *i, *i, 0, FL_RGBA, FL_UNSIGNED_BYTE, array.get());
      count++;
   }
}

void Texture2D::bind()
{
   flBindTexture(FL_TEXTURE_2D, textureId);
}

Texture2D::~Texture2D()
{
   info("~Texture2D()");
}

void Texture2D::enable()
{
   flEnable(FL_TEXTURE_2D);
}

void Texture2D::disable()
{
   flDisable(FL_TEXTURE_2D);
}
