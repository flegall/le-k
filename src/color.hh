#ifndef color_hh
#define color_hh

class Color
{
   public:
      static Color RGB(const float red, const float green, const float blue);
      static Color RGB(const float red, const float green, const float blue, const float alpha);
      static Color HSV(const float hue, const float saturation, const float value);
      static Color HSV(const float hue, const float saturation, const float value, const float alpha);

      const float red;
      const float green;
      const float blue;
      const float alpha;
      
   private:
      Color(const float red, const float green, const float blue, const float alpha);
};

void RGBtoHSV(const float r, const float g, const float b, float& h, float& s, float& v);
void HSVtoRGB(float& r, float& g, float& b, const float h, const float s, const float v);

#endif
