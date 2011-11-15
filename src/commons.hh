#ifndef commons_hh
#define commons_hh

#include <string>
#include <vector>
using namespace std;
#include <stdarg.h>

#include <SDL_ttf.h>

// Returns a const string printf formated using a base format template and variable arguments.
const string fPrint(const string base, ...);
// Returns a const string printf formated using a base format template and variable arguments list.
const string fPrint(const string base, va_list ap);

// Audio sample class
struct AudioSample
{
      double left;
      double right;
};

SDL_Surface* convertSurfaceToLocalByteOrder(SDL_Surface* const surface);

// Builds a vector from an array
// this function will only workd with an array explicitely allocated in the current
// scope (so that sizeof(array) returns the size of the array cause the compiler
// knows it.
#define buildVectorFromArray(array) _buildVectorFromArray(array, sizeof(array))

template<class T>
inline vector<T> _buildVectorFromArray(const T* array, const int size)
{
   vector<T> result;
   for(unsigned int i = 0; i < (size / sizeof(T)); i++)
   {
      result.push_back(array[i]);
   }
   return result;
};

template<class T>
inline const T get2DPos(const T x, const T y, const T lineWidth)
{
   return x + (y * lineWidth);
};

#endif
