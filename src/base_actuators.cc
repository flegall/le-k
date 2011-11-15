#include <base_actuators.hh>
#include <log.hh>


BezierActuator::BezierActuator(Bezier1D bezier) :
   bezier(bezier),
   offset(0.0)
{
   info("");
   info("offset:%lf", offset);
}

BezierActuator::BezierActuator(Bezier1D bezier, const double offset) :
   bezier(bezier),
   offset(offset)
{
   info("offset:%lf", offset);
}

BezierActuator::~BezierActuator()
{
   info("");
}

shared_ptr<Source<double> > BezierActuator::registerActuator()
{
   return shared_ptr<Source<double> >(new BezierSource(bezier, offset));
};

BezierSource::BezierSource(Bezier1D bezier) :
   bezier(bezier),
   offset(0.0)
{
   info("offset:%lf", offset);
}

BezierSource::BezierSource(Bezier1D bezier, const double offset) :
   bezier(bezier),
   offset(offset)
{
   info("");
   info("offset:%lf", offset);
}


BezierSource::~BezierSource()
{
   info("");
}


double BezierSource::getValue(const double ms)
{
   return bezier.getValue(ms) + offset;
}

bool BezierSource::hasValue(const double ms)
{
   return true;
}

ColorSource::ColorSource(Bezier1D h, Bezier1D s, Bezier1D v) :
   h(h),
   s(s),
   v(v)
{
   info("");
}

Color ColorSource::getValue(const double ms)
{
   return Color::HSV(h.getValue(ms), s.getValue(ms), v.getValue(ms));
}

bool ColorSource::hasValue(const double ms)
{
   return true;
}

ColorSource::~ColorSource()
{
   info("");
}

ColorActuator::ColorActuator(Bezier1D h, Bezier1D s, Bezier1D v) :
   h(h),
   s(s),
   v(v)
{
   info("");
}

shared_ptr< Source<Color> > ColorActuator::registerActuator()
{
   return shared_ptr<Source<Color> >(new ColorSource(h, s, v));
}

ColorActuator::~ColorActuator()
{
   info("");
}
