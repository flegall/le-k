class Script;
#ifndef script_hh
#define script_hh

#include <demo.hh>

class Script
{
   public:
      // The constructor only takes the demo engine instance.
      Script(Demo* const demo);
      // The initDemo method is supposed to init the demo engine.
      virtual void initDemo() = 0;
      // this destructor might be usefull
      virtual ~Script();
   protected:
      Demo* const demo;
};


#endif
