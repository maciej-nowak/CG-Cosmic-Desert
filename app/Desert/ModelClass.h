#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <glload/gl_3_3.h>
#include <glutil/glutil.h>
#include "../framework/Mesh.h"
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

class ModelClass
{
public:
	ModelClass();
	~ModelClass();

	GLuint LoadBMP(const char* path);
	bool Initialize(const char*, const char*);
	void Shutdown();
	void Render();

private:
	vector<glm::vec3> out_vertices;
	vector<glm::vec2> out_uvs;
	vector<glm::vec3> out_normals;

	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;
	GLuint vao;
	GLuint Texture;	
};

#endif