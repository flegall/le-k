#include <boost.hh>
#include <commons.hh>
#include <font.hh>
#include <log.hh>
#include <opengl.hh>

#include <math.h>

#include <list>

FontGlyphMetrics::FontGlyphMetrics()
{
}


FontGlyphMetrics::FontGlyphMetrics(TTF_Font *font, const char c)
{
   if(-1 == TTF_GlyphMetrics(font,static_cast<Uint16>(c),&xMin,&xMax,&yMin,&yMax,&advance))
   {  
      string err0R = fPrint("%s\n",TTF_GetError());
      throw err0R;
   }
}

FontRenderSquare::FontRenderSquare(const double xStart, const double xEnd, const double yStart, const double yEnd): xStart(xStart), xEnd(xEnd), yStart(yStart), yEnd(yEnd) 
{
}

FontRenderPlan::FontRenderPlan(const double xStart, const double xEnd, const double yStart, const double yEnd, const vector<LinePlan> linesToRender, const FontRenderConstraints constraints) : FontRenderSquare(xStart, xEnd, yStart, yEnd), linesToRender(linesToRender), constraints(constraints)
{
}

LinePlanX::LinePlanX(const string line, const vector<FontGlyphMetrics> glyphMetrics, const double xStart, const double xEnd) : line(line), glyphMetrics(glyphMetrics), xStart(xStart), xEnd(xEnd)
{
}

LinePlanX LinePlanX::operator = (const LinePlanX& plan) const
{
   return LinePlanX(plan.line, plan.glyphMetrics, plan.xStart, plan.xEnd);
}

LinePlan::LinePlan(const string line, const vector<FontGlyphMetrics> glyphMetrics, const double xStart, const double xEnd, const double yStart, const double yEnd) : LinePlanX(line, glyphMetrics, xStart, xEnd), yStart(yStart), yEnd(yEnd)
{
}

LinePlan::LinePlan(const LinePlanX& planX, const double yStart, const double yEnd) : LinePlanX(planX.line, planX.glyphMetrics, planX.xStart, planX.xEnd), yStart(yStart), yEnd(yEnd)
{
}

LinePlan LinePlan::operator = (const LinePlan& plan) const
{
   return LinePlan(plan.line, plan.glyphMetrics, plan.xStart, plan.xEnd, plan.yStart, plan.yEnd);
}


FontRenderConstraints::FontRenderConstraints(const PositionXConstraint xConstraint, const PositionYConstraint yConstraint, const FontRenderSquare renderSquare) : renderSquare(renderSquare), yConstraint(yConstraint), xConstraint(xConstraint)
{
}

const double FontRenderConstraints::getWidth() const
{
   return renderSquare.xEnd - renderSquare.xStart;
}

const double FontRenderConstraints::getHeight() const
{
   return renderSquare.yEnd - renderSquare.yStart;
}

FontRenderer::FontRenderer(const string fontLocation, const int ptSize, const int fontStyle) : fontLocation(fontLocation), ptSize(ptSize), fontStyle(fontStyle)
{
   font = TTF_OpenFont(fontLocation.c_str(), ptSize);
   if(NULL == font)
   {
      string message = fPrint("Couldn't open the font file : %s", fontLocation.c_str());
      error(message);
      string err0R = fPrint("%s\n",TTF_GetError());
      error(err0R);
      throw message;
   }
   fontHeight = TTF_FontHeight(font);
   fontAscent = TTF_FontAscent(font);
   fontDescent = TTF_FontDescent(font);
   lineSkip = TTF_FontLineSkip(font);
   
   info("Building new FontRenderer for font : \"%s\" : ptSize: %d, height : %d, line-skip : %d, ascent : %d, descent : %d", fontLocation.c_str(), ptSize, fontHeight, lineSkip, fontAscent, fontDescent);
}

FontRenderer::~FontRenderer()
{
   TTF_CloseFont(font);
   info("~FontRenderer()\nDeleting FontRenderer for font : \"%s\" : ptSize: %d, height : %d, line-skip : %d, ascent : %d, descent : %d", fontLocation.c_str(), ptSize, fontHeight, lineSkip, fontAscent, fontDescent);
}

vector<string> FontRenderer::splitLines(const string& text) const
{
   vector<string> result;
   
   typedef tokenizer<char_separator<char> > tokenizer;
   char_separator<char> sep("\n");
   tokenizer tokens(text, sep);

   for (tokenizer::iterator tok_iter = tokens.begin();
	tok_iter != tokens.end(); ++tok_iter)
   {
      const string token = *tok_iter;
      result.push_back(token);
      debug("TOKEN: %s", token.c_str());
   }
   debug("TOKEN");

   return result;
}

shared_ptr<FontRendererGlyph> FontRenderer::getGlyph(const char c)
{
   if(glyphs.find(c) == glyphs.end())
   {
      shared_ptr<FontRendererGlyph> ptr(new FontRendererGlyph(c, font));
      glyphs[c] = ptr;
   }
   return glyphs[c];
}

const FontGlyphMetrics& FontRenderer::getMetrics(const char c)
{
   if(metrics.find(c) == metrics.end())
   {
      metrics[c] = FontGlyphMetrics(font, c);
   }
   return metrics[c];
}



FontRenderPlan FontRenderer::getRenderDimensions(const string& text, const FontRenderConstraints& constraints)
{
   vector<string> lines = splitLines(text);
   int lineCount = 0;
   const double lineWidth = constraints.getWidth();
   const double textHeight = constraints.getHeight();
   double maxWidth = 0.0;


   vector<LinePlanX> linePlansToRender;
   
   for(vector<string>::const_iterator i = lines.begin(); i != lines.end(); i++)
   {
      string lineToRender;
      vector<FontGlyphMetrics> metricsList;
      lineCount++;

      const string line = *i;
      int pixelCount = 0;
      for(string::const_iterator j = line.begin(); j != line.end(); j++)
      {
	 const FontGlyphMetrics metrics = getMetrics(*j);
	 if(pixelCount + metrics.advance > lineWidth)
	 {
	    LinePlanX linePlan(lineToRender, metricsList, getXStart(constraints, metrics.advance), getXEnd(constraints, metrics.advance));
	    linePlansToRender.push_back(linePlan);
	    
	    lineToRender.clear();
	    metricsList.clear();
	    
	    lineToRender.push_back(*j);
	    metricsList.push_back(metrics);
	    pixelCount = metrics.advance;
	    lineCount++;
	 }
	 else
	 {
	    pixelCount += metrics.advance;
	    lineToRender.push_back(*j);
	    metricsList.push_back(metrics);
	 }
      }
      
      LinePlanX linePlan(lineToRender, metricsList, getXStart(constraints, pixelCount), getXEnd(constraints, pixelCount));
      linePlansToRender.push_back(linePlan);
      
      if(static_cast<double>(pixelCount) > maxWidth)
      {
	 maxWidth = static_cast<double>(pixelCount);
      }
   }

   const double width = min<double>(maxWidth, lineWidth);
   const double height = static_cast<double> (lineCount * lineSkip);

   debug("width:%f height:%f", width, height);

   const double diffWidth = lineWidth - width;
   const double diffHeight = textHeight - height;

   double xStart = constraints.renderSquare.xStart;
   double xEnd = constraints.renderSquare.xEnd;
   double yStart = constraints.renderSquare.yStart;
   double yEnd = constraints.renderSquare.yEnd;

   if(FontRenderConstraints::XLeft == constraints.xConstraint)
   {
      xEnd -= diffWidth;
   }

   if(FontRenderConstraints::XRight == constraints.xConstraint)
   {
      xStart += diffWidth;
   }

   if(FontRenderConstraints::XCenter == constraints.xConstraint)
   {
      xEnd -= diffWidth / 2.0;
      xStart += diffWidth / 2.0;
   }

   if(FontRenderConstraints::YBottom == constraints.yConstraint)
   {
      yEnd -= diffHeight;
   }

   if(FontRenderConstraints::YTop == constraints.yConstraint)
   {
      yStart += diffHeight;
   }

   if(FontRenderConstraints::YCenter == constraints.yConstraint)
   {
      yStart += diffHeight / 2.0;
      yEnd -= diffHeight / 2.0;
   }

   debug("x:%f:%f y:%f:%f", xStart, xEnd, yStart, yEnd);

   vector<LinePlan> linePlansToRenderResult;
   
   lineCount = 0;
   const int renderedLineCount = linePlansToRender.size();
   for(vector<LinePlanX>::const_iterator i = linePlansToRender.begin(); i !=linePlansToRender.end() ; i++)
   {
      const LinePlan linePlan(*i, getYStart(constraints, lineCount, renderedLineCount), getYEnd(constraints, lineCount, renderedLineCount));
      linePlansToRenderResult.push_back(linePlan);
      lineCount++;
   }
   
   return FontRenderPlan(xStart, xEnd, yStart, yEnd, linePlansToRenderResult, constraints);
}

double FontRenderer::getXStart(const FontRenderConstraints& constraints, const int advance) const
{
   if(constraints.xConstraint == FontRenderConstraints::XLeft)
   {
      return constraints.renderSquare.xStart;
   }
   if(constraints.xConstraint == FontRenderConstraints::XRight)
   {
      return constraints.renderSquare.xEnd - static_cast<double>(advance);
   }
   if(constraints.xConstraint == FontRenderConstraints::XCenter)
   {
      return (constraints.renderSquare.xEnd + constraints.renderSquare.xStart - static_cast<double>(advance)) / 2.0;
   }
   throw "Unreachable code";
}

double FontRenderer::getXEnd(const FontRenderConstraints& constraints, const int advance) const
{
   if(constraints.xConstraint == FontRenderConstraints::XLeft)
   {
      return double(constraints.renderSquare.xStart + static_cast<double>(advance));
   }
   if(constraints.xConstraint == FontRenderConstraints::XRight)
   {
      return constraints.renderSquare.xEnd;
   }
   if(constraints.xConstraint == FontRenderConstraints::XCenter)
   {
      return (constraints.renderSquare.xEnd + constraints.renderSquare.xStart + static_cast<double>(advance)) / 2.0;
   }
   throw "Unreachable code";
}

double FontRenderer::getYStart(const FontRenderConstraints& constraints, const int currentLineNumber, const int lineCount) const
{
   return getYEnd(constraints, currentLineNumber, lineCount) - lineSkip;
}


double FontRenderer::getYEnd(const FontRenderConstraints& constraints, const int currentLineNumber, const int lineCount) const
{
   if(constraints.yConstraint == FontRenderConstraints::YTop)
   {
      return constraints.renderSquare.yEnd - (static_cast<double>(currentLineNumber) * static_cast<double>(lineSkip));
   }
   
   if(constraints.yConstraint == FontRenderConstraints::YCenter)
   {
      return ((
		 constraints.renderSquare.yEnd + constraints.renderSquare.yStart
	       + static_cast<double>(lineCount) * static_cast<double>(lineSkip)
		  ) / 2.0)
	      - (static_cast<double>(currentLineNumber) * static_cast<double>(lineSkip));
   }
   
   if(constraints.yConstraint == FontRenderConstraints::YBottom)
   {
      return constraints.renderSquare.yStart + (static_cast<double>(lineCount - currentLineNumber) * static_cast<double>(lineSkip));
   }
   
   throw "Unreachable code";
}


FontRenderPlan FontRenderer::renderText(const string& text, const FontRenderConstraints& constraints)
{
   const FontRenderPlan plan = getRenderDimensions(text, constraints);
   debug("plan for text : %s\ncoords x:%f:%f y:%f:%f", text.c_str(), plan.xStart, plan.xEnd, plan.yStart, plan.yEnd);

   TextureRectangle::enable();
   
   flShadeModel(FL_FLAT);
   flEnable(FL_BLEND);
   flBlendFunc(FL_ONE, FL_ONE);

   for(vector<LinePlan>::const_iterator i = plan.linesToRender.begin(); i != plan.linesToRender.end(); i++)
   {
      const LinePlan linePlan = *i;
      debug("line : %s x:%f:%f y:%f:%f", linePlan.line.c_str(), linePlan.xStart, linePlan.xEnd, linePlan.yStart, linePlan.yEnd);
      const double yStart = linePlan.yStart;
      const double yEnd = linePlan.yEnd;
      double xStart = linePlan.xStart;
      double xEnd;

      const double yZero = yEnd - static_cast<double>(fontAscent);

      debug("yStart:%f yEnd:%f yZero:%f", yStart, yEnd, yZero);
      
      for(unsigned int j = 0; j < linePlan.line.size(); j++)
      {
	 const char c = linePlan.line[j];
	 
	 const FontGlyphMetrics metrics = linePlan.glyphMetrics[j];
	 const double advance = static_cast<double>(metrics.advance);
	 
	 if(' ' != c)
	 {
	    xEnd = xStart + advance;
	    
	    const shared_ptr<FontRendererGlyph> glyph = getGlyph(c);
	    const shared_ptr<TextureRectangle> texture = glyph->getTexture();
	       
	    const double xMin = static_cast<double>(metrics.xMin);
	    const double xMax = static_cast<double>(metrics.xMax);
	    const double yMin = static_cast<double>(metrics.yMin);
	    const double yMax = static_cast<double>(metrics.yMax);
	 
	    const double width = static_cast<double>(glyph->getWidth());
	    const double height = static_cast<double>(glyph->getHeight());

	    const double yBottom = yStart + yMin;
	    const double yTop = yStart + yMin + height;	 
	    const double xLeft = xStart + xMin;
	    const double xRight = xStart + xMin + width;
	 
	    debug("char : \'%c\' size : %f", c, advance);
	    debug("position: x:%f:%f y:%f:%f", xStart, xEnd, yStart, yEnd);
	    debug("metrics: x:%f:%f y:%f:%f", xMin, xMax, yMin, yMax);
	    debug("rendu x:%f:%f y:%f:%f", xLeft, xRight, yBottom, yTop);

	    texture->bind();
	 
	    flColor4d(1.0, 1.0, 1.0, 1.0);

	    flBegin(FL_QUADS);
	    {
	       flTexCoord2d(0.0, height);
	       flVertex2d(xLeft, yBottom);

	       flTexCoord2d(width, height);
	       flVertex2d(xRight, yBottom);

	       flTexCoord2d(width, 0.0);
	       flVertex2d(xRight, yTop);

	       flTexCoord2d(0.0, 0.0);
	       flVertex2d(xLeft, yTop);
	    }
	    flEnd();
	 }
	 
	 xStart += advance;
      }
   }

   flDisable(FL_BLEND);
   
   TextureRectangle::disable();

   return plan;
}

FontRendererGlyph::FontRendererGlyph(const char c, TTF_Font* const font) : c(c)
{
   info("FontRendererGlyph() : %c", c);
   const SDL_Color color = {255, 255, 255};

   SDL_Surface* temp = TTF_RenderGlyph_Blended(font, static_cast<Uint16>(c), color);
   SDL_Surface* surface = convertSurfaceToLocalByteOrder(temp);

   texture = shared_ptr<TextureRectangle>(new TextureRectangle(surface));

   width = surface->w;
   height = surface->h;
   
   SDL_FreeSurface(temp);
   SDL_FreeSurface(surface);
}

const shared_ptr<TextureRectangle> FontRendererGlyph::getTexture() const
{
   return texture;
}

const int FontRendererGlyph::getWidth() const
{
   return width;
}

const int FontRendererGlyph::getHeight() const
{
   return height;
}

FontRendererGlyph::~FontRendererGlyph()
{
   info("~FontRendererGlyph() : %c", c);
}
