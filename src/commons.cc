#include <commons.hh>

#include <stdlib.h>    	       	    
#include <string>
using namespace std;

#include <stdio.h>
#include <stdarg.h>

// stolen from : http://trac.videolan.org/vlc/browser/trunk/src/extras/libc.c?rev=6299
// because win32 is too dumb and doesn't have (hallo) vasprintf
int vlc_vasprintf(char **strp, const char *fmt, va_list ap) 
{ 
/* Guess we need no more than 100 bytes. */ 
   int     i_size = 100; 
   char    *p = (char*) malloc( i_size * sizeof (char)); 
   int     n; 
 		 
   if( p == NULL ) 
   { 
      *strp = NULL; 
      return -1; 
   } 
 		 
   for( ;; ) 
   { 
      /* Try to print in the allocated space. */ 
      n = vsnprintf( p, i_size, fmt, ap ); 
 		 
      /* If that worked, return the string. */ 
      if (n > -1 && n < i_size) 
      { 
	 *strp = p; 
	 return strlen( p ); 
      } 
      /* Else try again with more space. */ 
      if (n > -1)    /* glibc 2.1 */ 
      { 
	 i_size = n+1; /* precisely what is needed */
      } 
      else           /* glibc 2.0 */ 
      { 
	 i_size *= 2;  /* twice the old size */
      } 
      if( (p = (char *) realloc( p, i_size * sizeof (char)) ) == NULL) 
      { 
	 *strp = NULL; 
	 return -1; 
      } 
   } 
} 

const string fPrint(const string base, ...)
{
   va_list ap;
   va_start(ap, base);

   const string message(fPrint(base, ap));
   
   va_end(ap);
   return message;
}

const string fPrint(const string base, va_list ap)
{
   char* messageChars;
   vlc_vasprintf(&messageChars, base.c_str(), ap);
   const string message(messageChars);
   free(messageChars);

   return message;
}

SDL_Surface* convertSurfaceToLocalByteOrder(SDL_Surface* const surface)
{
   /* Create the target surface with correct color component ordering */
   SDL_Surface* result = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w,
				   surface->h, 32 /* bits */,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN // OpenGL RGBA masks 
				   0x000000FF, 
				   0x0000FF00, 
				   0x00FF0000, 
				   0xFF000000
#else
				   0xFF000000,
				   0x00FF0000, 
				   0x0000FF00, 
				   0x000000FF
#endif
      );

   // copy the surface
   SDL_BlitSurface(surface, NULL, result, NULL);
   return result;
}
