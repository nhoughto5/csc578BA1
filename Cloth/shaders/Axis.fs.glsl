#version 330 core
in vec3 fragColour;
out vec3 colour;

void main()
{
	colour = fragColour;
	//colour = vec4(1, 0, 0,1);
}