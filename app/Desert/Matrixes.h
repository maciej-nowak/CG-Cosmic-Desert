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
#include "Shader.h"

class Matrixes
{

	public:
		Matrixes(void);
		~Matrixes(void);

		void setModelToWorld(glutil::MatrixStack &camMatrix, Shader shader);
		void setWorldToCamera(glutil::MatrixStack &camMatrix, Shader shader);
		void setCameraToPerspective(glutil::MatrixStack &persMatrix, Shader shader);
};

