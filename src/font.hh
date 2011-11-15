#ifndef font_hh
#define font_hh

#include <SDL_ttf.h>

#include <commons.hh>
#include <boost.hh>
#include <texture.hh>

#include <string>
#include <vector>
#include <map>
using namespace std;


class FontGlyphMetrics
{
   public:
      FontGlyphMetrics();
      FontGlyphMetrics(TTF_Font *font, const char c);
      
      int xMin;
      int xMax;
      int yMin;
      int yMax;
      int advance;
};

class FontRenderSquare
{
   public:
      FontRenderSquare(const double xStart, const double xEnd, const double yStart, const double yEnd);
      const double xStart;
      const double xEnd;
      const double yStart;
      const double yEnd;
};

class FontRenderConstraints
{
   public:
      enum PositionXConstraint {XLeft, XCenter, XRight};
      enum PositionYConstraint {YTop, YBottom, YCenter};
      FontRenderConstraints(const PositionXConstraint xConstraint, const PositionYConstraint yConstraint, const FontRenderSquare renderSquare);
      const FontRenderSquare renderSquare;
      const PositionYConstraint yConstraint;
      const PositionXConstraint xConstraint;
      const double getWidth() const;
      const double getHeight() const;
};

class LinePlanX
{
   public:
      LinePlanX(const string line, const vector<FontGlyphMetrics> glyphMetrics, const double xStart, const double xEnd);
      const string line;
      const vector<FontGlyphMetrics> glyphMetrics;
      const double xStart;
      const double xEnd;
      LinePlanX operator = (const LinePlanX& plan) const;
};

class LinePlan : public LinePlanX
{
   public:
      LinePlan(const string line, const vector<FontGlyphMetrics> glyphMetrics, const double xStart, const double xEnd, const double yStart, const double yEnd);
      LinePlan(const LinePlanX& planX, const double yStart, const double yEnd);
      const double yStart;
      const double yEnd;
      LinePlan operator = (const LinePlan& plan) const;
};

class FontRenderPlan : public FontRenderSquare
{
   public:
      FontRenderPlan(const double xStart, const double xEnd, const double yStart, const double yEnd, const vector<LinePlan> linesToRender, const FontRenderConstraints constraints);
      const vector<LinePlan> linesToRender;
      const FontRenderConstraints constraints;
};

class FontRendererGlyph
{
   public:
      FontRendererGlyph(const char c, TTF_Font* const font);
      ~FontRendererGlyph();
      const shared_ptr<TextureRectangle> getTexture() const;
      const int getWidth() const;
      const int getHeight() const;
   private:
      shared_ptr<TextureRectangle> texture;
      const char c;
      int width;
      int height;
};

class FontRenderer
{
   public:
      FontRenderer(const string fontLocation, const int ptSize, const int fontStyle);
      FontRenderPlan getRenderDimensions(const string& text, const FontRenderConstraints& constraints);
      FontRenderPlan renderText(const string& text, const FontRenderConstraints& constraints);
      const string fontLocation;
      const int ptSize;
      TTF_Font* font;
      ~FontRenderer();

   private:
      int lineSkip;
      int fontDescent;
      int fontAscent;
      int fontHeight;
      int fontStyle;
      map<char, FontGlyphMetrics> metrics;
      map<char, shared_ptr <FontRendererGlyph> > glyphs;
      const FontGlyphMetrics& getMetrics(const char c);
      shared_ptr<FontRendererGlyph> getGlyph(const char c);
      vector<string> splitLines(const string& text) const;
      double getXStart(const FontRenderConstraints& constraints, const int advance) const;
      double getXEnd(const FontRenderConstraints& constraints, const int advance) const;
      double getYStart(const FontRenderConstraints& constraints, const int currentLineNumber, const int lineCount) const;
      double getYEnd(const FontRenderConstraints& constraints, const int currentLineNumber, const int lineCount) const;
};

#endif
