#include "TextureClass.h"

// Constructor.
TextureClass::TextureClass()
{
}

// Destructor.
TextureClass::~TextureClass()
{
}

// Function for loading a BMP format.
GLuint TextureClass::LoadBMP(const char* path)
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

// Function for loading a PNG format.
GLuint TextureClass::LoadPNG(const char* path)
{
	return 0;
}

// Function for loading a DDS format.
GLuint TextureClass::LoadDDS(const char* path)
{
	return 0;
}

// Function for loading a TGA format.
GLuint TextureClass::LoadTGA(const char* path)
{
	// Data read from the header of the TGA file.
	unsigned char header[20];
	unsigned int width, height, bpp;

	ifstream File(path, ios::in | ios::binary);
	
	if(!File)
	{
		cout << "Image could not be opened!" << endl;
		return 0;
	}

	// Read all 18 bytes of the header.
	File.read(reinterpret_cast<char *>(header), sizeof (char)*18);
	if(header[2] != 2 && header[2] != 10)
	{
		File.close();
		return 0;
	}

	// If there is an image ID section then skip over it.
	if (header[0])
	{
		File.seekg(header[0], ios_base::cur);
	}

	// Get the size and bitdepth from the header.
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	bpp = header[16]/8;

	if(bpp != 3 && bpp != 4)
	{
		File.close();
		return 0;
	}

	long imageSize = width * height * bpp;

	// Allocate memory for image data.
	unsigned char *data = new unsigned char[imageSize];

	// Read the uncompressed image data if type 2.
	if(header[2] == 2)
	{
		File.read(reinterpret_cast<char *>(data), sizeof (char)*imageSize);
	}

	long ctpixel = 0, ctloop = 0;

	// Read the compressed image data if type 10.
	if(header[2] == 10)
	{
		unsigned char rle;
		unsigned char color[4];

		while(ctpixel<imageSize)
		{
			File.read(reinterpret_cast<char *>(&rle), 1);

			// if the rle header is below 128 it means that what folows is just raw data with rle+1 pixels
			if (rle<128)
			{
				File.read(reinterpret_cast<char *>(&data[ctpixel]), bpp*(rle+1));
				ctpixel+=bpp*(rle+1);
			}

			// if the rle header is equal or above 128 it means that we have a string of rle-127 pixels 
			// that use the folowing pixels color
			else 
			{
				// read what color we should use
				File.read(reinterpret_cast<char *>(&color[0]), bpp);

				// insert the color stored in tmp into the folowing rle-127 pixels
				ctloop=0;
				while(ctloop<(rle-127))
				{						
					data[ctpixel]=color[0];
					data[ctpixel+1]=color[1];
					data[ctpixel+2]=color[2];
					if (bpp==4)
					{
						data[ctpixel+3]=color[3];
					}

					ctpixel+=bpp;
					ctloop++;
				}
			}
		}
	}

	ctpixel = 0;
	//Because TGA file store their colors in BGRA format we need to swap the red and blue color components
	while (ctpixel<imageSize)
	{
		data[ctpixel] ^= data[ctpixel+2] ^=	data[ctpixel] ^= data[ctpixel+2];
		ctpixel+= bpp;
	}

	// Close file.
	File.close();
	unsigned int color_mode = GL_RGB;
	if(bpp = 4) color_mode = GL_RGBA;

	// Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
 
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
 
    // Nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
 
	glTexImage2D(GL_TEXTURE_2D, 0, color_mode, width, height, 0, color_mode, GL_UNSIGNED_BYTE, data);
	delete[] data;
	data = 0;

    // Return the ID of the texture we just created
    return textureID;
}

GLuint TextureClass::LoadTGAGLFW(const char* path)
{
	// Create one OpenGL texture.
	return 0;
}

// Function for loading a BMP format for SKYBOX.
void TextureClass::LoadSkyboxBMP(const char* path, GLenum types)
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
		//return false;
	}
	if(fread(header, 1, 54, file) != 54)
	{
		cout << "Not a correct BMP file!" << endl;
		//return false;
	}
	if(header[0] != 'B' || header[1] != 'M')
	{
		cout << "Not a correct BMP file!" << endl;
		//return false;
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

	// Give the image to OpenGL.
	glTexImage2D(types, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version.
	delete [] data;

	return;
}