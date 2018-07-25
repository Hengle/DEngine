#ifdef _DGAPI_OPENGL

#include "DShaderPassOpGL.h"


DShaderProgramOpGL::DShaderProgramOpGL() : DShaderProgram()
{
}

DShaderProgramOpGL::~DShaderProgramOpGL()
{
}

void DShaderProgramOpGL::InitVertexShader(const char * content)
{
	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glShaderSource(m_vertexShaderID, 1, &content, NULL);
	glCompileShader(m_vertexShaderID);

	glGetShaderiv(m_vertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(m_vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		//std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		//glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		//printf("%s\n", &VertexShaderErrorMessage[0]);
		throw "shader cuowu";
	}
}

void DShaderProgramOpGL::InitFragmentShader(const char * content)
{
	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glShaderSource(m_fragmentShaderID, 1, &content, NULL);
	glCompileShader(m_fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(m_fragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(m_fragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) {
		/*std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);*/
		throw "shader cuowu";
	}
}

void DShaderProgramOpGL::InitProgram()
{
	GLint Result = GL_FALSE;
	int InfoLogLength;

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, m_vertexShaderID);
	glAttachShader(ProgramID, m_fragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		/*std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);*/
		throw "shader cuowu";
	}

	glDetachShader(ProgramID, m_vertexShaderID);
	glDetachShader(ProgramID, m_fragmentShaderID);

	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);

	m_ProgramID = ProgramID;


	m_vertexUsage = DVertexUsage_POSITION;

	m_isInitialized = true;
}

void DShaderProgramOpGL::GetResDesc(unsigned int index, DShaderResDesc &) const
{
}

bool DShaderProgramOpGL::HasProperty(const LPCSTR key) const
{
	GLuint uid = glGetUniformLocation(m_ProgramID, key);
	if (uid == 0xffffff)
		return false;
	return true;
}

void DShaderProgramOpGL::Release()
{
	glDeleteProgram(m_ProgramID);
}

void DShaderProgramOpGL::OnDraw()
{
	glUseProgram(m_ProgramID);
}

void DShaderProgramOpGL::OnApplyParams(std::map<std::string, float*>& params, std::map<std::string, float*>& gparams)
{
}

DShaderPassOpGL::DShaderPassOpGL() : DShaderPass()
{
	m_program = 0;
}


DShaderPassOpGL::~DShaderPassOpGL()
{
}

void DShaderPassOpGL::Release()
{
	if (m_program != NULL) {
		m_program->Release();
		delete m_program;
		m_program = 0;
	}
	
}

void DShaderPassOpGL::Draw()
{
	if (m_program != NULL)
		m_program->Draw();
}

int DShaderPassOpGL::GetVertexUsage()
{
	if (m_program != NULL)
		return m_program->GetVertexUsage();
	return 0;
}

bool DShaderPassOpGL::HasProperty(LPCSTR key)
{
	if (m_program != NULL)
		return m_program->HasProperty(key);
	return false;
}

void DShaderPassOpGL::CompileShader(std::ifstream & ifile)
{
	char read[64];
	while (!ifile.eof())
	{
		ifile >> read;
		if (strcmp(read, "#code"))
		{
			CompileShaderCode(ifile);
			return;
		}
	}
}

int DShaderPassOpGL::GetShaderProgramCount()
{
	return 1;
}

DShaderProgram * DShaderPassOpGL::GetShaderProgram(int index)
{
	if (index == 0)
		return m_program;
	return NULL;
}

void DShaderPassOpGL::CompileShaderCode(std::ifstream & ifile)
{
	char read[64];
	bool isBegin = false;
	m_program = new DShaderProgramOpGL();
	while (!ifile.eof())
	{
		ifile >> read;
		if (!isBegin)
		{
			if (strcmp(read, "["))
			{
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "]"))
			{
				isBegin = false;
				if (m_program != NULL)
				{
					m_program->InitProgram();
				}
				return;
			}
			else if(strcmp(read,"#vert"))
			{
				CompileVertexShader(ifile);
			}
			else if (strcmp(read, "#frag"))
			{
				CompileFragmentShader(ifile);
			}
		}
		
	}
}

void DShaderPassOpGL::CompileVertexShader(std::ifstream & ifile)
{
	bool isBegin = false;
	std::string s;
	char read = 0;
	while (!ifile.eof())
	{
		read = ifile.get();

		if (!isBegin)
		{
			if (read == '[')
			{
				isBegin = true;
			}
		}
		else
		{
			if (read == ']')
			{
				isBegin = false;

				const char* content = s.c_str();

				if (m_program != NULL)
					m_program->InitVertexShader(content);

				return;
			}
			else
			{
				s.push_back(read);
			}
		}
	}
}

void DShaderPassOpGL::CompileFragmentShader(std::ifstream & ifile)
{
	bool isBegin = false;
	std::string s;
	char read = 0;
	while (!ifile.eof())
	{
		read = ifile.get();

		if (!isBegin)
		{
			if (read == '[')
			{
				isBegin = true;
			}
		}
		else
		{
			if (read == ']')
			{
				isBegin = false;

				const char* content = s.c_str();

				if (m_program != NULL)
					m_program->InitFragmentShader(content);

				return;
			}
			else
			{
				s.push_back(read);
			}
		}
	}
}

#endif

