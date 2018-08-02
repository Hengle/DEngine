#ifdef _DGAPI_OPENGL

#include "DShaderPassOpGL.h"
#include <fstream>

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
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(m_vertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);

		std::ofstream fout;

		fout.open("shader-error.txt");

		//int i;
		//for (i = 0; i<VertexShaderErrorMessage.size(); i++)
		//{
		fout << &VertexShaderErrorMessage[0] << std::endl;

		fout << std::endl;

		fout << content << std::endl;
		//}

		fout.close();

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

		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(m_fragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);

		std::ofstream fout;

		fout.open("shader-error.txt");

		//int i;
		//for (i = 0; i<VertexShaderErrorMessage.size(); i++)
		//{
		fout << &FragmentShaderErrorMessage[0] << std::endl;

		fout << std::endl;

		fout << content << std::endl;
		//}

		fout.close();

		//printf("%s\n", &VertexShaderErrorMessage[0]);
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
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		//printf("%s\n", &ProgramErrorMessage[0]);

		std::ofstream fout;

		fout.open("shader-error.txt");

		//int i;
		//for (i = 0; i<VertexShaderErrorMessage.size(); i++)
		//{
		fout << &ProgramErrorMessage[0] << std::endl;
		//}

		fout.close();

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

void DShaderProgramOpGL::GetResDesc(unsigned int index, DShaderResDesc & desc) const
{
	if (index < m_resParams.size())
	{
		desc = m_resParams.at(index);
	}
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
			SetParam(v, iter->second.type, iter->second.paramId);
			
		}
		else if (iter->second.isGlobal && gparams.find(iter->first) != gparams.end())
		{
			float* v = gparams.at(iter->first);
			SetParam(v, iter->second.type, iter->second.paramId);
			
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
			m_vertexUsage |= 1UL << DVertexUsage_POSITION;
		}
		if (strcmp(pname, "input_color") == 0)
		{
			m_vertexUsage |= 1UL << DVertexUsage_COLOR;
		}
		if (strcmp(pname, "input_normal") == 0)
		{
			m_vertexUsage |= 1UL << DVertexUsage_NORMAL;
		}
		if (strcmp(pname, "input_texcoord0") == 0)
		{
			m_vertexUsage |= 1UL << DVertexUsage_TEXCOORD0;
		}
		if (strcmp(pname, "input_tangent") == 0)
		{
			m_vertexUsage |= 1UL << DVertexUsage_TANGENT;
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

	m_resCount = 0;

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
			m_resCount += 1;
			m_resParams.push_back(resdesc);
		}
		else
		{
			propert.type = type;
			propert.paramId = glGetUniformLocation(programId, pname);
			if (strlen(pname) >= 2 && pname[0] == 'g' && pname[1] == '_')
			{
				propert.isGlobal = true;
			}
			m_properties.insert(std::pair<std::string, ShaderPropertyDescOpGL>(pname, propert));
		}
	}
}

void DShaderProgramOpGL::SetParam(float * v, GLenum type, GLuint id)
{
	if (type == GL_FLOAT)
	{
		GLfloat p = v[0];
		glUniform1fv(id, 1, &p);
	}
	else if (type == GL_FLOAT_VEC2)
	{
		GLfloat p[2] = { v[0],v[1] };
		glUniform2fv(id, 1, &p[0]);
	}
	else if (type == GL_FLOAT_VEC3)
	{
		GLfloat p[3] = { v[0],v[1], v[2] };
		glUniform3fv(id, 1, &p[0]);
	}
	else if (type == GL_FLOAT_VEC4)
	{
		GLfloat p[4] = { v[0],v[1], v[2] };
		glUniform4fv(id, 1, &p[0]);
	}
	else if (type == GL_FLOAT_MAT4)
	{
		GLfloat p[16] = {
			v[0],v[1],v[2],v[3],
			v[4],v[5],v[6],v[7],
			v[8],v[9],v[10],v[11],
			v[12],v[13],v[14],v[15],
		};
		glUniformMatrix4fv(id, 1, GL_FALSE, &p[0]);
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

