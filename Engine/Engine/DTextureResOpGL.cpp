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

DTextureResOpGL::DTextureResOpGL(WCHAR * filename, DWrapMode wrapMode)
{
	m_isSuccess = false;

	m_data = LoadTGA(filename, m_width, m_height);
	if (m_data == 0)
		return;
	glGenTextures(1, &m_textureId);

	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapMode(m_wrapMode));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapMode(m_wrapMode));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);


	m_textureType = GL_TEXTURE_2D;

	m_wrapMode = wrapMode;

	m_isSuccess = true;
}

DTextureResOpGL::DTextureResOpGL(DTextureResOpGL * right, DTextureResOpGL * left, DTextureResOpGL * top, DTextureResOpGL * bottom, DTextureResOpGL * front, DTextureResOpGL* back)
{
	m_isSuccess = false;

	if (right == NULL || right->m_data == 0)
		return;
	if (left == NULL || left->m_data == 0)
		return;
	if (top == NULL || top->m_data == 0)
		return;
	if (bottom == NULL || bottom->m_data == 0)
		return;
	if (front == NULL || front->m_data == 0)
		return;
	if (back == NULL || back->m_data == 0)
		return;

	glGenTextures(1, &m_textureId);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, right->m_width, right->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, right->m_data);
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, left->m_width, left->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, left->m_data);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, top->m_width, top->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, top->m_data);
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, bottom->m_width, bottom->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottom->m_data);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, front->m_width, front->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, front->m_data);
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, back->m_width, back->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, back->m_data);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	m_textureType = GL_TEXTURE_CUBE_MAP;

	m_isSuccess = true;
}

DTextureResOpGL::~DTextureResOpGL()
{
}

void DTextureResOpGL::Apply(UINT location, int index)
{
	if (!m_isSuccess)
		return;
	if (location != -1) 
	{
		//GLint ac = glGetUniformLocation(3, "shaderTexture");
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(m_textureType, m_textureId);
		
		//glBindTexture(GL_TEXTURE_2D, m_textureId);
		glUniform1i(location, index);
	}
}

void DTextureResOpGL::ApplyWrapMode(UINT location, DWrapMode wrapMode)
{
	if (!m_isSuccess)
		return;
	if (m_wrapMode == wrapMode)
	{
		return;
	}
	m_wrapMode = wrapMode;
	glBindTexture(m_textureType, m_textureId);

	glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, GetWrapMode(m_wrapMode));
	glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, GetWrapMode(m_wrapMode));
}

void DTextureResOpGL::Release()
{
	if (m_isSuccess)
		glDeleteTextures(1, &m_textureId);
	if (m_data != 0)
	{
		delete[] m_data;
		m_data = 0;
	}
}

unsigned char* DTextureResOpGL::LoadBMP(WCHAR * path, unsigned int& width, unsigned int& height)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
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
		return 0;
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

	return data;
}

unsigned char* DTextureResOpGL::LoadTGA(WCHAR * path, unsigned int& width, unsigned int& height)
{
	int error, bpp, imageSize;
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
		return 0;
	}

	// Read in the file header.
	count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return 0;
	}

	// Get the important information from the header.
	width = (int)targaFileHeader.width;
	height = (int)targaFileHeader.height;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32)
	{
		return 0;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return 0;
	}

	// Read in the targa image data.
	count = fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return 0;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return 0;
	}

	// Generate an ID for the texture.

	//glUniform1i(3, m_textureId);
	

	// Release the targa image data.
	//delete[] targaImage;
	//targaImage = 0;

	// Set that the texture is loaded.

	return targaImage;
}

GLint DTextureResOpGL::GetWrapMode(DWrapMode wrapMode)
{
	if (wrapMode == DWrapMode_Clamp)
		return GL_CLAMP;
	else if (wrapMode == DWrapMode_Repeat)
		return GL_REPEAT;
	return GL_CLAMP;
}

DRenderTextureViewResOpGL::DRenderTextureViewResOpGL(float width, float height, DWrapMode wrapMode)
{
	m_isSuccess = false;
	m_wrapMode = wrapMode;

	m_frameBufferId = 0;
	glGenFramebuffers(1, &m_frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

	glGenTextures(1, &m_renderTextureId);

	glBindTexture(GL_TEXTURE_2D, m_renderTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapMode(m_wrapMode));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapMode(m_wrapMode));

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTextureId, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	m_isSuccess = true;
}

DRenderTextureViewResOpGL::~DRenderTextureViewResOpGL()
{
}

void DRenderTextureViewResOpGL::Apply(UINT location, int index)
{
	if (!m_isSuccess)
		return;
	if (location != -1)
	{
		//GLint ac = glGetUniformLocation(3, "shaderTexture");
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, m_frameBufferId);

		//glBindTexture(GL_TEXTURE_2D, m_textureId);
		glUniform1i(location, index);
	}
}

void DRenderTextureViewResOpGL::ApplyWrapMode(UINT, DWrapMode wrapMode)
{
	if (!m_isSuccess)
		return;
	if (m_wrapMode == wrapMode)
	{
		return;
	}
	m_wrapMode = wrapMode;
	glBindTexture(GL_TEXTURE_2D, m_frameBufferId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapMode(m_wrapMode));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapMode(m_wrapMode));
}

void DRenderTextureViewResOpGL::Release()
{
	if (m_isSuccess)
		glDeleteTextures(1, &m_frameBufferId);
	glDeleteFramebuffers(1, &m_frameBufferId);
}

IRenderBuffer * DRenderTextureViewResOpGL::GetColorBuffer()
{
	return nullptr;
}

IRenderBuffer * DRenderTextureViewResOpGL::GetDepthBuffer()
{
	return nullptr;
}

void DRenderTextureViewResOpGL::BindFBO()
{
	if(m_isSuccess)
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
}

void DRenderTextureViewResOpGL::UnBindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLint DRenderTextureViewResOpGL::GetWrapMode(DWrapMode wrapMode)
{
	if (wrapMode == DWrapMode_Clamp)
		return GL_CLAMP;
	else if (wrapMode == DWrapMode_Repeat)
		return GL_REPEAT;
	return GL_CLAMP;
}

#endif


