#include <boost.hh>
#include <log.hh>
#include <opengl.hh>
#include <rd_point.hh>

#include <math.h>

using namespace std;
#include <map>
#include <vector>


int PointRenderer::instanceCount = 0;
Texture2D* PointRenderer::texture = NULL;
const double radius_param = 0.85;

PointRenderer::PointRenderer()
{
   newInstanceProcess();
   info("");
}

void PointRenderer::begin()
{   
   Texture2D::enable();
   texture->bind();
   
   flColor4d(0.0, 0.0, 0.0, 1.0);
   flEnable(FL_BLEND);
   flBlendFunc(FL_SRC_ALPHA, FL_ONE_MINUS_SRC_ALPHA);
}

void PointRenderer::render(const double size)
{  
//   const double half = size / 2.0;
   const double half = size / (radius_param * 2.0);

   const double xMin = -half;
   const double xMax = +half;
   const double yMin = -half;
   const double yMax = +half;
   
   flBegin(FL_QUADS);
   {
      flTexCoord2d(0.0, 1.0);
      flVertex2d(xMin, yMin);
      
      flTexCoord2d(1.0, 1.0);
      flVertex2d(xMax, yMin);
   
      flTexCoord2d(1.0, 0.0);
      flVertex2d(xMax, yMax);
      
      flTexCoord2d(0.0, 0.0);
      flVertex2d(xMin, yMax);
   }
   flEnd();  
}

void PointRenderer::end()
{  
   flDisable(FL_BLEND);
   
   Texture2D::disable();
}

PointRenderer::~PointRenderer()
{
   lostInstanceProcess();
   info("");
}

void PointRenderer::newInstanceProcess()
{
   if(0 == instanceCount)
   {
      buildTexture();
   }
   instanceCount++;
}

void PointRenderer::lostInstanceProcess()
{
   instanceCount--;
   if(0 == instanceCount)
   {
      destroyTexture();
   }
}

void PointRenderer::buildTexture()
{
   const int mipMaps[] =
   {
      512,
      256,
      128,
      64,
      32,
      16,
      8,
      4,
      2,
      1,
   };
   
   vector<int> mipMapsVector = buildVectorFromArray(mipMaps);

   map<int, shared_array<Uint8> > mips;
   
   for(vector<int>::const_iterator i = mipMapsVector.begin(); i !=  mipMapsVector.end(); i++)
   {
      const int sizeInt = *i;
      const double size = static_cast<double>(sizeInt);
      shared_array<Uint8> buffer(new Uint8[sizeInt*sizeInt*4]);
      const double middleX = size / 2.0;
      const double middleY = size / 2.0;
      const double radius = ((size / 2.0) * radius_param) + 0.5;

      int zeroCount = 0;
      int ffCount = 0;
      int middleCount = 0;
      
      for(int y = 0; y < sizeInt; y++)
      {
	 const int linePosition = y * sizeInt * 4;
	 for(int x = 0; x < sizeInt ; x++)
	 {
	    const double distance = getDistance(static_cast<double>(x) + 0.5, static_cast<double>(y) + 0.5, middleX, middleY);
	    const Uint8 value = getValue(distance, radius);
	    if(0 == value)
	    {
	       zeroCount++;
	    }
	    else if(255 == value)
	    {
	       ffCount++;
	    }
	    else
	    {
	       middleCount++;
	    }

	    if(1 == sizeInt)
	    {
	       debug("value:%d x:%d y:%d", value, x, y);
	    }
	    
	    buffer[linePosition + (x*4)] = 255;
	    buffer[linePosition + (x*4) + 1] = 255;
	    buffer[linePosition + (x*4) + 2] = 255;
	    buffer[linePosition + (x*4) + 3] = value;
	 }
      }
      info("for size : %d 0:%d ff:%d mid:%d", sizeInt, zeroCount, ffCount, middleCount);
      mips[sizeInt] = buffer;
   }

   texture = new Texture2D(mips);
}

const Uint8 PointRenderer::getValue(const double distance, const double radius)
{
   if(radius < 1.0)
   {
      if(distance >= radius)
      {
	 return 0;
      }
      else
      {
	 const double ratio = distance / radius;
	 const double result = 1.0 - ratio;
	 const double result256 = result * 255.0;
	 return static_cast<Uint8>(result256);
      }
   }
   else
   {
      if(distance < (radius - 1.0))
      {
	 return 255;
      }
      else if (distance < radius)
      {
	 const double result = radius - distance;
	 assert(result >= 0.0 && result <= 1.0);
	 const double result256 = result * 255.0;
	 return static_cast<Uint8>(result256);
      }
      else
      {
	 return 0;
      }
   }
}

const double PointRenderer::getDistance(const double x, const double y, const double middleX, const double middleY)
{
   const double diffX = x - middleX;
   const double diffY = y - middleY;

   const double diffXSquare = diffX * diffX;
   const double diffYSquare = diffY * diffY;

   const double sum = diffXSquare + diffYSquare;
   const double distance = sqrt(static_cast<double>(sum));
   
   return distance;
}


void PointRenderer::destroyTexture()
{
   delete texture;
}
