#include"Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	//installs the program object specified by program as part of current rendering state.
	//http://docs.gl/gl4/glUseProgram
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	//glUniform modifies the value of a uniform variable or a uniform variable array. The location of the uniform variable to be modified is specified by location,
	//which should be a value returned by glGetUniformLocation. glUniform operates on the program object that was made part of current state by calling glUseProgram.
	//http://docs.gl/gl4/glUniform
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//glCreateProgram creates an empty program object and returns a non-zero value by which it can be referenced.
	//A program object is an object to which shader objects can be attached.
	//http://docs.gl/gl4/glCreateProgram
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//Attaches a shader object to a program object
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	//glLinkProgram links the program object specified by program. If any shader objects of type GL_VERTEX_SHADER are attached to program,
	//they will be used to create an executable that will run on the programmable vertex processor. If any shader objects of type GL_FRAGMENT_SHADER are attached to program,
	//they will be used to create an executable that will run on the programmable fragment processor.
	//http://docs.gl/gl4/glLinkProgram
	GLCall(glLinkProgram(program));

	//checks to see whether the executables contained in program can execute given the current OpenGL state.
	//http://docs.gl/gl4/glValidateProgram
	GLCall(glValidateProgram(program));

	//once shaders are compiled and are part of the program, u can use the older shader objects created
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

// Utility function that goes reads a file line by line and divides and stores it two different shader strings
ShaderProgramSource Shader::ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type = ShaderType::NONE;
	std::string line;
	std::stringstream ss[2];

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else if (type != ShaderType::NONE)
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	//http://docs.gl/gl4/glCreateShader
	//Creates a shader object
	unsigned int id = glCreateShader(type);

	//pointer to data inside source
	const char* src = source.c_str();

	//http://docs.gl/gl4/glShaderSource
	//Replaces the source code in a shader object
	//Params: shader, count, string, length
	GLCall(glShaderSource(id, 1, &src, nullptr));

	//http://docs.gl/gl4/glCompileShader
	//Compiles a shader object
	GLCall(glCompileShader(id));

	int result;
	//returns in params the value of a parameter for a specific shader object.
	//http://docs.gl/gl4/glGetShader
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		//c++ doesn't allow message[length] since length is not a const, hence using _alloc
		char* message = (char*)_malloca(length * sizeof(char));

		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile  " << (type == GL_VERTEX_SHADER ? "Vertex" : "fragment") << "Shader!" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	//returns an integer that represents the location of a specific uniform variable within a program object.
	//http://docs.gl/gl4/glGetUniformLocation
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

	if (location == -1)
		std::cout << "warning: uniform " << name << " not found" << std::endl;
	else
		m_UniformLocationCache[name] = location;
	return 0;
}