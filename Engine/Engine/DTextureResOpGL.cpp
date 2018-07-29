#ifdef _DGAPI_OPENGL

#include "DTextureResOpGL.h"
#include "GLFW/glfw3native.h"
#include <comdef.h>

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

DTextureResOpGL::DTextureResOpGL(WCHAR * filename)
{
	//m_textureId = LoadBMP(filename);
	LoadTGA(filename, m_textureId);
}

DTextureResOpGL::~DTextureResOpGL()
{
}

void DTextureResOpGL::Apply(UINT location, DWrapMode)
{
	if (location != -1) 
	{
		//GLint ac = glGetUniformLocation(3, "shaderTexture");
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_textureId);
		
		//glBindTexture(GL_TEXTURE_2D, m_textureId);
		glUniform1i(location, 0);
	}
}

void DTextureResOpGL::Release()
{
	glDeleteTextures(1, &m_textureId);
}

GLuint DTextureResOpGL::LoadBMP(WCHAR * path)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
							  // Actual RGB data
	unsigned char * data;

	_bstr_t tmp(path);
	const char* p = tmp;
	FILE * file;
	fopen_s(&file, p, "rb");
	if (!file)
	{
		//printf("Image could not be opened");
		throw "Image could not be opened";
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

										 // Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	

	return textureID;
}

GLuint DTextureResOpGL::LoadTGA(WCHAR * path, GLuint& textureId)
{
	int error, width, height, bpp, imageSize;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;


	_bstr_t tmp(path);
	const char* p = tmp;
	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, p, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	width = (int)targaFileHeader.width;
	height = (int)targaFileHeader.height;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return false;
	}

	// Read in the targa image data.
	count = fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Generate an ID for the texture.
	glGenTextures(1, &textureId);

	// Set the unique texture unit in which to store the data.
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture as a 2D texture.
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Load the image data into the texture unit.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, targaImage);
	/*float pixels[] = {
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);*/


	// Set the texture color to either wrap around or clamp to the edge.
	//if (wrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	//else
	{
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	// Set the texture filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate mipmaps for the texture.
	glGenerateMipmap(GL_TEXTURE_2D);

	//glUniform1i(3, m_textureId);
	

	// Release the targa image data.
	delete[] targaImage;
	targaImage = 0;

	// Set that the texture is loaded.

	return true;
}

#endif