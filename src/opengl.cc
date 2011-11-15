#include <boost.hh>
#include <commons.hh>
#include <constants.hh>
#include <log.hh>
#include <opengl.hh>

#include <GL/glew.h>


const FLbitfield FL_COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT;
const FLbitfield FL_DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT;
const FLbitfield FL_ACCUM_BUFFER_BIT = GL_ACCUM_BUFFER_BIT;
const FLbitfield FL_STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT;
const FLbitfield FL_ALL_ATTRIB_BITS = GL_ALL_ATTRIB_BITS;

const FLEnum FL_TEXTURE_1D = GL_TEXTURE_1D;
const FLEnum FL_TEXTURE_2D = GL_TEXTURE_2D;
const FLEnum FL_TEXTURE_RECTANGLE_ARB = GL_TEXTURE_RECTANGLE_ARB;
const FLEnum FL_TEXTURE_RECTANGLE_EXT = GL_TEXTURE_RECTANGLE_EXT;
const FLEnum FL_TEXTURE_RECTANGLE_NV = GL_TEXTURE_RECTANGLE_NV;
const FLEnum FL_TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER;
const FLEnum FL_TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER;
const FLEnum FL_PROJECTION = GL_PROJECTION;
const FLEnum FL_MODELVIEW = GL_MODELVIEW;
const FLEnum FL_QUADS = GL_QUADS;
const FLEnum FL_POLYGON = GL_POLYGON;
const FLEnum FL_FLAT = GL_FLAT;
const FLEnum FL_BLEND = GL_BLEND;
const FLEnum FL_SRC_ALPHA = GL_SRC_ALPHA;
const FLEnum FL_DST_ALPHA = GL_DST_ALPHA;
const FLEnum FL_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA;
const FLEnum FL_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA;
const FLEnum FL_TEXTURE_ENV = GL_TEXTURE_ENV;
const FLEnum FL_TEXTURE_ENV_MODE = GL_TEXTURE_ENV_MODE;
const FLEnum FL_SRC_COLOR = GL_SRC_COLOR;
const FLEnum FL_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR;
const FLEnum FL_ONE = GL_ONE;
const FLEnum FL_RGBA = GL_RGBA;
const FLEnum FL_UNSIGNED_BYTE = GL_UNSIGNED_BYTE;

const int FL_LINEAR = GL_LINEAR;
const int FL_NEAREST = GL_NEAREST;
const int FL_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR;
const int FL_REPLACE = GL_REPLACE;

int __glOpsCount = 0;

inline void incrementGlOpsCount()
{
   __glOpsCount++;
}

int getGlOpsCount()
{
   return __glOpsCount;
}

void resetGlOpsCount()
{
   __glOpsCount = 0;
}

// Returns a textual represensation of a bit field
const string getBitField(const FLbitfield mask)
{
   if(constants::log_opengl)
   {
      switch(mask)
      {
	 case FL_COLOR_BUFFER_BIT: return string("GL_COLOR_BUFFER_BIT"); break;
	 case FL_DEPTH_BUFFER_BIT: return string("GL_DEPTH_BUFFER_BIT"); break;
	 case FL_ACCUM_BUFFER_BIT: return string("GL_ACCUM_BUFFER_BIT"); break;
	 case FL_STENCIL_BUFFER_BIT: return string("GL_STENCIL_BUFFER_BIT"); break;
	 case FL_ALL_ATTRIB_BITS: return string("GL_ALL_ATTRIB_BITS"); break;
	 
	 default: return string(fPrint("GLBitField : %d", mask)); break;
      }
   }
   else
   {
      return string("");
   }
}

// Returns a textual representation of an enum
const string getEnum(const FLEnum flEnum)
{
   if(constants::log_opengl)
   {
      switch(flEnum)
      {
	 case FL_TEXTURE_RECTANGLE_ARB: return string("GL_TEXTURE_RECTANGLE_ARB"); break;
	 case FL_TEXTURE_1D: return string("GL_TEXTURE_1D"); break;
	 case FL_TEXTURE_2D: return string("GL_TEXTURE_2D"); break;
	 case FL_TEXTURE_MAG_FILTER: return string("GL_TEXTURE_MAG_FILTER"); break;
	 case FL_TEXTURE_MIN_FILTER: return string("GL_TEXTURE_MIN_FILTER"); break;
	 case FL_PROJECTION: return string("GL_PROJECTION"); break;
	 case FL_MODELVIEW: return string("GL_MODELVIEW"); break;
	 case FL_QUADS: return string("GL_QUADS"); break;
	 case FL_POLYGON: return string("GL_POLYGON"); break;
	 case FL_FLAT: return string("GL_FLAT"); break;
	 case FL_BLEND: return string("GL_BLEND"); break;
	 case FL_SRC_ALPHA: return string("GL_SRC_ALPHA"); break;
	 case FL_DST_ALPHA: return string("GL_DST_ALPHA"); break;  
	 case FL_ONE_MINUS_SRC_ALPHA: return string("GL_ONE_MINUS_SRC_ALPHA"); break;
	 case FL_ONE_MINUS_DST_ALPHA: return string("GL_ONE_MINUS_DST_ALPHA"); break;
	 case FL_TEXTURE_ENV: return string("GL_TEXTURE_ENV"); break;
	 case FL_TEXTURE_ENV_MODE: return string("GL_TEXTURE_ENV_MODE"); break;
	 case FL_SRC_COLOR: return string("GL_SRC_COLOR"); break;
	 case FL_ONE_MINUS_SRC_COLOR: return string("GL_ONE_MINUS_SRC_COLOR"); break;
	 case FL_RGBA: return string("GL_RGBA"); break;
	 case FL_UNSIGNED_BYTE: return string("GL_UNSIGNED_BYTE"); break;
	 
	 default: return string(fPrint("FLEnum : %d", flEnum)); break;
      }
   }
   else
   {
      return string("");
   }
}

// Returns a textual representation of an int
const string getInt(const int value)
{
   if(constants::log_opengl)
   {
      switch(value)
      {
	 case FL_NEAREST: return string("GL_NEAREST"); break;
	 case FL_LINEAR_MIPMAP_LINEAR: return string("GL_LINEAR_MIPMAP_LINEAR"); break;
	 case FL_LINEAR: return string("GL_LINEAR"); break;
	 case FL_REPLACE: return string("GL_REPLACE"); break;
	    
	 default: return string(fPrint("int : %d", value)); break;
      }
   }
   else
   {
      return string("");
   }
}

#define catchGlError() if(constants::catch_opengl_errors) {_catchGlError(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION);}

void _catchGlError(const char* file, const int line, const char* func)
{
   if(constants::catch_opengl_errors)
   {
      const GLenum err0r = glGetError();
      incrementGlOpsCount();
      
      if(GL_NO_ERROR != err0r)
      {
	 error("GL ERROR at %s:%d : function : \"%s\"", file, line, func);
	 switch(err0r)
	 {
	    case GL_NO_ERROR: break;
	    case GL_INVALID_ENUM: throw string("opengl error : GL_INVALID_ENUM");break;
	    case GL_INVALID_VALUE: throw string("opengl error : GL_INVALID_VALUE");break;
	    case GL_INVALID_OPERATION: throw string("opengl error : GL_INVALID_OPERATION");break;
	    case GL_STACK_OVERFLOW: throw string("opengl error : GL_STACK_OVERFLOW");break;
	    case GL_STACK_UNDERFLOW: throw string("opengl error : GL_STACK_UNDERFLOW");break;
	    case GL_OUT_OF_MEMORY: throw string("opengl error : GL_OUT_OF_MEMORY");break;
	    default: throw string("opengl error : unknown");break;
	 }
      }
   }
}

#define logGl(message, ...)	if(constants::log_opengl) {_logGl(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, message, ## __VA_ARGS__);}

inline void _logGl(const char* file, const int line, const char* func, const string message, ...)
{
   if(constants::log_opengl && shouldLog(file, line, func, constants::debug))
   {
      va_list ap;
      va_start(ap, message);
      logGlOut(fPrint(constants::log_format, file, line, func, message.c_str()), ap);
      va_end(ap);
   }
}


void flClearColor(const float red,
                  const float green,
                  const float blue,
                  const float alpha)
{
   logGl("glClearColor(%f, %f, %f, %f)", red, green, blue, alpha);
   glClearColor(red, green, blue, alpha);
   catchGlError();
   incrementGlOpsCount();
}


void flClear(const FLbitfield mask)
{

   logGl("glClear(%s)", getBitField(mask).c_str());
   glClear(mask);
   catchGlError();
   incrementGlOpsCount();
}

const unsigned long flGenTexture()
{
   GLuint texture;
   logGl("glGenTextures(%d,%d)", 1, &texture);
   glGenTextures(1, &texture);
   catchGlError();
   incrementGlOpsCount();
   return static_cast<unsigned long>(texture);
}

void flDeleteTexture(const unsigned long textureId)
{
   GLuint texture = static_cast<GLuint>(textureId);
   
   logGl("glDeleteTextures(1,%d)",&texture);
   glDeleteTextures(1, &texture);
   catchGlError();
   incrementGlOpsCount();
}

void flDeleteTextures(const vector<unsigned long> textureIds)
{
   GLuint* array = new GLuint[textureIds.size()];

   int count = 0;
   for(vector<unsigned long>::const_iterator i = textureIds.begin(); i != textureIds.end(); i++)
   {
      array[count] = static_cast<GLuint>(*i);
      count++;
   }

   logGl("glDeleteTextures(%d,%d)",textureIds.size(), array);
   glDeleteTextures(textureIds.size(), array);
   catchGlError();
   incrementGlOpsCount();
   
   delete[] array;
}


vector<unsigned long> flGenTextures(const int howMany)
{
   vector<unsigned long> result;
   GLuint* array = new GLuint[howMany];

   logGl("glGenTextures(%d,%d)", howMany, array);
   glGenTextures(howMany, array);
   catchGlError();
   incrementGlOpsCount();
   
   for(int i = 0; i < howMany; i++)
   {
      result.push_back(static_cast<unsigned long>(array[i]));
   }
   delete[] array;

   return result;
}


void flBindTexture(const FLEnum target, const unsigned long textureId)
{
   logGl("glBindTexture(%s,%d)",getEnum(target).c_str(), textureId);
   glBindTexture(target, textureId);
   catchGlError();
   incrementGlOpsCount();
}

void flTexParameteri(const FLEnum target, const FLEnum pname, const int param)
{
   logGl("glTexParameteri(%s,%s,%s)", getEnum(target).c_str(), getEnum(pname).c_str(), getEnum(param).c_str());
   glTexParameteri(target, pname, param);
   catchGlError();
   incrementGlOpsCount();
}

void flTexImage2D(const FLEnum target,
		  const int level,
		  const int internalformat,
		  const int width,
		  const int height,
		  const int border,
		  const FLEnum format,
		  const FLEnum type,
		  const void* pixels)
{
   logGl("glTexImage2D(%s,%d,%s,%d,%d,%d,%s,%s,%d)",getEnum(target).c_str(),level, getInt(internalformat).c_str(), width, height, border, getEnum(format).c_str(), getEnum(type).c_str(),pixels);
   glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
   catchGlError();
   incrementGlOpsCount();
}

void flMatrixMode(const FLEnum mode)
{
   logGl("glMatrixMode(%s)",getEnum(mode).c_str());
   glMatrixMode(mode);
   catchGlError();
   incrementGlOpsCount();
};

void flLoadIdentity()
{
   logGl("glLoadIdentity()");
   glLoadIdentity();
   catchGlError();
   incrementGlOpsCount();
};

void flSet2DMode()
{   
   flMatrixMode(FL_MODELVIEW);
   flLoadIdentity();
   
   flMatrixMode(FL_PROJECTION);
   flLoadIdentity();
   flOrtho(0.0, static_cast<double>(constants::demo_width), 0.0, static_cast<double>(constants::demo_height), -1.0, 1.0);
};

void flSet2DMode640()
{
   flMatrixMode(FL_MODELVIEW);
   flLoadIdentity();
   
   flMatrixMode(FL_PROJECTION);
   flLoadIdentity();
   flOrtho(0.0, static_cast<double>(640), 0.0, static_cast<double>(480), -1.0, 1.0); 
}

void flOrtho(const double left, const double right, const double bottom, const double top, const double zNear, const double zFar)
{
   logGl("glOrtho(%f,%f,%f,%f,%f,%f)", left, right, bottom, top, zNear, zFar);
   glOrtho(left, right, bottom, top, zNear, zFar);
   catchGlError();
   incrementGlOpsCount();
}

void flBegin(const FLEnum mode)
{
   logGl("glBegin(%s)", getEnum(mode).c_str());
   glBegin(mode);
   incrementGlOpsCount();
}

void flEnd()
{
   logGl("glEnd()");
   glEnd();
   catchGlError();
   incrementGlOpsCount();
}

void flVertex2d(const double x, const double y)
{
   logGl("glVertex2d(%f,%f)", x, y);
   glVertex2d(x, y);
   incrementGlOpsCount();
}

void flColor3d(const double red, const double green, const double blue)
{
   logGl("glColor3d(%f,%f,%f)", red, green, blue);
   glColor3d(red, green, blue);
   incrementGlOpsCount();
}

void flColor4d(const double red, const double green, const double blue, const double alpha)
{
   logGl("glColor4d(%f,%f,%f,%f)", red, green, blue, alpha);
   glColor4d(red, green, blue, alpha);
   incrementGlOpsCount();
}

void flShadeModel(const FLEnum model)
{
   logGl("glShadeModel(%s)", getEnum(model).c_str());
   glShadeModel(model);
   catchGlError();
   incrementGlOpsCount();
}

void flEnable(const FLEnum cap)
{
   logGl("glEnable(%s)", getEnum(cap).c_str());
   glEnable(cap);
   catchGlError();
}

void flDisable(const FLEnum cap)
{
   logGl("glDisable(%s)", getEnum(cap).c_str());
   glDisable(cap);
   catchGlError();
   incrementGlOpsCount();
}

void flBlendFunc(const FLEnum src, const FLEnum dst)
{
   logGl("glBlendFunc(%s,%s)", getEnum(src).c_str(), getEnum(dst).c_str());
   glBlendFunc(src, dst);
   catchGlError();
   incrementGlOpsCount();
}

void flTexCoord2d(const double s, const double t)
{
   logGl("glTexCoord2d(%f,%f)", s, t);
   glTexCoord2d(s, t);
   incrementGlOpsCount();
}

void flTexEnvi(const FLEnum target,
	       const FLEnum pname,
	       const int param)
{
   logGl("glTexEnvi(%s,%s,%s)", getEnum(target).c_str(), getEnum(pname).c_str(), getInt(param).c_str());
   glTexEnvi(target, pname, param);
   catchGlError();
   incrementGlOpsCount();
}

void flPushAttrib(const FLbitfield mask)
{
   logGl("glPushAttrib(%s)",getBitField(mask).c_str());
   glPushAttrib(mask);
   catchGlError();
   incrementGlOpsCount();
}

void flPopAttrib()
{
   logGl("glPopAttrib()");
   glPopAttrib();
   catchGlError();
   incrementGlOpsCount();
}

void flTranslate(const double x, const double y)
{
   logGl("glTranslated(%lf,%lf,0.0)", x, y);
   glTranslated(x, y, 0.0);
   catchGlError();
   incrementGlOpsCount();
}

void flRotate(const double angle)
{
   logGl("glRotated(%lf,0.0,0.0,1.0)", angle);
   glRotated(angle, 0.0, 0.0, 1.0);
   catchGlError();
   incrementGlOpsCount(); 
}

void flScale(const double factor)
{
   flScale(factor, factor, factor);
}

void flScale(const double x, const double y, const double z)
{
   logGl("glScaled(%lf,%lf,%lf)", x, y, z);
   glScaled(x, y, z);
   catchGlError();
   incrementGlOpsCount();
}

void flLoadMatrix(shared_array<double> matrix)
{
   logGl("glLoadMatrixd(%d)", matrix.get());
   glLoadMatrixd(matrix.get());
   catchGlError();
   incrementGlOpsCount();
}

shared_array<double> flGetModelViewMatrix()
{
   double* matrix = new double[16];
   
   logGl("glGetDoublev(GL_MODELVIEW_MATRIX, %d)", matrix);
   glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
   catchGlError();
   incrementGlOpsCount();
   
   shared_array<double> result(matrix);
   return result;
}

void ModelViewStack::push()
{
   shared_array<double> matrix = flGetModelViewMatrix();
   stack.push(matrix);
}

void ModelViewStack::pop()
{
   shared_array<double> matrix = stack.top();
   flLoadMatrix(matrix);
   stack.pop();
}

int ModelViewStack::size()
{
   return stack.size();
}

void fluBuild1DMipmaps(const FLEnum target, const int internalFormat, const int width, const FLEnum format, const FLEnum type, const void* texels)
{
   logGl("gluBuild1DMipmaps(%s, %s, %d, %s, %s, %d)",
	 getEnum(target).c_str(),
	 getInt(internalFormat).c_str(),
	 width,
	 getEnum(format).c_str(),
	 getEnum(type).c_str(),
	 texels
      );
   
   const int ok = gluBuild1DMipmaps(target, internalFormat, width, format, type, texels);
   if(ok == 0)
   {
      catchGlError();
   }
   else
   {
      const string err0R("gluBuild1DMipMaps error");
      error(err0R);
      throw(err0R);
   }
   
   incrementGlOpsCount(); 
}

void flTexCoord1d(const double coord)
{
   logGl("glTexCoord1d(%lf)", coord);
   glTexCoord1d(coord);
   incrementGlOpsCount();
}
