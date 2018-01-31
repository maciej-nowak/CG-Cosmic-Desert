#include "Matrixes.h"


Matrixes::Matrixes(void)
{
}


Matrixes::~Matrixes(void)
{
}

void setWorldToCamera(glutil::MatrixStack &camMatrix, Shader shader)
{
	glutil::PushStack push(camMatrix);
	glUseProgram(shader.theProgram);
	glUniformMatrix4fv(shader.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(0);
}

void setCameraToPerspective(glutil::MatrixStack &persMatrix, Shader shader)
{
	glutil::PushStack push(persMatrix);
	glUseProgram(shader.theProgram);
	glUniformMatrix4fv(shader.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(0);
}



