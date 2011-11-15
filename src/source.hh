#ifndef source_hh
#define source_hh

#include <log.hh>

template<typename T>
class Source
{
   public:
      virtual T getValue(const double ms) = 0;
      virtual bool hasValue(const double ms) = 0;
      
      virtual ~Source()
      {
	 info("~Source()");
      };
};

#endif
