#ifndef _2d_renderer_hh
#define _2d_renderer_hh

#include <boost.hh>

class PointPosition
{
   public:
      PointPosition(const double x, const double y, const tuple<double, double> xAxis, const tuple<double,double> yAxis);
      double x;
      double y;
      tuple<double, double> xAxis;
      tuple<double, double> yAxis;

      void scale(const double factor);
      void rotate(const double angle);
      void flip(const double angle);
      void translateX(const double distance);
      void translateY(const double distance);

      tuple<double, double> transform(const tuple<double, double>& position) const;
      tuple<double, double> transform(const double x, const double y) const;
};

class _2D_Renderer
{
   public:
      // should be called before submitting geometry
      virtual void begin() = 0;
      // should be called when submitting geometry
      virtual void render(const double size) = 0;
      // should be called after submitting geometry
      virtual void end() = 0;
   
      virtual ~_2D_Renderer();
};

#endif
