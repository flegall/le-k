class EmptyDemo;
#ifndef empty_demo_hh
#define empty_demo_hh

#include <script.hh>

class EmptyDemo : public Script
{
   public:
      EmptyDemo(Demo* const demo);
      void initDemo();
};

#endif
