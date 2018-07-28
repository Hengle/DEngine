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

	InitUniforms(ProgramID);

	InitInputLayouts(ProgramID);

	m_ProgramID = ProgramID;


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
	std::map<std::string, ShaderPropertyDescOpGL>::iterator  iter;
	for (iter = m_properties.begin(); iter != m_properties.end(); iter++)
	{
		if (params.find(iter->first) != params.end())
		{
			float* v = params.at(iter->first);
			if (iter->second.type == GL_FLOAT_MAT4)
			{
				GLfloat p[16] = {
					v[0],v[1],v[2],v[3],
					v[4],v[5],v[6],v[7],
					v[8],v[9],v[10],v[11],
					v[12],v[13],v[14],v[15],
				};
				glUniformMatrix4fv(iter->second.paramId, 1, GL_FALSE, &p[0]);
			}
		}
		else if (iter->second.isGlobal && gparams.find(iter->first) != gparams.end())
		{
			float* v = gparams.at(iter->first);
			if (iter->second.type == GL_FLOAT_MAT4)
			{
				GLfloat p[16] = {
					v[0],v[4],v[8],v[12],
					v[1],v[5],v[9],v[13],
					v[2],v[6],v[10],v[14],
					v[3],v[7],v[11],v[15],
				};
				glUniformMatrix4fv(iter->second.paramId, 1, GL_FALSE, &p[0]);
			}
		}
	}
}

void DShaderProgramOpGL::InitInputLayouts(GLuint programId)
{
	GLint paramLength;
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &paramLength);

	GLint i;
	GLchar pname[64];
	GLsizei length;
	GLint size;
	GLenum type;

	m_vertexUsage = 0;
	for (i = 0; i < paramLength; i++)
	{
		glGetActiveAttrib(programId, i, 64, &length, &size, &type, pname);
		
		if (strcmp(pname, "input_position") == 0)
		{
			m_vertexUsage |= DVertexUsage_POSITION;
		}
		if (strcmp(pname, "input_color") == 0)
		{
			m_vertexUsage |= DVertexUsage_COLOR;
		}
		if (strcmp(pname, "input_normal") == 0)
		{
			m_vertexUsage |= DVertexUsage_NORMAL;
		}
		if (strcmp(pname, "input_texcoord0") == 0)
		{
			m_vertexUsage |= DVertexUsage_TEXCOORD0;
		}
	}
}

void DShaderProgramOpGL::InitUniforms(GLuint programId)
{
	GLint paramLength;
	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &paramLength);

	GLint i;
	GLchar pname[64];
	GLsizei length;
	GLint size;
	GLenum type;

	ShaderPropertyDescOpGL propert;
	DShaderResDesc resdesc;

	for (i = 0; i < paramLength; i++)
	{
		glGetActiveUniform(programId, i, 64, &length, &size, &type, pname);

		if (type == GL_SAMPLER_2D)
		{
			resdesc.resName = pname;
			resdesc.offset = glGetUniformLocation(programId, pname);
			if (strlen(pname) >= 2 && pname[0] == 'g' && pname[1] == '_')
			{
				resdesc.isGlobal = true;

			}
		}

		propert.type = type;
		propert.paramId = glGetUniformLocation(programId, pname);
		if (strlen(pname) >= 2 && pname[0] == 'g' && pname[1] == '_')
		{
			propert.isGlobal = true;
		}
		m_properties.insert(std::pair<std::string, ShaderPropertyDescOpGL>(pname, propert));
	}
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
		if (strcmp(read, "#code") == 0)
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
			if (strcmp(read, "[") == 0)
			{
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "]") == 0)
			{
				isBegin = false;
				if (m_program != NULL)
				{
					m_program->InitProgram();
				}
				return;
			}
			else if(strcmp(read,"#vert") == 0)
			{
				CompileVertexShader(ifile);
			}
			else if (strcmp(read, "#frag") == 0)
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

