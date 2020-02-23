#pragma once
#include <string>


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniforms:
	void SetUnifrom4f(const std::string& name, float v0, float v1, float v2, float v3);
private:

	unsigned int m_RendererID;
	std::string m_FilePath;
	// caching for uniform

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUnifromLocation(const std::string& name);

};
