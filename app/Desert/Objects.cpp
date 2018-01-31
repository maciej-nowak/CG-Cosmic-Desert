#include "Objects.h"


Objects::Objects()
{
}


Objects::~Objects(void)
{
}


void Objects::LoadMeshes()
{
	cone = new Framework::Mesh("models/cone.xml"); //stozek - liscie drzewa
	cylinder = new Framework::Mesh("models/cylinder.xml"); //pien drzewa
	cube = new Framework::Mesh("models/cube.xml"); //poziom piramidy
	ship = new Framework::Mesh("models/ship.xml"); //statek
	ball = new Framework::Mesh("models/sphere.xml"); //kula
}

void Objects::LoadModels()
{
	earth = new Model;
	earth->Initialize("data/planete/Earth.obj", "data/planete/Earth.bmp");
	sun = new Model;
	sun->Initialize("data/planete/Sun.obj", "data/planete/Sun.bmp");
	moon = new Model;
	moon->Initialize("data/planete/Moon.obj", "data/planete/Moon.bmp");
	mars = new Model;
	mars->Initialize("data/planete/Mars.obj", "data/planete/Mars.bmp");
	desert = new Model;
	desert->Initialize("data/planete/desert.obj", "data/planete/desert.bmp");
}

//POZIOM LISCI
void Objects::DrawKorona(glutil::MatrixStack &modelMatrix, Shader shader, float stageWidth, float stageLength, float stageHeight, float stageX, float stageY, float stageZ)
{
	glutil::PushStack push(modelMatrix);
	modelMatrix.Scale(glm::vec3(stageWidth, stageHeight, stageLength));
	modelMatrix.Translate(glm::vec3(stageX, stageY, stageZ));

	glUseProgram(shader.theProgram);
	glUniformMatrix4fv(shader.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(shader.baseColorUnif, 0.0f, 0.9f, 0.0f, 0.9f);
	cone->Render();
	glUseProgram(0);
}

//DRZEWO
void Objects::DrawTree(glutil::MatrixStack &modelMatrix, Shader shader)
{
	//LISCIE
	float stageWidth = 11.0f;
	float stageY = 2.5f;
	for(int i=1; i<=8; i++) 
	{
		{
			glutil::PushStack push(modelMatrix);
			DrawKorona(modelMatrix, shader, stageWidth, stageWidth, 2.0f, 0.0f, stageY, 0.0f); 
		}
		stageWidth = stageWidth - 1.5f;
		stageY = stageY + 0.5f;
	}

	//PIEN
	{
		glutil::PushStack push(modelMatrix);
		glUseProgram(shader.theProgram);
		modelMatrix.Scale(2.0f, 15.0f, 2.0f);
		modelMatrix.Translate(0.0f, 0.2f, 0.0f);
		glUniformMatrix4fv(shader.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		glUniform4f(shader.baseColorUnif, 0.7f, 0.4f, 0.1f, 0.9f);
		cylinder->Render();
		glUseProgram(0);
	}
}

//POZIOM PIRAMIDY
void Objects::DrawStage(glutil::MatrixStack &modelMatrix, Shader shader, float stageWidth, float stageLength, float stageHeight, float stageX, float stageY, float stageZ)
{
	glutil::PushStack push(modelMatrix);
	modelMatrix.Scale(glm::vec3(stageWidth, stageHeight, stageLength));
	modelMatrix.Translate(glm::vec3(stageX, stageY, stageZ));

	glUseProgram(shader.theProgram);
	glUniformMatrix4fv(shader.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(shader.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
	cube->Render();
	glUseProgram(0);
}

//PIRAMIRDA
void Objects::DrawPyramid(glutil::MatrixStack &modelMatrix, Shader shader)
{
	float stageWidth = 15.0f;
	float stageY = 0.5f;
	for(int i=1; i<=8; i++) 
	{
		{
			glutil::PushStack push(modelMatrix);
			DrawStage(modelMatrix, shader, stageWidth, stageWidth, 1.0f, 0.0f, stageY, 0.0f); 
		}
		stageWidth = stageWidth - 2.0f;
		stageY = stageY + 1.0f;
	}
}

//KULA
void Objects::DrawBall(glutil::MatrixStack &modelMatrix, Shader shader)
{
	glutil::PushStack push(modelMatrix);
	glUseProgram(shader.theProgram);
	glUniform1f(shader.timeUnif, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	glUniformMatrix4fv(shader.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(shader.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
	ball->Render();
	glUseProgram(0);
}

//STATEK
void Objects::DrawShip(Shader shader)
{
	glutil::PushStack push(shipModelMatrix);
	shipModelMatrix.Scale(4.0f, 4.0f, 4.0f);
	glUseProgram(shader.theProgram);
	glUniformMatrix4fv(shader.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(shipModelMatrix.Top()));
	glUniform4f(shader.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
	ship->Render();
	glUseProgram(0);
}

//PLANETA
void Objects::DrawPlanete(glutil::MatrixStack &modelMatrix, Shader shader, Model *model, glm::vec3 offset)
{
	glutil::PushStack push(modelMatrix);
	glUseProgram(shader.theProgram);
	glUniform1f(shader.timeUnif, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	glUniform3f(shader.offsetUnif, offset.x, offset.y, offset.z);
	glUniformMatrix4fv(shader.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform1i(shader.texture, 0);
	model->Render();
	glUseProgram(0);
}

//PUSTYNIA
void Objects::DrawDesert(glutil::MatrixStack &modelMatrix, Shader shader, Model *model)
{
	glutil::PushStack push(modelMatrix);
	glUseProgram(shader.theProgram);
	glUniformMatrix4fv(shader.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform1i(shader.texture, 0);
	model->Render();
	glUseProgram(0);
}

//KLAWIATURA DLA STATKU
void Objects::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case '\\': shipModelMatrix.RotateY(5.0f); break;  //obrot w prawo
		case ';': shipModelMatrix.RotateY(-5.0f); break; //obrot w lewo
		case '\'': shipModelMatrix.Translate(glm::vec3(0.0f, -1.0f, 0.0f)); break; //nizej
		case '[': shipModelMatrix.Translate(glm::vec3(0.0f, 1.0f, 0.0f)); break; //wyzej
	}

	glutPostRedisplay();
}

//KLAWIATURA DLA STATKU - KLAWISZE SPECJALNE
void Objects::keyboardArrows(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP: shipModelMatrix.Translate(glm::vec3(0.0f, 0.0f, 1.0f)); break; //w przod
		case GLUT_KEY_DOWN: shipModelMatrix.Translate(glm::vec3(0.0f, 0.0f, -1.0f)); break; //w tyl
		case GLUT_KEY_LEFT: shipModelMatrix.Translate(glm::vec3(-1.0f, 0.0f, 0.0f)); break; //w lewo
		case GLUT_KEY_RIGHT: shipModelMatrix.Translate(glm::vec3(1.0f, 0.0f, 0.0f)); break; //w prawo
	}
	glutPostRedisplay();
} 