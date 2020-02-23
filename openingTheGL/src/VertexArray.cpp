#include"VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	//create a new VertexArray,
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		//the vertex attribute array need to be enabled to be used
		//http://docs.gl/gl4/glEnableVertexAttribArray
		GLCall(glEnableVertexAttribArray(i));

		//Define each attribute in the data which in turn is in the buffer
		//ip1: start
		//ip2: for example, how many data blocks at once(currently float), so for a 2D position its 2 float blocks
		//ip5: data total size
		//offset from start of attribute array
		//for example: GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
		//http://docs.gl/gl4/glVertexAttribPointer
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	//After creating the array, u need to select the array which is called "Binding" in opengl
	GLCall(glBindVertexArray(m_RendererID));
	//after binding the vertex array, in the next set of code you are binding a vertex buffer and defining attributes' structure in the vertex buffer,
	//Which will in turn be linked to the just created vertex array so that we can eliminate calling vertex attribute setup function every frame
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}
