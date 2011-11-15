#include <script.hh>
#include <log.hh>

Script::Script(Demo* const demo) : demo(demo)
{
}


Script::~Script()
{
   info("~Script()");
}
