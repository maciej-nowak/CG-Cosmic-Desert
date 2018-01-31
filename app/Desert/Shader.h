#pragma once

#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <stdio.h>
#include <glload/gl_3_3.h>
#include <glutil/glutil.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include "../framework/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{

	public:
		Shader(void);
		~Shader(void);

		static Shader LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader);

		GLuint theProgram;
		GLuint modelToWorldMatrixUnif;
		GLuint worldToCameraMatrixUnif;
		GLuint cameraToClipMatrixUnif;
		GLuint baseColorUnif;
		GLuint timeUnif;
		GLuint texture;
		GLuint offsetUnif;
};