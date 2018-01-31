#include "Shader.h"


Shader::Shader(void)
{
}


Shader::~Shader(void)
{
}

Shader Shader::LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
	std::vector<GLuint> shaderList;
	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, strFragmentShader));
	Shader data;
	data.theProgram = Framework::CreateProgram(shaderList); //tworzenie programu - laczenie shadera vertexa i shadera fragmentu
	data.modelToWorldMatrixUnif = glGetUniformLocation(data.theProgram, "modelToWorldMatrix");
	data.worldToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "worldToCameraMatrix");
	data.cameraToClipMatrixUnif = glGetUniformLocation(data.theProgram, "cameraToClipMatrix");
	data.baseColorUnif = glGetUniformLocation(data.theProgram, "baseColor"); //tylko objectShader i ballShader
	data.timeUnif = glGetUniformLocation(data.theProgram, "time"); //tylko planeteShader i ballShader
	data.offsetUnif = glGetUniformLocation(data.theProgram, "offset"); //tylko planeteShader i ballShader
	data.texture = glGetUniformLocation(data.theProgram, "myTextureSampler"); //tylko texturShader i planeteShader
	
	return data;
}


