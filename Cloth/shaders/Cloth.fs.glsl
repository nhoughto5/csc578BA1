#version 330 core

in vec3 fColour;
out vec4 Color;

void main()
{
	Color = vec4(fColour, 1);
}