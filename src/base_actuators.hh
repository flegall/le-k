#ifndef base_actuators_hh
#define base_actuators_hh

#include <actuator.hh>
#include <bezier.hh>
#include <boost.hh>
#include <color.hh>
#include <source.hh>

template<typename T>
class ConstantSource : public Source<T>
{
   public:
      ConstantSource(const T value) : value(value) {};
      T getValue(const double ms)
      {
	 return value;
      };
      bool hasValue(const double ms)
      {
	 return true;
      };
   private:
      const T value;
};
   
template<typename T>
class ConstantActuator : public Actuator<T>
{
   public:
      ConstantActuator(const T value) : value(value) {};
      shared_ptr< Source<T> > registerActuator()
      {
	 return shared_ptr< ConstantSource<T> >(new ConstantSource<T>(value));
      };
   private:
      const T value;
};

class BezierSource : public Source<double>
{
   public:
      BezierSource(Bezier1D bezier);
      BezierSource(Bezier1D bezier, const double offset);      
      double getValue(const double ms);
      bool hasValue(const double ms);
      ~BezierSource();
   private:
      Bezier1D bezier;
      double offset;
};

class BezierActuator : public Actuator<double>
{
   public:
      BezierActuator(Bezier1D bezier);
      BezierActuator(Bezier1D bezier, const double offset);
      shared_ptr< Source<double> > registerActuator();
      ~BezierActuator();
   private:
      Bezier1D bezier;
      double offset;
};

class ColorSource : public Source<Color>
{
   public:
      ColorSource(Bezier1D h, Bezier1D s, Bezier1D v);
      Color getValue(const double ms);
      bool hasValue(const double ms);
      ~ColorSource();
   private:
      Bezier1D h;
      Bezier1D s;
      Bezier1D v;
};

class ColorActuator : public Actuator<Color>
{
   public:
      ColorActuator(Bezier1D h, Bezier1D s, Bezier1D v);
      shared_ptr< Source<Color> > registerActuator();
      ~ColorActuator();
   private:
      Bezier1D h;
      Bezier1D s;
      Bezier1D v;
};

#endif
