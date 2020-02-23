/* Glew The OpenGL Extension Wrangler Library (GLEW) is a cross-platform open-source C/C++ extension loading library. GLEW provides efficient run-time mechanisms for determining which OpenGL
 * extensions are supported on the target platform. OpenGL core and extension functionality is exposed in a single header file. GLEW has been tested on a variety of operating systems,
 * including Windows, Linux, Mac OS X, FreeBSD, Irix, and Solaris
 * http://glew.sourceforge.net/
 * */
#include <GL/glew.h>

 //https://www.glfw.org/documentation.html
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//setting opengl version to 3.3
	//https://www.glfw.org/docs/latest/window_guide.html#window_hints_ctx
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Note: u cant set the GLFW_OPENGL_PROFILE to CORE without having a vertex array setup
	//https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//enable vsync
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float positions[] = {
			-0.5f, -0.5f, //0
			0.5f, -0.5f, //1
			0.5f, 0.5f, //2
			-0.5f, 0.5f, //3
		};

		unsigned int indices[] = {
			0,1 ,2,
			2,3,0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		
		//Unbind the program, vertex buffer and the index buffer.
		//and later in update you can set them, for each frame
		va.UnBind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			//http://docs.gl/gl4/glClear
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			//first set the program and its uniforms(i.e for example "u_color")
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			va.Bind();
			ib.Bind();

			// render primitives from array data
			// specifies multiple geometric primitives with very few subroutine calls.
			// http://docs.gl/gl4/glDrawElements
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.05f)
				increment = 0.05f;

			r += increment;

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}
	}
	GLCall(glfwTerminate());
	return 0;
}