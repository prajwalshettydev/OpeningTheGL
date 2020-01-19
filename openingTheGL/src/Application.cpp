

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


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

// a util fn that goes reads a file line by line and divides and stores it two different shader strings
static ShaderProgramSource ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT =1
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
		else if(type != ShaderType::NONE)
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) 
{
	//http://docs.gl/gl4/glCreateShader
	//Creates a shader object
	unsigned int id = glCreateShader(type);

	//pointer to data inside source
	const char* src = source.c_str(); 

	//http://docs.gl/gl4/glShaderSource
	//Replaces the source code in a shader object
	glShaderSource(id, 1, &src, nullptr);
	
	//http://docs.gl/gl4/glCompileShader
	//Compiles a shader object
	glCompileShader(id);
	
	int result;
	//returns in params the value of a parameter for a specific shader object.
	//http://docs.gl/gl4/glGetShader
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		//c++ doesnt allow message[length] since length is not a const, hence using alloca
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile  " << (type == GL_VERTEX_SHADER ? "Vertex" : "fragement") << "Shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;

}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
	//glCreateProgram
	//Creates a program object
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//Attaches a shader object to a program object
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	//glLinkProgram links the program object specified by program. If any shader objects of type GL_VERTEX_SHADER are attached to program, 
	//they will be used to create an executable that will run on the programmable vertex processor. If any shader objects of type GL_FRAGMENT_SHADER are attached to program, 
	//they will be used to create an executable that will run on the programmable fragment processor.
	//http://docs.gl/gl4/glLinkProgram
	glLinkProgram(program);

	//checks to see whether the executables contained in program can execute given the current OpenGL state.
	//http://docs.gl/gl4/glValidateProgram 
	glValidateProgram(program);

	//once shaders are compiled and are part of the the program, u can use the older shader objects created
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	

	std::cout << glGetString(GL_VERSION) << std::endl;

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

#pragma region Creating Vertex Buffer


	unsigned int buffer;
	//create a new buffer in gpu,
	//ip1: 1 is the number of buffers to create, 
	//ip2: takes a memory pointer to store the id of the buffer, so if you want to store the bufferID in variable called buffer, then pass its address by using "&" as suffix.
	//http://docs.gl/gl4/glGenBuffers
	glGenBuffers(1, &buffer);

	//After creating the buffer, u need to select the buffer which is called "Binding" in opengl
	//http://docs.gl/gl4/glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	
	//Fill Buffer with the data
	//http://docs.gl/gl4/glBufferData
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 *  sizeof(float), positions, GL_STATIC_DRAW);

	//the vertext attrib array need to be enabled to be used
	//http://docs.gl/gl4/glEnableVertexAttribArray
	glEnableVertexAttribArray(0);

	//Define each attrib in the data which inturn is in the buffer
	//ip1: start
	//ip2: for eg, how many data blocks at once(currently float), so for a 2D position its 2 float blocks
	//ip5: data total size
	//offset from start of attrib array
	//http://docs.gl/gl4/glVertexAttribPointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


#pragma endregion

#pragma region Creating Index Buffer

	//Index Buffer Object
	unsigned int ibo;

	//create a new buffer in gpu,
	//ip1: 1 is the number of buffers to create, 
	//ip2: takes a memory pointer to store the id of the buffer, so if you want to store the bufferID in variable called buffer, then pass its address by using "&" as suffix.
	//http://docs.gl/gl4/glGenBuffers
	glGenBuffers(1, &ibo);

	//After creating the buffer, u need to select the buffer which is called "Binding" in opengl
	//GL_ELEMENT_ARRAY_BUFFER is a slot ofr index buffers
	//http://docs.gl/gl4/glBindBuffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	//Fill Buffer with the data
	//It has to be unsigned.
	//http://docs.gl/gl4/glBufferData
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


#pragma endregion

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	//installs the program object specified by program as part of current rendering state.
	//http://docs.gl/gl4/glUseProgram
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//http://docs.gl/gl4/glClear
		glClear(GL_COLOR_BUFFER_BIT);

		//3 represnets 3 vertices, 
		//Draw Call, draws the current bind buffer
		//http://docs.gl/gl4/glDrawArrays
		//glDrawArrays(GL_TRIANGLES, 0, 6);


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}

//#include<iostream>
//
//int main() 
//{
//	std::cout << "Hello" << std::endl;
//	std::cin.get();
//}