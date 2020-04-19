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
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
	//window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
			100.0f,100.0f, 0.0f, 0.0f,//0 => bottom left
			200.0f, 100.0f, 1.0f, 0.0f, //1 => right most edge
			200.0f, 200.0f, 1.0f, 0.5f,//2 => top right
			100.0f, 200.0f, 0.0f, 0.5f //3 =>
		};

		unsigned int indices[] = {
			0,1 ,2,
			2,3,0
		};

		GLCall(glEnable(GL_BLEND));
		//define how opengl will render alpha
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //to support transparency

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2); //two vertex positions
		layout.Push<float>(2); //two for uv
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		//This matrix specifies the screen size, -2.0 is the left edge and so on
		//https://glm.g-truc.net/0.9.2/api/a00245.html
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0)); //Control camera position, i.e glm::vec3(-100, 0, 0)); is kind of camera position now
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0)); //Control model's position
		glm::mat4 mvp = proj * view * model; //mul in opengl is right to left due to how the matrices are column aligned
		
		// Example: consider a Vertex position vp at (100,100,0)
		// Now multiplying it with the projection matrix will result in the VP to be converted between -1 to 1 space
		glm::vec4 vp = glm::vec4(100.0f, 100.0f, 0.0f, 1.0f);
		glm::vec4 result = proj * vp;

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/screen.png");
		texture.Bind(0);
		shader.SetUniform1i("u_Texture", 0); //0 cuz we bound our texture to slot zero

		//Unbind the program, vertex buffer and the index buffer.
		//and later in update you can set them, for each frame
		va.UnBind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();
			//first set the program and its uniforms(i.e for example "u_color")
			/*shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);*/

			renderer.Draw(va, ib, shader);

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
	/*GLCall(*/glfwTerminate()/*)*/;
	return 0;
}