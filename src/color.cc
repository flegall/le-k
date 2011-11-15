#include <boost.hh>
#include <color.hh>

#include <math.h>

using namespace std;


Color Color::RGB(const float red, const float green, const float blue)
{
   return Color(red, green, blue, 1.0f);
}

Color Color::RGB(const float red, const float green, const float blue, const float alpha)
{
   return Color(red, green, blue, alpha);
}

Color Color::HSV(const float hue, const float saturation, const float value)
{
		  return HSV(hue, saturation, value, 1.0f);
}

Color Color::HSV(const float hue, const float saturation, const float value, const float alpha)
{
		  float r,g,b;
		  HSVtoRGB(r, g, b, hue, saturation, value);
		  return RGB(r, g, b, alpha);
}

Color::Color(const float red, const float green, const float blue, const float alpha) : red(red), green(green), blue(blue), alpha(alpha)
{
}


// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)

// stolen from http://www.cs.rit.edu/~ncs/color/t_convert.html
void RGBtoHSV(const float r, const float g, const float b, float& h, float& s, float& v)
{
		  float mini, maxi, delta;

		  mini = min<float>(min<float>(r, g), b);
		  maxi = max<float>(max<float>(r, g), b);
		  v = maxi;				// v

		  delta = maxi - mini;

		  if( maxi != 0 )
					 s = delta / maxi;		// s
		  else {
					  // r = g = b = 0		// s = 0, v is undefined	
					 s = 0;
					 h = -1;
					 return;
		  }	
		  if( r == maxi )
					 h = ( g - b ) / delta;		// between yellow & magenta
		  else if( g == maxi )
					 h = 2 + ( b - r ) / delta;	// between cyan & yellow
		  else
					 h = 4 + ( r - g ) / delta;	// between magenta & cyan

		  h *= 60;				// degrees
		  if( h < 0 )
					 h += 360;
}

// stolen from http://www.cs.rit.edu/~ncs/color/t_convert.html
void HSVtoRGB(float& r, float& g, float& b, const float hue, const float s, const float v)
{
		  float h = hue;
		  int i;	
		  float f, p, q, t;

		  if( s == 0 ) {
					 // achromatic (grey)	
					 r = g = b = v;
					 return;
		  }

		  h /= 60;			// sector 0 to 5
		  i = static_cast<int>(floor( h ));
		  f = h - i;			// factorial part of h
		  p = v * ( 1 - s );
		  q = v * ( 1 - s * f );
		  t = v * ( 1 - s * ( 1 - f ) );

		  switch( i ) {
					 case 0:
								r = v;
								g = t;
								b = p;
								break;
					 case 1:
								r = q;
								g = v;
								b = p;
								break;
					 case 2:
								r = p;
								g = v;
								b = t;
								break;
					 case 3:
								r = p;
								g = q;
								b = v;
								break;
					 case 4:
								r = t;
								g = p;
								b = v;
								break;
					 default:		// case 5:
								r = v;
								g = p;
								b = q;
								break;
		  }

}
