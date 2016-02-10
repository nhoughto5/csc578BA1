#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColour;
out vec3 fragColour;
uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(vPosition, 1);
	fragColour = vColour;
}
