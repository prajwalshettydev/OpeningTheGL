#pragma once
#include <string>


class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniforms:
	void SetUnifrom4f(const std::string& name, float v0, float v1, float f2, float f4);
private:

	unsigned int m_RendererID;
	// caching for unifrom

	unsigned int GetUnifromLocation(const std::string& name);

};
