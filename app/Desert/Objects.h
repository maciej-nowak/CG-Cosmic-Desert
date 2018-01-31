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
#include "Model.h"

class Objects
{

	public:
		Objects(void);
		~Objects(void);

		void LoadMeshes();
		void LoadModels();
		void DrawCube(glutil::MatrixStack &modelMatrix, Shader shader);
		void DrawKorona(glutil::MatrixStack &modelMatrix, Shader shader, float stageWidth, float stageLength, float stageHeight, float stageX, float stageY, float stageZ);
		void DrawTree(glutil::MatrixStack &modelMatrix, Shader shader);
		void DrawStage(glutil::MatrixStack &modelMatrix, Shader shader, float stageWidth, float stageLength, float stageHeight, float stageX, float stageY, float stageZ);
		void DrawPyramid(glutil::MatrixStack &modelMatrix, Shader shader);
		void DrawBall(glutil::MatrixStack &modelMatrix, Shader shader);
		void DrawPlanete(glutil::MatrixStack &modelMatrix, Shader shader, Model *model, glm::vec3 offset);
		void DrawDesert(glutil::MatrixStack &modelMatrix, Shader shader, Model *model);
		void DrawShip(Shader shader);
		void keyboard(unsigned char key, int x, int y);
		void keyboardArrows(int key, int x, int y);

		Model *earth;
		Model *sun;
		Model *mars;
		Model *moon;
		Model *desert;

		glutil::MatrixStack shipModelMatrix;

		Framework::Mesh *cone;
		Framework::Mesh *cylinder;
		Framework::Mesh *cube;
		Framework::Mesh *ship;
		Framework::Mesh *ball;
		
};

