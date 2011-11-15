#ifndef opengl_hh
#define opengl_hh

#include <boost.hh>

#include <string>
#include <vector>
#include <stack>
using namespace std;

typedef unsigned long FLbitfield;
typedef unsigned long FLEnum;

// Indicates the buffers currently enabled for color writing.
extern const FLbitfield FL_COLOR_BUFFER_BIT;
// Indicates the depth buffer.
extern const FLbitfield FL_DEPTH_BUFFER_BIT;
// Indicates the accumulation buffer.
extern const FLbitfield FL_ACCUM_BUFFER_BIT;
// Indicates the stencil buffer.
extern const FLbitfield FL_STENCIL_BUFFER_BIT;
// GL_ALL_ATTRIBS_BIT
extern const FLbitfield FL_ALL_ATTRIB_BITS;

// GL_TEXTURE_1D
extern const FLEnum FL_TEXTURE_1D;
// GL_TEXTURE_2D
extern const FLEnum FL_TEXTURE_2D;
// GL_TEXTURE_RECTANGLE_ARB
extern const FLEnum FL_TEXTURE_RECTANGLE_ARB;
// GL_TEXTURE_RECTANGLE_EXT
extern const FLEnum FL_TEXTURE_RECTANGLE_EXT;
// GL_TEXTURE_RECTANGLE_NV
extern const FLEnum FL_TEXTURE_RECTANGLE_NV;
// GL_TEXTURE_MAG_FILTER
extern const FLEnum FL_TEXTURE_MAG_FILTER;
// GL_TEXTURE_MIN_FILTER
extern const FLEnum FL_TEXTURE_MIN_FILTER;
// GL_PROJECTION
extern const FLEnum FL_PROJECTION;
// GL_MODELVIEW
extern const FLEnum FL_MODELVIEW;
// GL_QUADS
extern const FLEnum FL_QUADS;
// GL_POLYGON
extern const FLEnum FL_POLYGON;
// GL_FLAT
extern const FLEnum FL_FLAT;
// GL_BLENDING
extern const FLEnum FL_BLEND;
// GL_SRC_ALPHA
extern const FLEnum FL_SRC_ALPHA;
// GL_DST_ALPHA
extern const FLEnum FL_DST_ALPHA;
// GL_ONE_MINUS_SRC_ALPHA
extern const FLEnum FL_ONE_MINUS_SRC_ALPHA;
// GL_ONE_MINUS_DST_ALPHA
extern const FLEnum FL_ONE_MINUS_DST_ALPHA;
// GL_ONE
extern const FLEnum FL_ONE;
// GL_TEXTURE_ENV
extern const FLEnum FL_TEXTURE_ENV;
// GL_TEXTURE_ENV_MODE
extern const FLEnum FL_TEXTURE_ENV_MODE;
// GL_SRC_COLOR
extern const FLEnum FL_SRC_COLOR;
// GL_ONE_MINUS_SRC_COLOR
extern const FLEnum FL_ONE_MINUS_SRC_COLOR;
// GL_RGBA
extern const FLEnum FL_RGBA;
// GL_UNSIGNED_BYTE
extern const FLEnum FL_UNSIGNED_BYTE;


// GL_NEAREST
extern const int FL_NEAREST;
// GL_LINEAR
extern const int FL_LINEAR;
// GL_LINEAR_MIPMAP_LINEAR
extern const int FL_LINEAR_MIPMAP_LINEAR;
// GL_REPLACE
extern const int FL_REPLACE;



// glClear -- clear buffers within the viewport
void flClear(const FLbitfield mask);

//glClearColor -- specify clear values for the color buffers
void flClearColor(const float red,
                  const float green,
                  const float blue,
                  const float alpha);

// generates one texture id;
const unsigned long flGenTexture();

// generates many textures id;
vector<unsigned long> flGenTextures(const int howMany);

// deletes one texture
void flDeleteTexture(const unsigned long textureId);

void flDeleteTextures(const vector<unsigned long> textureIds);

// binds a texture glBindTexture
void flBindTexture(const FLEnum target, const unsigned long textureId);

// sets texture parameters
void flTexParameteri(const FLEnum target, const FLEnum pname, const int param);

// uploads texture data
void flTexImage2D(const FLEnum target,
		  const int level,
		  const int internalformat,
		  const int width,
		  const int height,
		  const int border,
		  const FLEnum format,
		  const FLEnum type,
		  const void* pixels);

// sets the matrix mode
void flMatrixMode(const FLEnum mode);

// resets the 2d mode
void flSet2DMode();

void flSet2DMode640();

// loads the identity matrix
void flLoadIdentity();

// multiply the current matrix with an orthographic matrix
void flOrtho(const double left, const double right, const double bottom, const double top, const double zNear, const double zFar);

// glBegin();
void flBegin(const FLEnum mode);

//glEnd();
void flEnd();

// geVertex2d();
void flVertex2d(const double x, const double y);

// glColor3d
void flColor3d(const double red, const double green, const double blue);

// glColor4d
void flColor4d(const double red, const double green, const double blue, const double alpha);

// glShadeModel
void flShadeModel(const FLEnum model);

// glEnable
void flEnable(const FLEnum cap);

//glDisable
void flDisable(const FLEnum cap);

// glBlendFunc
void flBlendFunc(const FLEnum src, const FLEnum dst);

// glTexCoord2d
void flTexCoord2d(const double s, const double t);

// glTexEnvi
void flTexEnvi(const FLEnum target,
	       const FLEnum pname,
	       const int param);

// glPushAttrib
void flPushAttrib(const FLbitfield mask);

// glPopAttrib
void flPopAttrib();

// returns the number of opengl operations since the last call to resetGlOpsCount()
int getGlOpsCount();

// resets the opengl operations counter
void resetGlOpsCount();

// glTranslate(x,y,0.0);
void flTranslate(const double x, const double y);

// glRotate(angle, 0.0, 0.0, 0.0);
void flRotate(const double angle);

// glScale(factor, factor, factor)
void flScale(const double factor);

// glScale(x, y, z)
void flScale(const double x, const double y, const double z);

// glLoadMatrix
void flLoadMatrix(shared_array<double> matrix);

// getGetDoublev(GL_MODELVIEW_MATRIX);
shared_array<double> flGetModelViewMatrix();

// gluBuild1DMipmaps
void fluBuild1DMipmaps(const FLEnum target, const int internalFormat, const int width, const FLEnum format, const FLEnum type, const void* texels);

// glTextCoord1d
void flTexCoord1d(const double coord);

class ModelViewStack
{
   public:
      void push();
      void pop();
      int size();
   private:
      stack<shared_array<double> > stack;
};

#endif
