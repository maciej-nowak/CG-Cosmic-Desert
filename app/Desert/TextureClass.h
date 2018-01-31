#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <glload/gl_3_3.h>
#include <glutil/glutil.h>
#include "../framework/Mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class TextureClass
{
public:
	TextureClass();
	~TextureClass();

	GLuint LoadPNG(const char* path);
	GLuint LoadBMP(const char* path);
	GLuint LoadTGA(const char* path);
	GLuint LoadDDS(const char* path);
	GLuint LoadTGAGLFW(const char* path);

	void LoadSkyboxBMP(const char*, GLenum);

private:

};

#endif