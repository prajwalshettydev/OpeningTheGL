#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	//flips the texture, makes it upside down, cuz bottom left in opengl is 0,0 for png its the oposite.
	stbi_set_flip_vertically_on_load(1);
	//our local storage of the texture
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); //4 is the desired channels, i.e RGBA

	//glGenTextures returns n texture names in textures.
	//params: n, textures
	GLCall(glGenTextures(1, &m_RendererID));
	//use a named texture
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//set some texture settings
	//Four settings params set here: GL_TEXTURE_MIN_FILTER,GL_TEXTURE_MAG_FILTER,GL_TEXTURE_WRAP_S,GL_TEXTURE_WRAP_T
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//specify a two-dimensional texture image
	//basically loading into opengl, aka gpu mem
	//http://docs.gl/gl4/glTexImage2D
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	//unbind texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer); //free the local buffer since not required
}

Texture::~Texture()
{
	//delete the texture from the gpu
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	//select the texture slot and make it active
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	//then set the texture to the active slot
	//use a named texture
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	//unbind texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}