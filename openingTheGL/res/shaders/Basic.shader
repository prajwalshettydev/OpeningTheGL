#shader vertex
#version 330 core

//input data into vertex shader using "in"
//taking data defined from VertexBufferLayout
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

//output data from vertex shader to fragment shader
out vec2 v_TexCoord;

uniform mat4 u_MVP; //Model view projection matrix

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

//input data from vertex shader to fragment shader
in vec2 v_TexCoord;

//u_ defines a uniform variable
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;// vec4(0.2, 0.0, 0.8, 1.0);
};