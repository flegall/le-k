#ifndef actuator_hh
#define actuator_hh

#include <source.hh>

#include <boost.hh>

template<typename T>
class Actuator
{
   public:
      virtual shared_ptr< Source<T> > registerActuator() = 0;
      virtual ~Actuator()
      {
	 info("~Actuator()");
      };
};


#endif
