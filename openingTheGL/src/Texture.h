#pragma once
#include"Renderer.h"

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

   // typically android has 8 texture slot, opengl max 32
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Width; }

private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
};
