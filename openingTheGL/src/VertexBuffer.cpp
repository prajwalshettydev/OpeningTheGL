#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	//create a new buffer in gpu,
	//ip1: 1 is the number of buffers to create,
	//ip2: takes a memory pointer to store the id of the buffer, so if you want to store the bufferID in variable called buffer, then pass its address by using "&" as suffix.
	//http://docs.gl/gl4/glGenBuffers
	GLCall(glGenBuffers(1, &m_RendererID));

	//After creating the buffer, u need to select the buffer which is called "Binding" in opengl
	//http://docs.gl/gl4/glBindBuffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

	//Fill Buffer with the data
	//http://docs.gl/gl4/glBufferData
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	//After creating the buffer, u need to select the buffer which is called "Binding" in opengl
	//http://docs.gl/gl4/glBindBuffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	//After creating the buffer, u need to select the buffer which is called "Binding" in opengl
	//http://docs.gl/gl4/glBindBuffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}