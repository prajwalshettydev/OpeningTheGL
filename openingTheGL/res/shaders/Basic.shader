#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
void main()
{
	gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

//u_ defines a uniform variable
uniform vec4 u_Color;

void main()
{
	color = vec4(0.2, 0.0, 0.8, 1.0); //u_Color;// vec4(0.2, 0.0, 0.8, 1.0);
};
