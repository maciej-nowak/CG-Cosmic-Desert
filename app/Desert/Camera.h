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

class Camera
{

	public:
		Camera(void);
		~Camera(void);

		glm::vec3 ResolveCamPosition();
		void keyboard(unsigned char key, int x, int y);

		glm::vec3 cameraTarget;
		glm::vec3 sphereCameraPosistion;

};

