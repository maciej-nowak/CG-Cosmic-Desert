#include "ModelClass.h"

// Constructor.
ModelClass::ModelClass()
{
}

// Destructor.
ModelClass::~ModelClass()
{
}

GLuint ModelClass::LoadBMP(const char* path)
{
	// Data read from the header of the BMP file.
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	// Actual RGB data.
	unsigned char* data;

	// Open the file.
	FILE * file = fopen(path, "rb");
	if(!file)
	{
		cout << "Image could not be opened!" << endl;
		return false;
	}
	if(fread(header, 1, 54, file) != 54)
	{
		cout << "Not a correct BMP file!" << endl;
		return false;
	}
	if(header[0] != 'B' || header[1] != 'M')
	{
		cout << "Not a correct BMP file!" << endl;
		return false;
	}
	// Read ints from the byte array.
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information.
	if (imageSize == 0)
		imageSize = width*height*3;
	if(dataPos == 0)
		dataPos = 54;

	// Create a buffer.
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer.
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file can be closed.
	fclose(file);

	// Create one OpenGL texture.
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version.
	delete [] data;

	// Poor filtering or nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glGenerateMipmap(GL_TEXTURE_2D);

    // Return the ID of the texture we just created
    return textureID;
}

// Initialize the Model object.
bool ModelClass::Initialize(const char* path, const char* path_texture)
{
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;

	// Read file.
	FILE * file = fopen(path, "r");
	if(file == NULL)
	{
		cout << "Imposible to open the file!\n" << endl;
		return false;
	}
	while(1)
	{
		char lineHeader[128];

		// Read the first word of the line.
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		// Else : parse lineHeader.
		if(strcmp(lineHeader, "v") == 0)
		{
			// Read for vertex.
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}	
		else if(strcmp(lineHeader, "vt") == 0)
		{
			// Read for uv.
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if(strcmp(lineHeader, "vn") == 0)
		{
			// Read for normal.
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if(strcmp(lineHeader, "f") == 0)
		{
			// Read for f.
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				cout << "File can't be read by our simple parser!" << endl;
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else
		{
			// Probably a comment, eat up the rest of the line.
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	// For each vertex of each triangle.
	for(unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		// Get the indices of its attributes.
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index.
		glm::vec3 vertex = temp_vertices[vertexIndex-1];
		glm::vec2 uv = temp_uvs[uvIndex-1];
		glm::vec3 normal = temp_normals[normalIndex-1];

		// Put the attributes in buffers.
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}

	// Load texture from file.
	Texture = LoadBMP(path_texture);

	// Set OpenGL for vertex and uv.
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(glm::vec2), &out_uvs[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 1);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(glm::vec3), &out_normals[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 2);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	return true;
}

// Render the model.
void ModelClass::Render()
{
	// Bind out texture in Texture Unit 0.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// Set OpenGL render for vertex and uv.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);

	glDrawArrays(GL_TRIANGLES, 0, out_vertices.size());

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	return;
}

// Close the Model object.
void ModelClass::Shutdown()
{

	// Release buffers from vertex.
	glDeleteBuffers(1, &vertexBuffer);

	// Release buffers from uv.
	glDeleteBuffers(1, &uvBuffer);

	// Release buffers from normal.
	glDeleteBuffers(1, &normalBuffer);
	return;
}