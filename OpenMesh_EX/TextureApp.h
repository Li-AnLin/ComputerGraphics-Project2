#pragma once

#include <string.h>
#include <vector>
#include <math.h>

#include <GL\glut.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#define BUFFER_OFFSET(x)  ((const void*) (x))

namespace TextureApp{
	bool	LoadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData);
	GLuint	GenTexture(char* filepath);
}