#version 330 core

layout(location = 0) in vec2 vertexPosition;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	gl_Position =  projection * model * vec4(vertexPosition.xy, 1.0, 1.0);	
}
